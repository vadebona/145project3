/*
 * Project3.c
 *
 * Created: 5/11/2018 10:06:07 AM
 * Author : Vicky Adebona
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.c"
#include "lcd.c"


int main(void) {
	// Sets PB0 to output:
	DDRB = 1;
	clr_lcd();
	
	while (1) {
		SET_BIT(PORTB, 0); // Writes 1 to PORTB
		wait_avr(1000);
		
		CLR_BIT(PORTB, 0); // Writes 0 to PORTB
		wait_avr(1000);
		
	}
}

