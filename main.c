/*
 * File:   main.c
 * Author: Ryo Wakabayashi
 *
 * Created on 18/12/7, 16:03
 */

#include "main.h"


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



void interrupt Handler(void)
{
   
}