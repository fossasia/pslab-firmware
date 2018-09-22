/******************************************************************************/
/******** This file contains NRF control modules of function.c file ***********/
/******************************************************************************/
#include "COMMANDS.h"
#include "Common_Functions.h"
#include "PSLAB_NRF.h"
#include "PSLAB_SPI.h"

BYTE rfCardPresent = 0;
BYTE nodecount = 0;
BYTE ca = 0;
char tmpstr[25];
BYTE i2c_list[NRF_REPORT_ROWS][NRF_ROW_LENGTH];
BYTE RXTX_ADDR[3] = {0x01, 0xAA, 0xAA}; //Randomly chosen address
BYTE TOKEN_ADDR[3] = {0xFF, 0xAA, 0xAA}; //Fixed address on pipe 2.

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void) {
    if ((ReadStatus()&0x42) == 0x42) {
        logit("Remote:");
        ca = ReadRegister(R_RX_PL_WID);
        ReadPayload(ca, &i2c_list[nodecount][0]); //Whatever the node said. Assuming it'll send I2C sensor list
        WriteRegister(NRF_STATUS, 0x30);
        tmpstr[2] = (char) (nodecount % 10 + 48);
        tmpstr[1] = (char) ((nodecount / 10) % 10 + 48);
        tmpstr[0] = (char) ((nodecount / 100) % 10 + 48);
        tmpstr[3] = '\0';
        logit(&tmpstr[0]);
        nodecount += 1;
        if (nodecount == 15) {
            nodecount = 0;
            logit("Nodecount exceeded. index reset to 0.");
        }
    }
    _INT1IF = 0;
}

/*Command set for the NRFL01+ radio*/
void nRF_Setup() {
    CSN_HIGH;
    CE_LOW;
    WriteRegister(NRF_CONFIG, 0x3F); //MASK max_rt TX_DS, 1 BYTE CRC, POWER UP, PRX
    WriteRegister(EN_AA, 0x01); //enable auto ack
    WriteRegister(DYNPD, 0x01); //enable dynamic payload
    WriteRegister(EN_RXADDR, 0x01); //Enable data pipe 0
    WriteRegister(SETUP_AW, 0x01); //3 BYTE address
    WriteRegister(SETUP_RETR, 0x25); //Retransmit enabled
    WriteRegister(FEATURE, 0x04); //enable dynamic payload length
    WriteRegister(RF_CH, 100); //Randomly chosen RF channel
    WriteRegister(RF_SETUP, 0x26); //250kbps, 0dBm(0x26) , 2MbPS ( 0x0E)
    //WriteRegister(RX_PW_P0, 0x01); //RX payload = 1 BYTE
    WriteAddress(RX_ADDR_P0, 3, RXTX_ADDR);
    WriteAddress(RX_ADDR_P1, 3, TOKEN_ADDR);
    WriteAddress(TX_ADDR, 3, RXTX_ADDR);

    FlushTXRX();
    if ((ReadRegister(NRF_CONFIG) & 0x08) != 0) {
        rfCardPresent = 0;
    }

    /*Configure falling edge interrupt on RPI34 /AN4/RB2*/
    INTCON2bits.INT1EP = 1; //negative interrupt on AN4
    RPINR0bits.INT1R = 34; //remap to RPI34

}

void RXMode() {
    WriteRegister(NRF_CONFIG, 0x3F); //1 BYTE CRC, POWER UP, PRX
    CE_HIGH;
    Delay_us(4); //Tpece2csn
}

void TXMode() {
    CE_LOW;
    WriteRegister(NRF_CONFIG, 0x3E); //1 BYTE CRC, POWER UP, PTX
}

void PowerDown() {
    CE_LOW;
    WriteRegister(NRF_CONFIG, 0);
}

BYTE RXChar() {
    BYTE data;
    ReadPayload(1, &data);
    WriteRegister(NRF_STATUS, 0x30);
    return data;
}

void TXChar(BYTE ch) {
    WritePayload(TX_PAYLOAD, 1, &ch);
    if (rfCardPresent) {
        //Wait for char to be sent
        BYTE stat;
        do {
            stat = ReadStatus();
        } while ((stat & 0x20) == 0);
    }
    //Clear status bit
    WriteRegister(NRF_STATUS, 0x20);
}

BYTE ReadDataAvailable() {
    BYTE stat = ReadStatus();
    return (stat & 0x40) != 0;
}

void FlushTXRX() {
    WriteRegister(NRF_STATUS, 0x30); //Clear: data RX ready, data sent TX, Max TX retransmits
    WriteCommand(FLUSH_RX);
    WriteCommand(FLUSH_TX);
}

// *************** Helper Methods for NRF ***************

void WriteRegister(BYTE reg, BYTE val) {
    CSN_LOW;
    spi_write8(W_REG | reg);
    spi_write8(val);
    CSN_HIGH;
}
//Address is 3-5 bytes, LSB first

void WriteAddress(BYTE reg, BYTE num, BYTE* addr) {
    CSN_LOW;
    spi_write8(W_REG | reg);
    for (i = 0; i < num; i++)
        spi_write8(addr[i]);
    CSN_HIGH;
    Nop();
    Nop();
    Nop();
}

BYTE ReadRegister(BYTE reg) {
    BYTE val;
    CSN_LOW;
    spi_write8(R_REG | reg);
    val = spi_write8(0x00);
    CSN_HIGH;
    return val;
}

BYTE ReadStatus() {
    BYTE val;
    CSN_LOW;
    val = spi_write8(R_STATUS);
    CSN_HIGH;
    return val;
}

void WriteCommand(BYTE command) {
    CSN_LOW;
    spi_write8(command);
    CSN_HIGH;
}

void WritePayload(BYTE addr, BYTE num, BYTE* data) {
    CSN_LOW;
    spi_write8(addr);
    for (i = 0; i < (num & 0x3F); i++) spi_write8(data[i]);
    CSN_HIGH;
    if (num & 0x80) {
        CE_HIGH;
        Delay_us(12); //Thce (10us) + a bit (2us)
        CE_LOW;
    }
}

void ReadPayload(BYTE num, BYTE* data) {
    CSN_LOW;
    spi_write8(RX_PAYLOAD);
    for (i = 0; i < num; i++)
        data[i] = spi_write8(0);
    CSN_HIGH;
    WriteRegister(NRF_STATUS, 0x40);
}
