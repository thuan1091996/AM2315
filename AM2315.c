/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 2/18/2019
Author  : 
Company : 
Comments: 


Chip type               : ATmega8A
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega8.h>
#include <i2c.h>
#include <alcd.h>           // Alphanumeric LCD functions
#include <stdio.h>
#include <delay.h>

#define F_CPU               8000000
#define AM2315_ADDR         0x5C
unsigned char Receive[20]={0};
unsigned char *pointer_recv=&Receive[0];
/*----------------Global variable definition------------*/
//unsigned int    T_delay=0;                 //Tang 1 moi khi ngat timer0 (100us)
/*----------------Global function declaration-----------*/
void GPIO_Init();                   // GPIO initialization   
//void Timer0_Init();                 // Timer0 initialization
//void Delay_100us(unsigned int Time);// Delay 100us timer0
char Convert_2Char(unsigned char numb_in);
int Read_AM2315(void);
/*-----------------------Main---------------------------*/
void main(void)
{
    GPIO_Init();              // GPIO initialization
//    Timer0_Init();          // Timer0 initialization 
    i2c_init();
//    twi_master_init(50);    // TWI initialization
    lcd_init(16);
    PORTC|=0x01;    //Off LED
    #asm("sei")
    while (1)
    {       
        lcd_gotoxy(0,1);
        lcd_puts("AM2315");
//        if(Read_AM2315()==1)
//        {
//            PORTC&=~0x01;    ON LED
//            delay_ms(00);
//            PORTC|=0x01;    Off LED   
//        } 
        Read_AM2315();
        delay_ms(2000);
    }
}
//
//void Timer0_Init()
//{
//// Timer/Counter 0 initialization
//// Clock source: System Clock
//// Clock value: 8000.000 kHz
//// Prescaler: 8 
//    TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);  //Prescaler - 8
//    TCNT0=0x9C;                               // 100us
//}

//void Delay_100us(unsigned int Time) 
//{
///* Create delay function with 100us corresponding to each value */
//    TCNT0=0x9D;         //Tinh toan = 0x9D, nhung bu sai so nen A0
//    TIMSK|=0x01;        //Cho phep ngat tran timer0
//    T_delay=0;          //Reset gia tri dem
//    while(T_delay<Time);//Chua du
//    TIMSK&=~0x01;       //Du thoi gian, tat ngat tran timer0
//};

void GPIO_Init(void){
// Input/Output Ports initialization
// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=Out Bit1=In Bit0=In 
    DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (1<<DDB2) | (0<<DDB1) | (0<<DDB0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

    // Port C initialization
    // Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (1<<DDC0);
    // State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

    // Port D initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);
}

char Convert_2Char(unsigned char numb_in){
    char char_out;
    if(numb_in<0x0A)  char_out=numb_in+0x30; //0-9 return '0'-'9'
    else              char_out=numb_in+55;   //0x0A-0x0F return 'A'-'F'
    return char_out;
}

int Read_AM2315(void)
{
    unsigned char i=0;
    static unsigned char finish=0;
    //Wake up sensor (write value) Same
    i2c_start();
    i2c_write(0xb8);
    delay_ms(2);
    i2c_stop();     
       
    //Write command to read value     same
    i2c_start();
    i2c_write(0xB8);
    i2c_write(0x03);
    i2c_write(0x00);
    i2c_write(0x04);
    i2c_stop(); 
    
    delay_ms(10); 
    
    //Read value
    i2c_start();
    i2c_write(0xb9);
    for(i=0;i<8;i++)
    {
        Receive[i]=i2c_read(1); 
    }  
    i2c_stop();
    ///////////////////////////////
    if (Receive[0]!=3) return 0;
    if (Receive[1]!=4) return 0;
    {
        for(i=0;i<8;i++)
        {                                       
            Receive[i]=Convert_2Char(Receive[i]);
        }     
        lcd_gotoxy(0,0); 
        lcd_puts(pointer_recv);
        lcd_gotoxy(15,1);
        lcd_putchar(finish+'0');
        finish++; 
        if(finish>=10) PORTC&=~0x01;
        else PORTC|=0x01; 
        if (finish>=100) finish=0;
    }
    return 1;     
}

//interrupt [TIM0_OVF] void timer0_ovf_isr(void)
//{
//    // Reinitialize Timer 0 value
//    T_delay++;
//    TCNT0=0x9C;    //Tinh toan = 0x9C (100), nhung bu sai so nen A0 
//};