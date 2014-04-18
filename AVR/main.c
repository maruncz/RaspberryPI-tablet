#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#define F_CPU 14745600UL

uint8_t i,inter;
char zap,ac,batt,lcd_en;


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
uint16_t mer(uint8_t channel)
{
    uint8_t a,b;
    uint16_t vys;
    ADMUX=0b01000000|channel;
    ADCSRA=(ADCSRA|(1<<ADSC));
    do{}
    while(ADCSRA&(1<<ADSC));
    b=ADCL;
    a=ADCH;
    vys=a;
    vys=vys*256;
    vys=vys+b;
    return vys;
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
void zel(char a)
{
    if(a==0)
    {
        PORTA=PORTA&~(1<<PORT1);
    }
    else
    {
        PORTA=PORTA|(1<<PORT1);
    }
}
void cerv(char a)
{
    if(a==0)
    {
        PORTA=PORTA&~(1<<PORT6);
    }
    else
    {
        PORTA=PORTA|(1<<PORT6);
    }
}
void vypni(void)
{
    zap=0;
    cerv(1);
    unsigned char i;
    for(i=0;i<255;i++)
    {
        wdt_reset();
        _delay_ms(60);
    }
}

void rpi(uint8_t a)
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
void interrupt(uint8_t i)
{
    inter=i;
    PORTD=PORTD|(1<<PORT2);
    _delay_ms(1);
    PORTD=PORTD&~(1<<PORT2);
}
void lcd(uint8_t a)
{
    if(a!=0)
    {
        PORTC=PORTC&~(1<<PORT5);
        lcd_en=1;
        _delay_ms(50);
        cerv(0);
    }
    else
    {
        PORTC=PORTC|(1<<PORT5);
        lcd_en=0;
        interrupt(1);
        _delay_ms(50);
        cerv(1);
    }
}

char test_nap(void)
{
    ac=(530<=mer(3));
    batt=(300<=mer(4));
    return (batt||ac);
}

void nap_vyber(void)
{
    if((!ac)&&(batt))
    {
        PORTC=PORTC|(1<<PORT3);
    }
    else
    {
        PORTC=PORTC&~(1<<PORT3);
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
    PORTD=0;
    TCCR0=0b01101101;
    OCR0=128;
    SPCR=(1<<SPE);
    ADMUX=((0<<REFS1)|(1<<REFS0)|(0b00000000));
    ADCSRA=((1<<ADEN)|(0b00000111));
    wdt_reset();
    wdt_enable(WDTO_2S);
    zap=0;
    ac=0;
    batt=0;
    lcd_en=0;
    PORTC=PORTC|((1<<PORT4)|(1<<PORT5));

    while(1)
    {
        if(zap==0)
        {
            PORTC=PORTC&~(1<<PORT3);
            rpi(0);
            lcd(0);
            cerv(0);
            zel(0);
        };
        wdt_reset();
        if((PINC&(1<<PIN2))&&(zap==1))
        {
            lcd(!lcd_en);
            do{wdt_reset();}
            while(PINC&(1<<PIN2));
        }
        if((PINC&(1<<PIN2))&&(zap==0))
        {
            uint8_t i;
            char b;
            b=test_nap();
            for(i=0;((i<51)&&(b==1));i++)
            {
                _delay_ms(18);
                if(!(PINC&(1<<PIN2)))
                {
                    b=0;
                    i=200;
                    wdt_reset();
                }
            };

            if(b==1)
            {
                zap=1;
                nap_vyber();
                cerv(0);
                zel(1);
                rpi(1);
                lcd(1);
            }
            else
            {
                zel(0);
                cerv(1);
            }
            do{wdt_reset();}
            while(PINC&(1<<PIN2));
        };
        test_nap();
        nap_vyber();
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
            case 0x45:
                switch(inter)
                {
                case 1:
                    SPDR=0x40;
                    spi_prijem();
                break;
            default:
                    SPDR=0x45;
                    spi_prijem();
                break;
                }
            };
        };


    };

    return 0;
}


