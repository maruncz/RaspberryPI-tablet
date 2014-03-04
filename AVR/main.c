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

int main(void)
{
    UCSRA=0b00000000;
    UCSRB=((1<<RXEN)|(1<<TXEN));
    UCSRC=((1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0));
    UBRRH=0x1;
    UBRRL=0x19;
    DDRD=0b01000000;
    DDRB=0b01001000;
    DDRC=0b00011100;
    DDRA=0b01000010;
    PORTC=0;
    PORTB=0b00000000;
    TCCR0=0b01101101;
    OCR0=128;
    SPCR=(1<<SPE);
    ADMUX=((0<<REFS1)|(1<<REFS0)|(0b00000000));
    ADCSRA=((1<<ADEN)|(0b00000111));
    wdt_enable(WDTO_1S);
    zap=0;

    while(1)
    {
        if((PINC&(1<<PIN2))&&(zap==0))
        {
            uint8_t i,j;
            char b=0;
            for(i=0;((i<50));i++)
            {
                j=i;
                _delay_ms(10);
                if(!(PINC&(1<<PIN2)))
                {
                    PORTA=PORTA|(1<<PORT6);
                    b=1;
                    i=200;

                }
            };

            if(b==0)
            {
                zap=1;
                PORTC=PORTC|(1<<PORT3);
                PORTA=PORTA|(1<<PORT1);
            };
        };


        if(UCSRA & (1<<RXC))
        {
            i=UDR;
            UDR=i;
            uart_cek();
        }
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
            };
        };


    };

    return 0;
}


