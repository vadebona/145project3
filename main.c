/*
 * Project3.c
 *
 * Created: 5/11/2018 10:06:07 AM
 * Author : Vicky Adebona and Nelly F. Dzul Chi
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.c"
#include "lcd.c"

#define A1 440
#define C 523
#define D 587
#define E 659
#define F 698
#define G 784
#define A2 880

#define W 1000
#define H 500
#define Q 250
//#define E 125

struct note {
	int f; 
	int d;
};


/* Twinkle, Twinkle Little Star */
struct note SONG[] = {{C, Q}, {C,Q}, {G, Q}, {G, Q}, {A2, Q}, {A2, Q}, {G, H}, {F, Q}, {F, Q}, {E, Q}, {E, Q}, {D, Q}, {D,Q}, {C, H},
						{G, Q}, {G, Q}, {F, Q}, {F, Q}, {E, Q}, {E, Q}, {D, H}, 
						{G, Q}, {G, Q}, {F, Q}, {F, Q}, {E, Q}, {E, Q}, {D, H},
						{C, Q}, {C,Q}, {G, Q}, {G, Q}, {A2, Q}, {A2, Q}, {G, H}, {F, Q}, {F, Q}, {E, Q}, {E, Q}, {D, Q}, {D,Q}, {C, H}};

void playNote(int freq, int dur)
{
	int i, k;
	float t;
	k = dur * freq;
	t = (((1/freq)/2)*5000000);
	
	for (i = 0; i < k; ++i) {
		SET_BIT(PORTB, 0); // Writes 1 to PORTB
		wait_avr(t);	//1000);
		
		CLR_BIT(PORTB, 0); // Writes 0 to PORTB
		wait_avr(t);	//1000);
	}	
}

void playMusic(struct note *S)
{
	int i;
	for (i = 0; i < sizeof(S); ++i)
	{
		playNote(S[i].f, S[i].d);
	}	
}

int main(void) {
	// Sets PB0 to output:
	ini_avr();
	DDRB = 1;
	clr_lcd();
	
	playMusic(SONG);
/*	while (1) {
		SET_BIT(PORTB, 0); // Writes 1 to PORTB
		wait_avr(1000);
		
		CLR_BIT(PORTB, 0); // Writes 0 to PORTB
		wait_avr(1000);
		
	}*/
} 

