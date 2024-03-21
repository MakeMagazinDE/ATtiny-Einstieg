/*
* Florian Schäffer
* https://www.blafusel.de
*
* 1/2024
* ATtiny Demo mit DAC
*/

#define F_CPU 3333333UL		// 3,3 MHz (OSCCFG = 20 Mhz, MCLKCTRLB = 0x8 => Prescaler 6)

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	PORTA.DIRSET = (1 << 3);					// Pin 3 im Richtungsregister auf 1 = Output 
	PORTA.DIRCLR = (1 << 2);					// Pin 2 auf 1 = als Eingang
	PORTA.PIN2CTRL = PORT_PULLUPEN_bm;			// Pullup für Pin 2 von Port A aktivieren; Bit 3 altern.: (1<<3)  
	
	VREF.CTRLA |= VREF_DAC0REFSEL_4V34_gc;       // Voltage reference auf 4.34V
	_delay_us(25);	// 25 µs warten lt. Datenblatt
	// The DAC output pin needs to have the digital input buffer and the pull-up resistor disabled to reduce its load:
	PORTA.PIN6CTRL &= ~PORT_ISC_gm;
	PORTA.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTA.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm | DAC_RUNSTDBY_bm;		// Enabling DAC, Output Buffer (sends the OUT signal to a pin) and Run in Standby sleep mode
	
	uint8_t DACwert = 0;

	while (1)
	{
		if (PORTA.IN & (1 << 2))		// PA2 High? = Taster offen
		{
			PORTA.OUTCLR |= (1<<3);		// PB3 LED LOW (aus)
			_delay_ms(150);
		}
		else
		{
			PORTA.OUTSET |= (1<<3);		// PB3 LED HIGH (ein)
			DACwert += 5;							// Variable um 5 erhöhen
			DAC0.DATA = DACwert;			// DAC-Wert ausgeben
			_delay_ms(50);
			PORTA.OUTCLR |= (1<<3);		// PB3 LED LOW
			_delay_ms(50);
		}
	}
}

