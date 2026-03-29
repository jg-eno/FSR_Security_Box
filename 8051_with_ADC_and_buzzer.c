#include <reg51.h>

#define ADC_DATA P1
#define LCD_DATA P3

sbit rd   = P2^5;
sbit wr   = P2^6;
sbit intr = P2^7;

sbit RS = P2^0;
sbit EN = P2^1;
sbit BUZZER = P2^2;

void delay(int n)
{
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<255;j++);
}

void lcd_cmd(unsigned char cmd)
{
    LCD_DATA = cmd;
    RS = 0;
    EN = 1;
    delay(2);
    EN = 0;
}

void lcd_data(unsigned char d)
{
    LCD_DATA = d;
    RS = 1;
    EN = 1;
    delay(2);
    EN = 0;
}

void lcd_init()
{
    lcd_cmd(0x38);
    lcd_cmd(0x0E);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

void display(char *s)
{
    while(*s)
        lcd_data(*s++);
}

void print_num(unsigned char val)
{
    lcd_data((val/100)+'0');
    lcd_data(((val/10)%10)+'0');
    lcd_data((val%10)+'0');
}

unsigned char read_adc()
{
    unsigned char val;

    wr = 0;
    delay(1);
    wr = 1;

    while(intr);

    rd = 0;
    delay(1);
    val = ADC_DATA;
    rd = 1;

    return val;
}

void beep()
{
    int i;
    for(i = 0; i < 150; i++)
    {
        BUZZER = 1;
        delay(1);
        BUZZER = 0;
        delay(1);
    }
}

void main()
{
    unsigned char value;

    ADC_DATA = 0xFF;

    rd = 1;
    wr = 1;
    intr = 1;
    BUZZER = 0;

    lcd_init();

    while(1)
    {
        value = read_adc();

        lcd_cmd(0x80);
        display("ADC:");
        print_num(value);

        lcd_cmd(0xC0);
        if(value >= 200)
        {
            display("THEFT ");
            beep();
        }
        else
        {
            display("SAFE  ");
            BUZZER = 0;
        }

        delay(100);
    }
}
