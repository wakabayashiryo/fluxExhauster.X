/*
 * File:   main.c
 * Author: Ryo Wakabayashi
 *
 * Created on 18/12/7, 16:03
 */

#include "main.h"

void SW_Init(void);
inline uint8_t SW1_Read(void);
inline uint8_t SW2_Read(void);

void ADC_Init(void);
uint16_t ADC_Scan_Voltage(uint8_t channel);

#define FAN_PINOUT      GPIO4

#define LED_PINOUT      GPIO5

#define STATLED_ON()    GPIO1 = 0
#define STATLED_OFF()   GPIO1 = 1

void main(void) 
{
    uint16_t sw_cnt1 = 0;
    uint16_t sw_cnt2 = 0;
    
    Device_Startup();
    
    SW_Init();
    
    ADC_Init();
    
    while(1)
    {
        if((SW1_Read()==0)&&(sw_cnt1==0))
        {
            sw_cnt1 = 2500;
            LED_PINOUT = !LED_PINOUT;
        }
        else if((SW1_Read()==1)&&(sw_cnt1>0))
        {
            sw_cnt1--;
        }

        if((SW2_Read()==0)&&(sw_cnt2==0))
        {
            sw_cnt2 = 2500;
            FAN_PINOUT = !FAN_PINOUT;
        }
        else if((SW2_Read()==1)&&(sw_cnt2>0))
        {
            sw_cnt2--;
        }
        
        if(ADC_Scan_Voltage(0)<652)
            STATLED_ON();
        else
            STATLED_OFF();
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
	
	LED_PINOUT = 0;
	FAN_PINOUT = 0;
}

void SW_Init(void)
{
    TRISIO  = 0x0C;     //GPIO2,3 are input for Switches
}

inline uint8_t SW1_Read(void)
{
    return (uint8_t)GPIO2;
}

inline uint8_t SW2_Read(void)
{
    return (uint8_t)GPIO3;
}

void ADC_Init(void)
{
    TRISIO  |= (1<<0);  //GPIOx is input mode
    ANSEL   |= (1<<0);  //GPIOx is analog input
    ANSEL   |= (1<<4);  //A/D conversion clock source is 1/8
    
    ADCON0  = 0x81;     //Result formed is right justified
                        //reference voltage VDD
}

uint16_t ADC_Scan_Voltage(uint8_t channel)
{
    ADCON0   &=   (uint8_t)~(3<<2);        //clear channel select bits
    ADCON0   |=   (uint8_t) (channel<<2);  //set channel select bits
    
    GO_nDONE = 1;
   
    while(GO_nDONE);
    
    return (uint16_t)((ADRESH<<8)|ADRESL);
}   
