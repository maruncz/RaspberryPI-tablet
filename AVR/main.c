#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#define F_CPU 14745600UL

char i;
char zap;

void uart_cek(void)
{
    do{}
    while(!(UCSRA&(1<<UDRE)));
}
void spi_prijem(void)
{
    do{}
    while (!(SPSR & (1<<SPIF)));
}
void adc(uint8_t channel)
{
    uint8_t a,b;
    ADMUX=0b01000000|channel;
    ADCSRA=(ADCSRA|(1<<ADSC));
    do{}
    while(ADCSRA&(1<<ADSC));
    b=ADCL;
    a=ADCH;
    a=a|168;
    SPDR=a;
    spi_prijem();
    i=SPDR;
    SPDR=b;
    spi_prijem();
    i=SPDR;
    i=a^b;
    SPDR=i;
    spi_prijem();
    i=SPDR;
}

void vypni(void)
{
    unsigned char i;
    for(i=0;i<255;i++)
    {
        wdt_reset();
        _delay_ms(60);
    }
    zap=0;
}

void rpi(char a)
{
    if(a!=0)
    {
        PORTC=PORTC&~(1<<PORT4);
    }
    else
    {
        PORTC=PORTC|(1<<PORT4);
    }
}

void lcd(char a)
{
    if(a!=0)
    {
        PORTC=PORTC&~(1<<PORT5);
    }
    else
    {
        PORTC=PORTC|(1<<PORT5);
    }
}

int main(void)
{
    DDRD=((1<<PORT2));
    DDRB=((1<<PORT3)|(1<<PORT6));
    DDRC=((1<<PORT5)|(1<<PORT4)|(1<<PORT3));
    DDRA=((1<<PORT1)|(1<<PORT6 ));
    PORTC=0;
    PORTB=0b00000000;
    TCCR0=0b01101101;
    OCR0=128;
    SPCR=(1<<SPE);
    ADMUX=((0<<REFS1)|(1<<REFS0)|(0b00000000));
    ADCSRA=((1<<ADEN)|(0b00000111));
    wdt_reset();
    wdt_enable(WDTO_2S);
    zap=0;
    PORTC=PORTC|((1<<PORT4)|(1<<PORT5));

    while(1)
    {
        if(zap==0)
        {
            PORTC=PORTC&~(1<<PORT3);
            rpi(0);
            lcd(0);
            PORTA=PORTA&~(1<<PORT1);
        };
        wdt_reset();
        if((PINC&(1<<PIN2))&&(zap==1))
        {
            lcd(1);
        }
        if((PINC&(1<<PIN2))&&(zap==0))
        {
            uint8_t i;
            char b=0;
            for(i=0;(i<51);i++)
            {
                _delay_ms(18);
                if(!(PINC&(1<<PIN2)))
                {
                    b=1;
                    i=200;

                }
            };

            if(b==0)
            {
                zap=1;
                PORTC=PORTC|(1<<PORT3);
                PORTA=PORTA|(1<<PORT1);
                PORTA=PORTA&~(1<<PORT6);
                rpi(1);
                lcd(1);
            }
            else
            {
                PORTA=PORTA|(1<<PORT6);
                PORTA=PORTA&~(1<<PORT1);
            }
        };
        wdt_reset();
        if (SPSR & (1<<SPIF))
        {
            i=SPDR;
            switch(i)
            {
            case 0x40:
                spi_prijem();
                i=SPDR;
                OCR0=i;
                break;
            case 0x41:
                adc(3);
                break;
            case 0x42:
                adc(4);
                break;
            case 0x43:
                vypni();
                break;
            case 0x44:
                lcd(0);
                break;
            };
        };


    };

    return 0;
}


