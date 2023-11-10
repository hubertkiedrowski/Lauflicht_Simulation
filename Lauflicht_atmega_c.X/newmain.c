/*
 * File:   newmain.c
 * Author: elberto
 *
 * Created on October 30, 2023, 2:12 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define Gruen1 PC1
#define Gruen2 PC2
#define Gruen3 PC3
#define Gruen4 PC4
#define Gruen5 PC5
#define Gruen6 PD4
#define Gruen7 PD5
#define ROT1 PD6
#define ROT2 PD7
#define ROT3 PB0
#define BUTTON1 PD1
#define BUTTON2 PB1
setup()
{
  DDRC |= (1 << Gruen1) | (1 << Gruen2) | (1 << Gruen3) | (1 << Gruen4) | (1 << Gruen5);
  DDRD |= (1 << Gruen6) | (1 << Gruen7) | (1 << ROT1) | (1 << ROT2);
  DDRB |= (1 << ROT3);

  DDRD &= ~(1 << BUTTON1);
  DDRB &= ~(1 << BUTTON2);


  PCMSK0 |= (1 << PCINT1);
      PCICR |= (1 << PCIE0); // Aktivierung der Pin Change Interrupts für Gruppe 0
};

ISR(PCINT0_vect)
{
  static unsigned char i = 1;
  turnOffGreenLEDs();
  if (i % 2 == 0)
  {
    turnOnRedLEDs();
  }
  else
  {
    turnOffRedLEDs();
  }
  i++;
}

// Funktionen zur Steuerung der LEDs
void turnOnGreenLEDs()
{
  PORTC |= (1 << Gruen1) | (1 << Gruen2) | (1 << Gruen3) | (1 << Gruen4) | (1 << Gruen5);
  PORTD |= (1 << Gruen6) | (1 << Gruen7);
};

void turnOffGreenLEDs()
{
  PORTC &= ~((1 << Gruen1) | (1 << Gruen2) | (1 << Gruen3) | (1 << Gruen4) | (1 << Gruen5));
  PORTD &= ~((1 << Gruen6) | (1 << Gruen7));
};

void turnOnRedLEDs()
{
  PORTD |= (1 << ROT1) | (1 << ROT2);
  PORTB |= (1 << ROT3);
};

void turnOffRedLEDs()
{
  PORTD &= ~((1 << ROT1) | (1 << ROT2));
  PORTB &= ~(1 << ROT3);
};
int main()
{
  setup();
  sei();
  while (1)
  {
    if (!(PIND & (1 << Gruen1)))
    {
      turnOnGreenLEDs();
    }
  }
}

