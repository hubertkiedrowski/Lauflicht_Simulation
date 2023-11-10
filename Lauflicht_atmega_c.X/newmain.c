
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define LED1 PC1
#define LED2 PC2
#define LED3 PC3
#define LED4 PC4
#define LED5 PC5
#define LED6 PD4
#define LED7 PD5
#define LED8 PD6
#define LED9 PD7
#define LED10 PB0

static unsigned char currentLED = 0;

void init(void){
    // Konfiguration LEDs als Output
    DDRC |= ((1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5));
    DDRD |= ((1 << PD4) | (1 << PD5) | (1 << PD6) | ( 1 << PD7));
    DDRB |= (1 << PB0);
   
}

void init_TIMER(void){
    
    // Timer Mask -> sagt Interrupt soll gesetzt werden soll, sobald Zähler oben
    TIMSK1 |= (1 << OCIE1A); // enable T1 Match Interrupt
    
    // Hier CTC Mode eingeschaltet _> Max kann selbst gesetzt werden
    TCCR1B |= (1 << WGM12); // -> im CTC Mode kann man selbst das Max festlegen -> 0 1 0
    
    // OCR Wert (Vergleichswert) gesetzt -> bis dieses Max wird gezählt
    OCR1A = 15625; // --> Top wird mit OCR register gesetzt hier nach 15625Hz (Timer Match nach 1sec))
    
    // Prescaler -> auf 1 0 1 die Taktfrequenz von 16Mhz auf 15625Hz (16Mio / 1024) (infos aus Datenblatt)
    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B |= (1 << CS10);
}

void turnOFF(void){
    PORTC &= ~((1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5));
    PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6) | ( 1 << PD7));
    PORTB &= ~(1 << PB0);
}

void turnON(void){

    switch(currentLED){
        case 1:
            PORTC |= (1 << LED1);
            break;
        case 2:
            PORTC |= (1 << LED2);
            break;
        case 3:
            PORTC |= (1 << LED3);
            break;
        case 4:
            PORTC |= (1 << LED4);
            break;
        case 5:
            PORTC |= (1 << LED5);
            break;
        case 6:
            PORTD |= (1 << LED6);
            break;
        case 7:
            PORTD |= (1 << LED7);
            break;
        case 8:
            PORTD |= (1 << LED8);
            break;
        case 9:
            PORTD |= (1 << LED9);
            break;
        case 10:
            PORTB |= (1 << LED10);
            break;
    }
    
}

ISR(TIMER1_COMPA_vect){
    turnOFF();
    
    currentLED++;
    
    if(currentLED > 10){
        currentLED = 1;
    }
    
    turnON();
    
}

int main() {
    init();
    init_TIMER();
    sei();
           
    while(1){
       
    }
}
