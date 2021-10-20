/*
 * main.c
 *
 *  Created on: 20.10.2021
 *      Author: ag4716
 */



//#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void setup() ;
void loop() ;
ISR (SPI_STC_vect) ;
void ws2812_sendarray_16Mhz(uint8_t *data,uint16_t n) ;

int main(void){
	setup();
	while(1)
		loop();
	return 0;
}


#define	MAX_OUTPUT_LEDS			500
#define LED_PIN					PD4
#define CS						PB2
#define MOSI					PB3
#define MISO					PB4
//necessary for Assembler routine. Update if changing WS2812 Pin!
#define ws2812_port 0x0B  // Number of the data port register
#define ws2812_pin 	PD4    // Number of the data out pin



volatile uint8_t ledData[3*MAX_OUTPUT_LEDS];
volatile uint16_t ledDataPos;
uint8_t transmit;
uint8_t CSBuffer = 0xFF;
uint16_t lastLedDataPos = 0x0000;


void ws2812_sendarray_16Mhz(uint8_t *data,uint16_t n);


//The setup function is called once at startup of the sketch
void setup()
{
//	uint8_t resetCause = MCUSR&0b00001010;
	DDRD |= (1<< LED_PIN);
	DDRB |= (1<< MISO);
	DDRB &= ~(1<<MOSI);
	PORTB |= (1<<MOSI);

	for(uint16_t i = 0; i < MAX_OUTPUT_LEDS; i+=3){
		ledData[i] = 0;
		ledData[i+1] = 0;
		ledData[i+2] = 0;
	}
	ledDataPos = 0;
	transmit = 0x00;

	//transmit 0 to every LED when turning on
	ws2812_sendarray_16Mhz(ledData, MAX_OUTPUT_LEDS*3);
	for(int i = 0; i<1000;i++)
		asm volatile("nop\n\t");

//	if(resetCause){
////		ledData[resetCause*3+1] = 0xFF;
//		//watchdog -> blue light
//		if(resetCause &(1<<3))
//			ledData[3*3+2] = 0xFF;
//		//brownout -> green light
//		if(resetCause &(1<<2))
//			ledData[2*3+0] = 0xFF;
//		//external -> red light
//		if(resetCause &(1<<1))
//			ledData[1*3+1] = 0xFF;
//		ws2812_sendarray_16Mhz(ledData, 30);	//transmit 0 to every LED when turning on
//		while(1);
//	}

	while((PINB&(1<<CS)) == 0);	//wait until transmission complete before enabling SPI

	// turn on SPI in slave mode
	SPCR |= _BV(SPE);

	// turn on interrupts
	SPCR |= _BV(SPIE);
}

void loop()
{
	if(transmit){
		uint16_t dataLen = ledDataPos;
		ledDataPos = 0;
		transmit = 0x00;
		lastLedDataPos = 0;
		ws2812_sendarray_16Mhz(ledData, dataLen);
	}

	//check if new data has been received
	uint16_t tempPos = ledDataPos;
	if(tempPos > lastLedDataPos)
		CSBuffer = 0x00;
	lastLedDataPos = tempPos;

	//check if transmission ended
	CSBuffer<<=1;
	if(PINB & (1<<CS))
		CSBuffer |= 0x01;
	if(lastLedDataPos >= 1 && CSBuffer == 0xFF){ //rising CS = transmission complete
		transmit = 0xFF;
	}
	if(ledDataPos > MAX_OUTPUT_LEDS*3){
		ledDataPos = 0;
	}
}

ISR (SPI_STC_vect)
{
	ledData[ledDataPos] = SPDR;
	ledDataPos++;
}


//Pro LED 3 Byte daten: (G8:R8:B8)
void ws2812_sendarray_16Mhz(uint8_t *data,uint16_t n)
{
  uint8_t curbyte,ctr;
  uint8_t sreg_on = SREG;
	  cli();

  while (n--) {
    curbyte=*data++;
    sreg_on = SREG;
//    if(ws2812_transmission_corrupted){
//    	return;
//    }
//    cli();

    asm volatile(
        "    ldi  %0,8       \n\t"    // 0
        "    nop\n\t"

        "loop%=:lsl  %1      \n\t"    //  1     Datenbit ins Carry
//    	"	cli\n\t"	//added
        "    dec  %0         \n\t"    //  2     Schleifenzähler in Z-Flag
        "    sbi  %2,  %3    \n\t"    //  4

        "    nop\n\t"                  //  5
        "    nop\n\t"                  //  6

        "    brcs .+2        \n\t"    //  7l / 8h
        "    cbi  %2,  %3    \n\t"    //  9l / -    Überspringen bei C=1

        "    nop\n\t"                  // 10l /  9h
        "    nop\n\t"                  // 11l / 10h
        "    nop\n\t"                  // 12l / 11h
        "    nop\n\t"                  // 13l / 12h

        "    brcc .+2        \n\t"    // 15l / 13h
        "    cbi  %2,  %3    \n\t"    // -  / 15h   Überspringen bei C=0

//    	"	sei\n\t"	//added
        "    nop\n\t"                  // 16
        "    nop\n\t"                  // 17
        "    nop\n\t"                  // 18
        "    nop\n\t"                  // 19

        "    brne loop%=     \n\t"    // 11 loop /10nt  Weiter, wenn Z=1

    :  "=&d" (ctr)
    :  "r" (curbyte), "I" (ws2812_port), "I" (ws2812_pin)
    );
    SREG=sreg_on;

    // loop overhead including byte load is 6+1 cycles
  }
  sei();
}
