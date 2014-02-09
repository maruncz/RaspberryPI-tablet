#include <avr/io.h>
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
void adc_mereni(void)
{
    ADCSRA=(ADCSRA|(1<<ADSC));
    do{}
    while(!(ADCSRA&(1<<ADSC)));
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
    SPDR='z';
    ADMUX=((0<<REFS1)|(1<<REFS0)|(0b00000000));
    ADCSRA=((1<<ADEN)|(0b00000111));
    char i;

    while(1)
    {
        /*if (UCSRA & (1<<RXC))
        {
            i=UDR;
            UDR=i;
            uart_cek();
        };*/
        if (SPSR & (1<<SPIF))
        {
            i=SPDR;
            if(i==0x40)
            {
                spi_prijem();
                i=SPDR;
                OCR0=i;
            };
            if(i==0x41)
            {
                i=SPDR;
                adc_mereni();
                SPDR=ADCH+168;
                spi_prijem();
                i=SPDR;
                SPDR=ADCL;
                spi_prijem();
                i=SPDR;
                i=(ADCH+168)^ADCL;
                SPDR=i;
                spi_prijem();
                i=SPDR;
            };
        };


    };

    return 0;
}


