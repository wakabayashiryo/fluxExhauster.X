/*
 * File:   main.c
 * Author: Ryo Wakabayashi
 *
 * Created on 18/12/7, 16:03
 */

#include "main.h"

/*
pin definitation
GPIOx 	contents
GPIO0	Detect voltage
GPIO1	Tact Switch 1
GPIO2	Tact Switch 2
GPIO3	Status LED
GPIO4	Fan output
GPIO5	BCM output
*/

void main(void) 
{
    Device_Startup();
    
    //peripheral initialize functions etc...
    
    
    while(1)
    {
        GPIO1 = !GPIO1;
        __delay_ms(500);
    }       
}

void Device_Startup(void)
{
    TRISIO  = 0x00;
    CMCON   = 0x07;
    ANSEL   = 0x00;
    IOC     = 0x00;
        
    WPU     = 0x00;
    
    GPIO    = 0x00;
}

void SW_Init(void)
{
    TRISIO  = 0x06;     //GPIO1,2 are input for Switches
}

inline uint8_t SW1_Read(void)
{
    return (uint8_t)GPIO1;
}

inline uint8_t SW2_Read(void)
{
    return (uint8_t)GPIO2;
}

#define FAN_ON()    GPIO4 = 1;
#define FAN_OFF()   GPIO4 = 0;

#define STATLED_ON()    GPIO4 = 1;
#define STATLED_OFF()   GPIO4 = 0;

void ADC_Init(void)
{
    TRISIO  |= (1<<0);  //GPIOx is input mode
    ANSEL   |= (1<<0);  //GPIOx is analog input
    ANSEL   |= (1<<4);  //A/D conversion clock source is 1/8
    
    ADCON0  = 0x80;     //Result formed is right justified
                        //reference voltage VDD
}

uint16_t ADC_Scan_Voltage(uint8_t channel)
{
    ANSEL   &=   (uint8_t)~(3<<4);        //clear channel select bits
    ANSEL   |=   (uint8_t) (channel<<4);  //set channel select bits
    
    GO_nDONE = 1;
    
    __delay_us(20);
    
    GO      = 1;
    while(GO);
    
    return (uint16_t)((ADRESH<<8)|ADRESL);
}   

uint8_t duty_param = 0;

void BCM_Init(void)
{
	T1CON &= ~(1<<6);		//Timer1 gate disable
	T1CON &= ~(3<<4);		//input clock source 1:1
	T1CON &= ~(1<<3);		//LP oscillator is off
	T1CON &= ~(1<<1);		//Timer clock source is internal
	
	TMR1 = 0xFFFF;
	
	TMR1IF = 0;
	TMR1IE = 1;
	
	PEIE = 1;
	GIE  = 1;
}

void BCM_Set_Duty(uint8_t duty)
{	
	duty_param = duty;
}

inline void BCM_Interrupt(void)
{
	static uint8_t bitmask = 0;
	
	if(TMR1IE&&TMR1IF)
	{
		if(duty_param&(1<<bitmask))
			GPIO5 = 1;
		else
			GPIO5 = 0;
		
		TMR1 = 0xFFFF - (uint16_t)(1<<bitmask);
		
		if(++bitmask>7)bitmask = 0;
		
		TMR1IF = 0;
	}
}

void interrupt Handler(void)
{
   BCM_Interrupt();
}