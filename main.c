/*
 * ATTiny10SpeakerModule.c
 *
 * Created: 31.07.2017 11:14:31
 * Author : Tomasz Jaworski
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
// ------------------------------------------------------------

void cpu_init(void);

// ------------------------------------------------------------

volatile uint16_t sound_register;
volatile uint16_t shadow_sound_register;
volatile uint8_t shadow_sound_register_shift_counter;
volatile uint8_t idle_counter;

#define TIMER_ISR_ENABLE	TIMSK0 |= (1<<OCIE0A);
#define TIMER_ISR_DISABLE	TIMSK0 = 0x00;

int main(void)
{
	cpu_init();
    
	while(1)
	{
		// wait approx. 100us
		for (int i = 0; i < 159; i++)
			asm("nop");

		// try to resynchronize			
		if (idle_counter++ > 10)
		{
			idle_counter = 0;
			shadow_sound_register_shift_counter = 0;
		}
		
	}
	
}

void cpu_init(void)
{
	// temporarily remove protected register write lock
	CCP = 0xD8;
	
	// and set prescaler to 0 -> full 8MHz of raw speed :)
	CLKPSR = 0;
	OSCCAL = 145;

	// 1 - PB0 (PCINT0/TPIDATA/OC0A/ADC0/AIN0)				INPUT, SOUND SERIAL DATA		INPUT, SOUND SERIAL DATA
	// 2 - GND												
	// 3 - PB1 (PCINT1/TPICLK/CLKI/ICP0/OC0B/ADC1/AIN1)		INPUT, SOUND SERIAL CLOCK		OUTPUT, SPEAKER
	// 4 - PB2 (T0/CLKO/PCINT2/INT0/ADC2)					OUTPUT, SPEAKER					INPUT, SOUND SERIAL CLOCK
	// 5 - VCC
	// 6 - PB3 (RESET/PCINT3/ADC3)							INPUT, RESET
	
	//
	// RESET
	//	PB3		vcc		PB2
	//	6		5		4
	//	
	//	1		2		3
	//	PB0		gnd		PB1
	
	DDRB = 0b00000010;
	PORTB = 0b0000000; // turn off output transistor
	PUEB = 0b00000000; // setup pullups

	// init timer
	TCCR0A = 0;
	TCCR0B = (1<<CS01)|(1 << WGM02); // CTC (Clear Timer on Compare)
	OCR0A = 800;
	TIMER_ISR_DISABLE;
	
	// configure INT0 to rising edge
	EICRA = 0b00000011;
	EIMSK = 0b00000001;
	
	sound_register = 0;
	shadow_sound_register = 0;
	shadow_sound_register_shift_counter = 0;
	
	// start interrupts
	sei();
}

// interrupt on INT0 on rising edge
ISR(INT0_vect)
{
	// write data from input pin and shift
	shadow_sound_register |= PINB & 0b00000001;
	shadow_sound_register <<= 1;
	shadow_sound_register_shift_counter++;
	idle_counter = 0;

	if (shadow_sound_register_shift_counter >= 16)
	{
		sound_register = shadow_sound_register;
		shadow_sound_register_shift_counter = 0;
		
		// turn off sound?
		if (sound_register == 0)
		{
			// turn off timer's interrupt and transistor
			TIMER_ISR_DISABLE;
			PORTB &= ~_BV(PORTB1);
			return;
		}
		
		OCR0A = sound_register;
		TIMER_ISR_ENABLE;
	}
	
}

ISR(TIM0_COMPA_vect)
{
	PINB |= _BV(PINB1);
}
