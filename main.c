/*
 * Project4.c
 *
 * Created: 5/29/2018 12:28:57 PM
 * Author : Nelly Dzul
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.c"
#include "lcd.c"


struct State
{
	float min;
	float max;
	float current;
	//avg = sum/count
	float count;
	float sum; 	
};

struct State stats = {0, 0, 0, 0, 0};
	
void update(float s)
{
	if (s <= stats.min){
		stats.min = s;
	}
	
	if (s >= stats.max){
		stats.max = s;
	}
	
	stats.current = s;	
	stats.count += 1;
	stats.sum += s;
}


void display()
{
	char buf[10];
	char buf2[10];
	
	sprintf(buf, "%f   %f", stats.current, stats.max);
	sprintf(buf2, "%f  %f". stats.min, (stats.sum/stats.count));
	
}

float get_a2c()
{
	
	
}

int is_pressed(int r, int c) {
	// Set all R's and all C's to N/C
	DDRC = 0;
	PORTC = 0;
	
	// Set r to Strong 0
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	// Set c to Weak 1
	CLR_BIT(DDRC, c+4); // Add 4 to reach pins 4-7 on keypad
	SET_BIT(PORTC, c+4);
	wait_avr(1);
	
	// Change state of button
	if(GET_BIT(PINC, c+4)) {
		return 0; // button is not pressed
		} else {
		return 1; // button is pressed
	}
}


int get_key() {
	int r, c;
	
	for(r = 0; r < 4; ++r) {
		for(c = 0; c < 4; ++c) {
			if(is_pressed(r, c)) {
				return r * 4 + c + 1;
			}
		}
	}
	return 0;
}

int main(void)
{
	ini_avr();
	ini_lcd();
	clr_lcd();
	
	float s;
	int k;
	int on = 0;
    while (1) 
    {
		k = get_key();	//check user input
		if (k == 1) { //start
			on = 1;
		} else if (k == 2) { //reset
			stats = {0, 0, 0, 0, 0};
		}
		
		if (on == 1){
			wait_avr(500);
			s = get_a2c();
			update(s); //display(); //stats is a global 
		}
		
		display();
}

