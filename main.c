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
GPIO1	Status LED
GPIO2	Tact Switch 1
GPIO3	Tact Switch 2
GPIO4	Fan output
GPIO5	BCM output
*/

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
            sw_cnt1 = 1000;
            FAN_PINOUT = !FAN_PINOUT;
        }
        else if(sw_cnt1>0)
        {
            sw_cnt1--;
        }

        if((SW2_Read()==0)&&(sw_cnt2==0))
        {
            sw_cnt2 = 1000;
            FAN_PINOUT = !FAN_PINOUT;
        }
        else if(sw_cnt2>0)
        {
            sw_cnt2--;
        }
        
        if(ADC_Scan_Voltage(0)<562)
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

//Full charged 6.0V = 1.5V x 4
//Input voltage when full charaged: 3.0V
//R1 = 5.6k
//R2 = 5.6k
//End voltage:4.2V = 1.05V x 4
//Input voltage when end voltage:2.1V
//ADC value :651
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
