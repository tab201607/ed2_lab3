//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "LCD8.h"
 #include <stdio.h>
 #include <stdlib.h>

//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);



//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    int adc0, adc1;
    char cnt;
    
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       PORTEbits.RE0= 0;       //Slave 1 Select
       __delay_ms(10);
       
       //Tomar  datos del contador y del adc del primer micro
       spiWrite(1);
       __delay_us(100);
       adc0 = spiRead() * 19;
       __delay_us(100);
       spiWrite(0);
       __delay_us(100);
       cnt = spiRead();
       
       __delay_ms(10);
       PORTEbits.RE0 = 1;       //Slave 1Deselect
       PORTEbits.RE1 = 0;       //Slave 2 Select
       
       __delay_ms(10);
       
       //tomar micro del segundo
       spiWrite(0);
       adc1 = spiRead() * 19;
       
       __delay_ms(10);
       PORTEbits.RE1 = 1;       //Slave 2 Deselect 
     
       __delay_ms(1);
        

            //Colocar datos en strings
        char display0[10]; 
        char display1[10];
        char display2[50];
        sprintf(display0, "%umV", adc0);
        sprintf(display1, "%umV", adc1);
        sprintf(display2, "%u", cnt);
        
        
        
        //desplegar datos en pantalla
        Lcd_Set_Cursor(2,1);
        __delay_us(10);
        Lcd_Write_String("                                 ");
        Lcd_Set_Cursor(2,1);
        __delay_us(10);
        Lcd_Write_String(display0);
        Lcd_Set_Cursor(2,8);
        __delay_us(10);
        Lcd_Write_String(display1); 
        Lcd_Set_Cursor (2,14);
        __delay_us(10);
        Lcd_Write_String(display2);
        __delay_ms(10);
    }
    return;
} 
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    
    TRISA = 0;
    PORTA = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    
    TRISE = 0;
    PORTE = 0;
    
    PORTEbits.RE0 = 1;
    PORTEbits.RE1 = 1;
    PORTEbits.RE2 = 1;
    
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("ADC0   ADC1  CNT");
    

}