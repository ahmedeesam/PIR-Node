/*
 * PIR_updated.c
 *
 * Created: 06/02/2022 04:08:31 م
 * Author : ahmed essam
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ext_int.h"
#include "timer1.h" 
#include "uart.h"

char count;

ISR (INT1_vect)
{
	serial_debug("on\n"); //send on to esp (perform action)
	start_tim1_normal(); 
	int1_STOP(); 
}

ISR (TIMER1_OVF_vect)
{
	if ((PINB & (1<<PORTB0)) == 0) //if pir outputs low
	{
		count++; //increment counter
		serial_debug("low");
	} 
	else if ((PINB & (1<<PORTB0)) != 0) //if pir outputs High
	{
		count = 0; //restart counter
		serial_debug("high");
	}
	
	if (count == 3)
	{
		serial_debug("off\n"); //send off to esp (perform action)
		count = 0;
		int1_init(); // start INT1 Rising Edge Interrupt
		read_tim1(); // Stop Timer1 
	} 
}



int main(void)
{
	_delay_ms(1500);
	DDRB &= ~(1<<PORTB0); 
	uart_init();  //For Serial Monitor
	int1_init();  //Rising Edge Interrupt
	tim1_OV_interrupt_init(); 
	serial_debug("start!\n");
	sei();
    
    while (1) 
    {
    }
}

