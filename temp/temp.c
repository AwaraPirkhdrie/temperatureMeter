﻿#include <avr/io.h>
#include <avr/interrupt.h>
#include "temp.h"

/*	For storage of ADC value from temperature sensor.
	Initial value is good to use before A/D conversion is configured!	*/
static volatile uint16_t adc = 221;

/*
 * Interrupt Service Routine for the ADC.
 * The ISR will execute when a A/D conversion is complete.
 */
ISR(ADC_vect)
{
	// read ADC value
	// UPPGIFT: Läs in ADC-värdet. Börja med att läsa av det "låga" registret, därefter det "höga" registret!
	unsigned char low,high;
	unsigned int value;
	
	low = ADCL; // read ADC value (low byte first!)
	high = ADCH; // read ADC value high byte
	value = (high << 8) + low;
	adc = value;
}

/*
 * Initialize the ADC and ISR.
 */
void temp_init(void)
{
	// UPPGIFT: konfigurera ADC-enheten genom ställa in ADMUX och ADCSRA enligt kommentarerna nedanför!
	//ADMUX |= (1<< REFS1);    // set reference voltage (internal 5V)
	ADMUX |= (1<< REFS0);    // set reference voltage (internal 5V)

	
	ADMUX |= (1<< MUX0);		// RÄTT?? // select diff.amp 10x on ADC0 & ADC1
	ADMUX |= (1<< MUX3);		// right adjustment of ADC value
	
	ADCSRA |= (1<<ADPS2);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS0);		// prescaler 128
	
	ADCSRA |= (1<<ADATE);		// enable Auto Trigger
	ADCSRA |= (1<<ADIE);		// enable Interrupt
	ADCSRA |= (1<<ADEN);		// enable ADC

	// disable digital input on ADC0 and ADC1
	DIDR0 = 1;
		
	// disable USB controller (to make interrupts possible)
	USBCON = 0;	
	// enable global interrupts
	sei();

	// start initial conversion
	ADCSRA |= (1<<ADSC);	// UPPGIFT: gör så att den initiala A/D-omvandlingen sker
}

/*
 * Returns the temperature in Celsius.
 */
uint8_t temp_read_celsius(void)
{
	uint16_t adc_correction = adc * 98;
	uint16_t temp = adc_correction / 1000;
	// round up?
	if ((adc_correction % 1000) >= 500) {
		temp++;
	}
	return (uint8_t) temp;
}

/*
 * Returns the temperature in Fahrenheit.
 */
uint8_t temp_read_fahrenheit(void)
{
	uint16_t convert = ((temp_read_celsius() * 90) / 5) + 320;
	uint16_t temp = convert / 10;
	// round up?
	if ((convert % 10) >= 5) {
		temp++;
	}
	return (uint8_t) temp;
}