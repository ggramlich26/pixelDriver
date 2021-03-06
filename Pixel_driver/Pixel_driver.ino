#include "Arduino.h"

//removed includes : Pixel_driver/core/core      Pixel_driver/core/variant




//#define	MAX_OUTPUT_LEDS			500
//#define LED_PIN					PD4
//#define CS						PB2
//#define MOSI					PB3
//#define MISO					PB4
////necessary for Assembler routine. Update if changing WS2812 Pin!
//#define ws2812_port 0x0B  // Number of the data port register
//#define ws2812_pin 	PD4    // Number of the data out pin
//
//
//
//uint8_t ledData[3*MAX_OUTPUT_LEDS];
//uint16_t ledDataPos;
//uint8_t transmit;
//
//
//void ws2812_sendarray_16Mhz(uint8_t *data,uint16_t n);
//
//
////The setup function is called once at startup of the sketch
//void setup()
//{
////	pinMode(LED_PIN,OUTPUT);
//	DDRD |= (1<< LED_PIN);
//	for(uint16_t i = 0; i < MAX_OUTPUT_LEDS; i+=3){
//		ledData[i] = 0;
//		ledData[i+1] = 0;
//		ledData[i+2] = 0;
//	}
//	ledDataPos = MAX_OUTPUT_LEDS*3-1;	//transmit 0 to every LED when turning on
//	transmit = 0xFF;
//	// have to send on master in, *slave out*
////	pinMode(MISO, OUTPUT);
////	pinMode(MOSI, INPUT_PULLUP);
//	DDRB |= (1<< MISO);
//	DDRB &= ~(1<<MOSI);
//	PORTB |= (1<<MOSI);
//
//	// turn on SPI in slave mode
//	SPCR |= _BV(SPE);
//
//	// turn on interrupts
//	SPCR |= _BV(SPIE);
//}
//
//void loop()
//{
//	if(transmit){
//		ws2812_sendarray_16Mhz(ledData, ledDataPos);
//		ledDataPos = 0;
//		transmit = 0x00;
//	}
//	static uint8_t lastCSState = 0xFF;
////	bool CSState = digitalRead(CS);
//	uint8_t CSState = PINB & (1<<CS);
//	if(!lastCSState && CSState){ //rising CS = transmission complete
//		transmit = 0xFF;
//	}
//	lastCSState = CSState;
//	if(ledDataPos > MAX_OUTPUT_LEDS*3){
//		ledDataPos = 0;
//	}
//}
//
//ISR (SPI_STC_vect)
//{
////	if(ledDataPos >= MAX_OUTPUT_LEDS*3)
////		return;
//
//	ledData[ledDataPos] = SPDR;
//	ledDataPos++;
//}
//
//
////Pro LED 3 Byte daten: (G8:R8:B8)
//void ws2812_sendarray_16Mhz(uint8_t *data,uint16_t n)
//{
//  uint8_t curbyte,ctr;
//  uint8_t sreg_on = SREG;
//	  cli();
//
//  while (n--) {
//    curbyte=*data++;
//    sreg_on = SREG;
////    if(ws2812_transmission_corrupted){
////    	return;
////    }
////    cli();
//
//    asm volatile(
//        "    ldi  %0,8       \n\t"    // 0
//        "    nop\n\t"
//
//        "loop%=:lsl  %1      \n\t"    //  1     Datenbit ins Carry
////    	"	cli\n\t"	//added
//        "    dec  %0         \n\t"    //  2     Schleifenz?hler in Z-Flag
//        "    sbi  %2,  %3    \n\t"    //  4
//
//        "    nop\n\t"                  //  5
//        "    nop\n\t"                  //  6
//
//        "    brcs .+2        \n\t"    //  7l / 8h
//        "    cbi  %2,  %3    \n\t"    //  9l / -    ?berspringen bei C=1
//
//        "    nop\n\t"                  // 10l /  9h
//        "    nop\n\t"                  // 11l / 10h
//        "    nop\n\t"                  // 12l / 11h
//        "    nop\n\t"                  // 13l / 12h
//
//        "    brcc .+2        \n\t"    // 15l / 13h
//        "    cbi  %2,  %3    \n\t"    // -  / 15h   ?berspringen bei C=0
//
////    	"	sei\n\t"	//added
//        "    nop\n\t"                  // 16
//        "    nop\n\t"                  // 17
//        "    nop\n\t"                  // 18
//        "    nop\n\t"                  // 19
//
//        "    brne loop%=     \n\t"    // 11 loop /10nt  Weiter, wenn Z=1
//
//    :  "=&d" (ctr)
//    :  "r" (curbyte), "I" (ws2812_port), "I" (ws2812_pin)
//    );
//    SREG=sreg_on;
//
//    // loop overhead including byte load is 6+1 cycles
//  }
//  sei();
//}
