#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

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
void adc(char channel)
{
    char i;
    static char vysledek[2];
    ADCSRA=(ADCSRA|(1<<ADSC));
    do{}
    while(!(ADCSRA&(1<<ADSC)));
    vysledek[0] = ADCL;
    vysledek[1] = ADCH;
    SPDR=(vysledek[1])+168;
    spi_prijem();
    i=SPDR;
    UDR=5;
    uart_cek();
    SPDR=vysledek[0];
    spi_prijem();
    i=SPDR;
    UDR=6;
    uart_cek();
    i=(ADCH+168)^ADCL;
    SPDR=i;
    UDR=7;
    uart_cek();
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
    PORTB=0b00000000;
    TCCR0=0b01101001;
    OCR0=128;
    SPCR=(1<<SPE);
    ADMUX=((0<<REFS1)|(1<<REFS0)|(0b00000000));
    ADCSRA=((1<<ADEN)|(0b00000111));
    wdt_enable(WDTO_1S);
    char i;

    while(1)
    {
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
            UDR=i;
            uart_cek();
            switch(i)
            {
            case 0x40:
                spi_prijem();
                i=SPDR;
                OCR0=i;
                UDR=2;
                UDR=i;
                break;
            case 0x41:
                adc(0);
                UDR=3;
                uart_cek();
                break;
            case 0x42:
                adc(1);
                UDR=4;
                uart_cek();
                break;
            };
        };


    };

    return 0;
}


