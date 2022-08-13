#ifndef DMA_H
#define DMA_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    /**
      Section: Data Types
     */

    /** DMA Modes
 
     @Summary 
       Defines the current DMA mode in use
 
     @Description
       This routine defines the current mode that is used to handle during
       interrupt events
 
     Remarks:
       None
     */
    typedef enum {
        DMA_MODES_ONE_CHANNEL,
        DMA_MODES_TWO_CHANNEL,
        DMA_MODES_THREE_CHANNEL,
        DMA_MODES_FOUR_CHANNEL
    } DMA_MODES;

    /** DMA Channel Definition
 
     @Summary 
       Defines the channels available for DMA
 
     @Description
       This routine defines the channels that are available for the module to use.
 
     Remarks:
       None
     */
    typedef enum {
        DMA_CHANNEL_0 = 0,
        DMA_CHANNEL_1 = 1,
        DMA_CHANNEL_2 = 2,
        DMA_CHANNEL_3 = 3
    } DMA_CHANNEL;

    /** DMA Operating Modes Definition
 
     @Summary 
       Defines the operating modes for DMA modules
 
     @Description
       This routine defines the order of block transfers from/to DMA buffer
 
     Remarks:
       None
     */
    typedef enum {
        DMA_OPERATING_MODE_CONTINUOUS            = 0b00,
        DMA_OPERATING_MODE_ONE_SHOT              = 0b01,
        DMA_OPERATING_MODE_CONTINUOUS_PING_PONG  = 0b10,
        DMA_OPERATING_MODE_ONE_SHOT_PINT_PONG    = 0b11
    } DMA_OPERATING_MODE;

    /** DMA Peripheral IRQ Number
 
     @Summary 
       Defines the DMA Peripheral IRQ number.
 
     @Description
       This routine defines the DMA Peripheral IRQ number.
 
     Remarks:
       None
     */
    typedef enum {
        DMA_PERIPHERAL_IRQ_INT0         = 0x00,
        DMA_PERIPHERAL_IRQ_IC1          = 0x01,
        DMA_PERIPHERAL_IRQ_IC2          = 0x05,
        DMA_PERIPHERAL_IRQ_IC3          = 0x25,
        DMA_PERIPHERAL_IRQ_IC4          = 0x26,
        DMA_PERIPHERAL_IRQ_OC1          = 0x02,
        DMA_PERIPHERAL_IRQ_OC2          = 0x06,
        DMA_PERIPHERAL_IRQ_OC3          = 0x19,
        DMA_PERIPHERAL_IRQ_OC4          = 0x1A,
        DMA_PERIPHERAL_IRQ_TMR2         = 0x07,
        DMA_PERIPHERAL_IRQ_TMR3         = 0x08,
        DMA_PERIPHERAL_IRQ_TMR4         = 0x1B,
        DMA_PERIPHERAL_IRQ_TMR5         = 0x1C,
        DMA_PERIPHERAL_IRQ_SPI1         = 0x0A,
        DMA_PERIPHERAL_IRQ_SPI2         = 0x21,
        DMA_PERIPHERAL_IRQ_UART1_TX     = 0x0C,
        DMA_PERIPHERAL_IRQ_UART1_RX     = 0x0B,
        DMA_PERIPHERAL_IRQ_UART2_TX     = 0x1F,
        DMA_PERIPHERAL_IRQ_UART2_RX     = 0x1E,
        DMA_PERIPHERAL_IRQ_ADC1         = 0x0D
    } DMA_PERIPHERAL_IRQ_NUMBER;

    /**
      Section: Interface Routines
     */

    /**
      @Summary
        This function initializes DMA instance : 1

      @Description
        This routine initializes the DMA driver instance for : 1
        index, making it ready for clients to open and use it. It also initializes any
        internal data structures.
        This routine must be called before any other DMA routine is called. 

      @Preconditions
        None.

      @Param
        None.

      @Returns
        None.

      @Comment
   
 
      @Example
        <code>
            unsigned short int srcArray[100];
            unsigned short int dstArray[100];
            int i;
            int count;
            for (i=0; i<100; i++)
            {
                srcArray[i] = i+1;
                dstArray[i] = 0;
            }
        
            DMA_Initialize();
            DMA_SoftwareTriggerEnable(CHANNEL1);
        
            count = DMA_TransferCountGet;
            while(count > 0)
            {
            while(
            DMA_SoftwareTriggerEnable(CHANNEL1);
            }
        </code>

     */
    void DMA_Initialize(void);

    void DMA_InitializeChannel0(void);
    void DMA_InitializeChannel1(void);
    void DMA_InitializeChannel2(void);
    void DMA_InitializeChannel3(void);

    /**
      @Summary
        Clears the interrupt request flag.

      @Description
        This routine is used to clear the interrupt request flag. This routine sets the value of the DMAIF bit to 0.
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_FlagInterruptClear(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                IFS0bits.DMA0IF = 0;
                break;
            case DMA_CHANNEL_1:
                IFS0bits.DMA1IF = 0;
                break;
            case DMA_CHANNEL_2:
                IFS1bits.DMA2IF = 0;
                break;
            case DMA_CHANNEL_3:
                IFS2bits.DMA3IF = 0;
                break;
            default:break;

        }
    }

    /**
    @Summary
        Enables the interrupt for a DMA channel

      @Description
        This routine is used to enable an interrupt for a DMA channel. This routine
        sets the value of the DMAIE bit to 1.
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_InterruptEnable(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                IEC0bits.DMA0IE = 1;
                break;
            case DMA_CHANNEL_1:
                IEC0bits.DMA1IE = 1;
                break;
            case DMA_CHANNEL_2:
                IEC1bits.DMA2IE = 1;
                break;
            case DMA_CHANNEL_3:
                IEC2bits.DMA3IE = 1;
                break;
            default:break;

        }
    }

    /**
    @Summary
     Disables the interrupt for a DMA channel

    @Description
     This routine is used to disable an interrupt for a DMA channel. This routine
     sets the value of the DMAIE bit to 0.
 
    @Preconditions
      DMA_Initializer() function should have been 
      called before calling this function.
 
    @Returns
     None

    @Param
     None
  
    @Example
     Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_InterruptDisable(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                IEC0bits.DMA0IE = 0;
                break;
            case DMA_CHANNEL_1:
                IEC0bits.DMA1IE = 0;
                break;
            case DMA_CHANNEL_2:
                IEC1bits.DMA2IE = 0;
                break;
            case DMA_CHANNEL_3:
                IEC2bits.DMA3IE = 0;
                break;
            default:break;
        }
    }

    /**
     @Summary
      Enables the channel in the DMA

     @Description
      This routine is used to enable a channel in the DMA. This routine
      sets the value of the CHEN bit to 1.
 
     @Preconditions
      DMA_Initializer() function should have been 
      called before calling this function.
 
     @Returns
      None

     @Param
      None
  
     @Example
     Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_ChannelEnable(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0CONbits.CHEN = 1;
                break;
            case DMA_CHANNEL_1:
                DMA1CONbits.CHEN = 1;
                break;
            case DMA_CHANNEL_2:
                DMA2CONbits.CHEN = 1;
                break;
            case DMA_CHANNEL_3:
                DMA3CONbits.CHEN = 1;
                break;
            default: break;
        }
    }

    /**
     @Summary
     Disables the channel in the DMA

     @Description
      This routine is used to disable a channel in the DMA. This routine
      sets the value of the CHEN bit to 0.
 
    @Preconditions
     DMA_Initializer() function should have been 
     called before calling this function.
 
    @Returns
     None

    @Param
     None
  
    @Example
     Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_ChannelDisable(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0CONbits.CHEN = 0;
                break;
            case DMA_CHANNEL_1:
                DMA1CONbits.CHEN = 0;
                break;
            case DMA_CHANNEL_2:
                DMA2CONbits.CHEN = 0;
                break;
            case DMA_CHANNEL_3:
                DMA3CONbits.CHEN = 0;
                break;
            default: break;
        }
    }

    /**
      @Summary
      Enables one-shot mode.

     @Description
      This routine is used to enable one-shot mode on a channel in the DMA. In
      one-shot mode, the channel is automatically disabled after finishing
      moving the data block.
 
    @Preconditions
     DMA_Initializer() function should have been 
     called before calling this function.
 
    @Returns
     None

    @Param
     Pass in the required channel from the DMA_CHANNEL list.
  
    @Example
     DMA_SetOneShotMode(DMA_CHANNEL_0); // Set one-shot mode on DMA0.
     */
    inline static void DMA_SetOneShotMode(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0CONbits.MODE |= 0b01;
                break;
            case DMA_CHANNEL_1:
                DMA1CONbits.MODE |= 0b01;
                break;
            case DMA_CHANNEL_2:
                DMA2CONbits.MODE |= 0b01;
                break;
            case DMA_CHANNEL_3:
                DMA3CONbits.MODE |= 0b01;
                break;
            default: break;
        }
    }
    
    /**
      @Summary
        Sets the transfer count of the DMA

      @Description
        This routine is used to set the DMA transfer count. This routine sets the
        value of the DMACNT register. 
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_TransferCountSet(DMA_CHANNEL channel, uint16_t transferCount) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0CNT = transferCount;
                break;
            case DMA_CHANNEL_1:
                DMA1CNT = transferCount;
                break;
            case DMA_CHANNEL_2:
                DMA2CNT = transferCount;
                break;
            case DMA_CHANNEL_3:
                DMA3CNT = transferCount;
                break;
            default: break;
        }
    }

    /**
     @Summary
      Returns the transfer count of the DMA

    @Description
     This routine is used to determine the DMA transfer count. This routine
     returns the value of the DMACNT register. 
 
    @Preconditions
      DMA_Initializer() function should have been 
      called before calling this function.
 
    @Returns
     Returns the transfer count of the DMA

    @Param
     None
  
    @Example
     Refer to DMA_Initializer(); for an example
     */
    inline static uint16_t DMA_TransferCountGet(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                return (DMA0CNT);
            case DMA_CHANNEL_1:
                return (DMA1CNT);
            case DMA_CHANNEL_2:
                return (DMA2CNT);
            case DMA_CHANNEL_3:
                return (DMA3CNT);
            default: return 0;
        }
    }

    /**
     @Summary
      Initiates a transfer on the requested DMA channel.

     @Description
      This routine is used to initiate a transfer on the requested DMA channel. When a transfer on the requested channel is initiated the routine
      returns the value of the FORCE bit. It returns 0 otherwise.
 
     @Preconditions
       DMA_Initializer() function should have been 
       called before calling this function.
 
     @Returns
      Returns true if the transfer on the requested channel is initiated 

     @Param
      None
  
     @Example
      Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_SoftwareTriggerEnable(DMA_CHANNEL channel) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0REQbits.FORCE = 1;
                break;
            case DMA_CHANNEL_1:
                DMA1REQbits.FORCE = 1;
                break;
            case DMA_CHANNEL_2:
                DMA2REQbits.FORCE = 1;
                break;
            case DMA_CHANNEL_3:
                DMA3REQbits.FORCE = 1;
                break;
            default: break;
        }
    }

    /**
      @Summary
        Sets the address for register A in the DMA

      @Description
        This routine is used to set the address in register A for a DMA channel. 
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_StartAddressASet(DMA_CHANNEL channel, uint16_t address) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0STAL = address;
                DMA0STAH = 0;
                break;
            case DMA_CHANNEL_1:
                DMA1STAL = address;
                DMA1STAH = 0;
                break;
            case DMA_CHANNEL_2:
                DMA2STAL = address;
                DMA2STAH = 0;
                break;
            case DMA_CHANNEL_3:
                DMA3STAL = address;
                DMA3STAH = 0;
                break;
            default: break;
        }
    }

    /**
      @Summary
        Sets high and low addresss for register A in the DMA

      @Description
        This routine is used to set both the high and low address in register A 
        for a DMA channel. 
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        channel: DMA channel
        address_low: Low address location
        address_high: high address location
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_StartAddressAFullSet(DMA_CHANNEL channel, uint16_t address_low, uint16_t address_high) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0STAL = address_low;
                DMA0STAH = address_high;
                break;
            case DMA_CHANNEL_1:
                DMA1STAL = address_low;
                DMA1STAH = address_high;
                break;
            case DMA_CHANNEL_2:
                DMA2STAL = address_low;
                DMA2STAH = address_high;
                break;
            case DMA_CHANNEL_3:
                DMA3STAL = address_low;
                DMA3STAH = address_high;
                break;
            default: break;
        }
    }

    /**
      @Summary
        Sets the address for register B in the DMA

      @Description
        This routine is used to set the address in register B for a DMA channel. 
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */

    inline static void DMA_StartAddressBSet(DMA_CHANNEL channel, uint16_t address) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0STBL = address;
                DMA0STBH = 0;
                break;
            case DMA_CHANNEL_1:
                DMA1STBL = address;
                DMA1STBH = 0;
                break;
            case DMA_CHANNEL_2:
                DMA2STBL = address;
                DMA2STBH = 0;
                break;
            case DMA_CHANNEL_3:
                DMA3STBL = address;
                DMA3STBH = 0;
                break;
            default: break;
        }
    }

    /**
    @Summary
      Gets the address for register A in the DMA

    @Description
      This routine is used to get the address in register A for a DMA channel. 
 
    @Preconditions
      DMA_Initializer() function should have been 
      called before calling this function.
 
    @Returns
      None

    @Param
      None
  
    @Example
      Refer to DMA_Initializer(); for an example
     */
    inline static uint16_t DMA_StartAddressAGet(DMA_CHANNEL channel) {
        uint16_t address;

        switch (channel) {
            case DMA_CHANNEL_0:
                address = DMA0STAL;
                break;
            case DMA_CHANNEL_1:
                address = DMA1STAL;
                break;
            case DMA_CHANNEL_2:
                address = DMA2STAL;
                break;
            case DMA_CHANNEL_3:
                address = DMA3STAL;
                break;
            default:
                address = 0;
                break;
        }
        return address;
    }

    /**
    @Summary
      Gets the address for register B in the DMA

    @Description
      This routine is used to get the address in register B for a DMA channel. 
 
    @Preconditions
      DMA_Initializer() function should have been 
      called before calling this function.
 
    @Returns
      None

    @Param
      None
  
    @Example
      Refer to DMA_Initializer(); for an example
     */
    inline static uint16_t DMA_StartAddressBGet(DMA_CHANNEL channel) {
        uint16_t address;

        switch (channel) {
            case DMA_CHANNEL_0:
                address = DMA0STBL;
                break;
            case DMA_CHANNEL_1:
                address = DMA1STBL;
                break;
            case DMA_CHANNEL_2:
                address = DMA2STBL;
                break;
            case DMA_CHANNEL_3:
                address = DMA3STBL;
                break;
            default:
                address = 0;
                break;
        }
        return address;
    }

    /**
    @Summary
    Sets the peripheral address in the DMA

  @Description
    This routine is used to set the peripheral address for a DMA channel. 
 
  @Preconditions
    DMA_Initializer() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
    Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_PeripheralAddressSet(DMA_CHANNEL channel, uint16_t address) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0PAD = address;
                break;
            case DMA_CHANNEL_1:
                DMA1PAD = address;
                break;
            case DMA_CHANNEL_2:
                DMA2PAD = address;
                break;
            case DMA_CHANNEL_3:
                DMA3PAD = address;
                break;
            default: break;
        }
    }

    /**
      @Summary
        Returns true when there is a Peripheral Write Collision Event

      @Description
        This routine is used to determine if there is a Peripheral Write Collision Event. This routine
        returns the value of the PWCOL bit in DMAPWC register. When there is a Peripheral Write Collision Event, the routine
        returns 1. It returns 0 otherwise.
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        Returns true if there is a Peripheral Write Collision Event

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static bool DMA_IsPeripheralWriteCollision(uint16_t dmaChannel) {
        return DMAPWC & (1 << dmaChannel);
    }

    /**
      @Summary
        Returns true when there is a Request Collision Event

      @Description
        This routine is used to determine if there is a Request Collision Event. This routine
        returns the value of the RQCOL bit in DMARQC register. When there is a Request Collision Event, the routine
        returns 1. It returns 0 otherwise.
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        Returns true if there is a Request Collision Event

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static bool DMA_IsRequestCollision(uint16_t dmaChannel) {
        return DMARQC & (1 << dmaChannel);
    }

    /**
      @Summary
        Sets the requested DMA Channel IRQ Select register with the requested peripheral IRQ number.

      @Description
        This routine is used to set the requested DMA Channel IRQ Select register with the requested peripheral IRQ number.
 
      @Preconditions
        DMA_Initializer() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to DMA_Initializer(); for an example
     */
    inline static void DMA_PeripheralIrqNumberSet(DMA_CHANNEL channel, DMA_PERIPHERAL_IRQ_NUMBER irqNumber) {
        switch (channel) {
            case DMA_CHANNEL_0:
                DMA0REQbits.IRQSEL = irqNumber;
                break;
            case DMA_CHANNEL_1:
                DMA1REQbits.IRQSEL = irqNumber;
                break;
            case DMA_CHANNEL_2:
                DMA2REQbits.IRQSEL = irqNumber;
                break;
            case DMA_CHANNEL_3:
                DMA3REQbits.IRQSEL = irqNumber;
                break;
            default: break;
        }
    }

    /**
      @Summary
        Set channel mode for DMA1 interrupts when using logic analyzer

      @Description
        This routine is a setter method to prevent using global variables in ISR

      @Param
        LOGICANALYZER_DMA_MODES

      @Returns
        None
     */
    void DMA_SetLogicAnalyzerChannelMode(DMA_MODES mode);

    /**
      @Summary
        Prepares DMA0 channel to connect with IC1 module

      @Description
        This routine prepares pads and sources to capture timer values in IC1
        upon a signal requesting the DMA module to start a transfer

      @Preconditions
        DMA0 channel should be turned off prior to calling this routine and the
        channel could be turned on after this routine is complete

      @Param
        count: number of transfers

        address: pointer to the buffer location to store data

        trigger: signal source to generate a data transfer action

      @Returns
        None
     */
    void DMA_PrepareChannel0(uint16_t count, volatile uint16_t* low_address,  
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger);

    /**
      @Summary
        Prepares DMA1 channel to connect with IC2 module

      @Description
        This routine prepares pads and sources to capture timer values in IC2
        upon a signal requesting the DMA module to start a transfer

      @Preconditions
        DMA0 channel should be turned off prior to calling this routine and the
        channel could be turned on after this routine is complete

      @Param
        count: number of transfers

        address: pointer to the buffer location to store data

        trigger: signal source to generate a data transfer action

      @Returns
        None
     */
    void DMA_PrepareChannel1(uint16_t count, volatile uint16_t* low_address,  
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger);

    /**
      @Summary
        Prepares DMA2 channel to connect with IC3 module

      @Description
        This routine prepares pads and sources to capture timer values in IC3
        upon a signal requesting the DMA module to start a transfer

      @Preconditions
        DMA0 channel should be turned off prior to calling this routine and the
        channel could be turned on after this routine is complete

      @Param
        count: number of transfers

        address: pointer to the buffer location to store data

        trigger: signal source to generate a data transfer action

      @Returns
        None
     */
    void DMA_PrepareChannel2(uint16_t count, volatile uint16_t* low_address, 
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger);

    /**
      @Summary
        Prepares DMA3 channel to connect with IC4 module

      @Description
        This routine prepares pads and sources to capture timer values in IC4
        upon a signal requesting the DMA module to start a transfer

      @Preconditions
        DMA0 channel should be turned off prior to calling this routine and the
        channel could be turned on after this routine is complete

      @Param
        count: number of transfers

        address: pointer to the buffer location to store data

        trigger: signal source to generate a data transfer action

      @Returns
        None
     */
    void DMA_PrepareChannel3(uint16_t count, volatile uint16_t* low_address, 
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger);
    
    // Enable all four channels in one go
    void DMA_EnableAllChannels(void);
    
    // Disable all four channels in one go
    void DMA_DisableAllChannels(void);

    // Getters and Setters
    void SetDMA_MODE(DMA_MODES mode);
    DMA_MODES GetDMA_MODE(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif  // DMA_H
