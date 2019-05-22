#include <avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
void initADC(void);
uint16_t ReadADC(uint8_t );


volatile uint16_t sensor=0,n=120, flag1=0,flag2=0;

 int main()
{

   DDRD&=~(1<<PD2);// interrupt purpose for person seating
   DDRD&=~(1<<PD3);// interrupt purpose for seat belt
   PORTD|=(1<<PD2);// for 5v D2
   PORTD|=(1<<PD3);// for 5v D3



   EICRA|=(1<<ISC00);//for any logic changes in INT0 for seat sensor
   EICRA|=(1<<ISC10);//for any logic changes in INT1 seat_belt
   EIMSK|=(1<<INT0);
   EIMSK|=(1<<INT1);
   sei();


    DDRB|=(1<<PB3);//out one uncontrolled speed
     DDRB|=(1<<PB4);//out two normal speed
     DDRC&=~(1<<PC2);//adc read1 for speed  monitor(input)




  initADC();
    while(1)
          {
            if((flag1==1)&&(flag2==1))
          {
     sensor=ReadADC(2);
       _delay_ms(200);

               //_delay_ms(100);
              if(sensor>=n)
               {
                   PORTB|=(1<<PB3);//uncontrolled
                   PORTB&=~(1<<PB4);
                  _delay_ms(100);
                }
               else if(sensor<=n)
               {
                       PORTB&=~(1<<PB3);
                       PORTB|=(1<<PB4);
                        _delay_ms(200);
                       }//normal

          else
          {
               PORTB&=~(1<<PB3);
                PORTB&=~(1<<PB4);
          }}
           else
          {
               PORTB&=~(1<<PB3);
                PORTB&=~(1<<PB4);
                }}
    return 0;
}

void initADC()
{
    ADMUX=(1 << REFS0);
    ADCSRA=(1 << ADEN)|(7 << ADPS0);

}
uint16_t ReadADC(uint8_t ch)
{
    ADMUX&=0xf8;
    ch=ch&0b00000111;
    ADMUX |=ch;
    ADCSRA|=(1 << ADSC);
    while(!(ADCSRA& (1 << ADIF)));
    ADCSRA |=(1 << ADIF);
    return(ADC);
}

ISR(INT0_vect)
    {
        flag1=1;
    }
ISR(INT1_vect)
    {
        flag2=1;
    }
