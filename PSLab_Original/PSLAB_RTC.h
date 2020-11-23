#ifndef PSLAB_RTC_H
#define	PSLAB_RTC_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern unsigned char time_array[7];
    
    unsigned char setRealTime(unsigned char seconds, 
            unsigned char minutes, 
            unsigned char hours, 
            unsigned char day, 
            unsigned char date, 
            unsigned char month, 
            unsigned char year, 
            unsigned char control_reg);
    unsigned char* getRealTime(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PSLAB_RTC_H */

