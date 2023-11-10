
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
#define Button1 PD1
#define Button2 PB1

static unsigned char currentLED = 0;
static unsigned char isButtonPressed = 0;

unsigned char debounce(unsigned char button)
{
  static unsigned char buttonState = 0;

  if ((PIND & (1 << button)))
  {
    if (buttonState < 50)
    {
      buttonState++;
    }
    if (buttonState <= 15)
    {
      // Tastendruck erkannt, Zustand zurücksetzen
      buttonState = 0;
      return 1;
    }
  }
  else
  {
    if (buttonState > 0)
    {
      buttonState = 0;
      return 0;
    }
  }
  return 0;
}



void initLEDs(void)
{
  // Konfiguration LEDs als Output
  DDRC |= ((1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5));
  DDRD |= ((1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
  DDRB |= (1 << PB0);
}
void initButtons(void)
{
  DDRD &= ~(1 << Button1);
  PORTD |= (1 << Button1);

  DDRB &= ~(1 << Button2);
  PORTB |= (1 << Button2);
}

void init_TIMER(void)
{

  // Timer Mask -> sagt Interrupt soll gesetzt werden soll, sobald Zähler oben
  TIMSK1 |= (1 << OCIE1A); // enable T1 Match Interrupt

  // Hier CTC Mode eingeschaltet _> Max kann selbst gesetzt werden
  TCCR1B |= (1 << WGM12); // -> im CTC Mode kann man selbst das Max festlegen -> 0 1 0

  // OCR Wert (Vergleichswert) gesetzt -> bis dieses Max wird gezählt
  OCR1A = 1562; // interrupt alle 0,1 sek

  // Prescaler -> auf 1 0 1 die Taktfrequenz von 16Mhz auf 15625Hz (16Mio / 1024) (infos aus Datenblatt)
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B |= (1 << CS10);
}

void turnOFF(void)
{
  PORTC &= ~((1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5));
  PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
  PORTB &= ~(1 << PB0);
}

void turnON(void)
{

  switch (currentLED)
  {
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
  default:
    break;
  }
}

ISR(TIMER1_COMPA_vect)
{
  turnOFF();
  if (debounce(Button1) && !isButtonPressed)
  {
    isButtonPressed = 1; // Markieren Sie den Button als gedrückt
    currentLED++;
  }
  
  if (!debounce(Button1))
  {
    isButtonPressed = 0; // Markieren Sie den Button als nicht gedrückt
  }

  if (currentLED > 10)
  {
    currentLED = 1;
  }

  turnON();
}


int main()
{
  initLEDs();
  initButtons();
  init_TIMER();
  sei();

  while (1)
  {
  }
}
