/*
 * Project5.c
 *

 * Created: 6/6/2018 7:04:49 PM

 * Author : Nelly Dzul & Victoria Adebona

 */ 



#include <avr/io.h>

#include "avr.c" 

#include "lcd.c"

#include <stdio.h>



#define Gb4	370

#define G4	392

#define Ab	415

#define A4	440

#define Bb4 466

#define B4	494

#define C5	523

#define Db5	554

#define D	  587

#define Eb  622

#define E	  659

#define F	  698

#define Gb5	740

#define G5	784

#define Ab5	831

#define A5	880

#define Bb5	932

#define B5	988

#define Cb5	1047

#define C6	1109

#define Db6	1175



#define W  1000

#define H  500

#define Q  250

#define EH 125



struct note {
	int f;
	int d;
};



struct Clock {

	unsigned int month;

	unsigned int day;

	unsigned int year;

	

	unsigned int hour;

	unsigned int minute;

	unsigned int second;



	unsigned int flag;				//for hour, if it is AM then

};



struct note sunshine[82] = {

	{A4, Q}, {Db5, Q}, {E, Q}, {E, Q}, {E, H}, {E, Q}, {E, Q}, {Gb5, Q}, {A5, H}, {Db5, Q}, {B4, Q}, {A4, Q}, {Gb4, Q}, {B4, H}, {A4, W}, //15

	{A4, Q}, {Db5, Q}, {E, Q}, {E, Q}, {E, H}, {E, Q}, {E, Q}, {Gb5, Q}, {A5, H}, {Db5, Q}, {B4, Q}, {A4, Q}, {Gb4, Q}, {B4, H}, {A4, W}, //15

	{A4, Q}, {Db5, Q}, {E, Q}, {E, Q}, {E, H}, {E, Q}, {E, Q}, {Gb5, Q}, {Gb5, Q}, {Gb5, Q}, {Db5, Q}, {Gb5, H}, {E, W}, {E, W},			//14

	{Gb5, Q}, {A5, Q}, {Gb5, Q}, {Db5, Q}, {Gb5, H}, {E, Q}, {Db5, Q}, {Gb5, H}, {E, H}, {Db5, Q}, {B4, Q}, {A4, Q}, {Gb4, Q}, {B4, Q}, {A4, W}, {A4, W}, //16

	{Gb5, Q}, {A5, Q}, {Gb5, Q}, {Db5, Q}, {Gb5, H}, {E, W}, {B5, H}, {A5, W},	//8

	{Gb5, Q}, {A5, Q}, {Gb5, Q}, {Db5, Q}, {Gb5, H}, {E, W}, {B5, H}, {A5, W}, {Gb5, Q}, {A5, Q}, {Gb5, Q}, {A5, Q}, {Db6, W} //14

};


int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // stores how many days are in each month in an array
char mmddyyyy[11] = {'0','1','/','0','1','/','0','0','0','0','\0'};
char hhmmss[9] = {'0','0',':','0','0',':','0','0','\0'}; 
int setAlarm = 0;
int alarmOn = 0;
int displayAlarm = 0;


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





struct Clock set_struct(struct Clock cl, int k)

{

	if (k == 1) {

		//cl.second -= 2;
		if (cl.second <= 0){

			cl.second = 59;

		} else {
			cl.second -= 2;
		}

	} else if (k == 2) {

		cl.second += 1;

		if (cl.second >= 60) {

			cl.second = 0;

		}

	} else if (k == 5) {

		if (cl.minute <= 0) {

			cl.minute = 59;

		} else {

			cl.minute -= 1;

		}

	} else if (k == 6) {

		cl.minute += 1;

		if (cl.minute >= 60) {

			cl.minute = 0;

		}

	} else if (k == 9) {

		if (cl.hour == 0){

			cl.hour = 23;

		} else {

			cl.hour -= 1;

		}

	} else if (k == 10) { //taken care of??

		if (cl.hour == 23){

			cl.hour = 0;

		} else {

			cl.hour += 1;

		}

	} else if (k == 3) {

		cl.day -= 1;

		if (cl.day <= 0){

			cl.day = months[cl.month-1];

		}

	} else if (k== 4) {

		cl.day += 1;

		if (cl.day > months[cl.month-1]){

			cl.day = 1;

		}

	} else if (k == 7) {

		if (cl.month == 1) {

			cl.month = 12;

		} else {

			cl.month -= 1;

		}

		

		if (cl.day > months[cl.month-1]){

			cl.day = 1;

		}

	} else if (k == 8) {

		cl.month += 1;

		if (cl.month > 12){

			cl.month = 1;

		}

		if (cl.day > months[cl.month-1]){

			cl.day = 1;

		}

	} else if (k == 11 && cl.year != 0) {

		cl.year -= 1;

	} else if (k == 12) {

		cl.year += 1;

	}

	return cl;

}





//CHANGES THE BUFFER OR MOVES CURSOR

int get_input()

{

	int k;

	k = get_key();

	if (k == 13) {

		if (setAlarm == 0) {

			setAlarm = 1;

		} else { //setting the alarm

			displayAlarm = 1;

			setAlarm = 0;

		}

	} else if (k == 14) {

		if (alarmOn == 0) {

			alarmOn = 1;

		} else {

			alarmOn = 0;

		}

	} else if (k == 15){

		if (displayAlarm == 1) {

			displayAlarm = 0;

		} else {

			displayAlarm = 1;

		}

	} /*else if (k <= 12 && k >= 1) {

		if (setAlarm == 1) {

			alarm = set_struct(alarm, k);

			return alarm;

		} else {

			c = set_struct(c, k);

			return c;

		}

	}*/

	return k;

}





struct Clock run_clock(struct Clock c)

{

	c.second += 1;

	if (c.second == 60){

		c.second = 0;

		c.minute += 1;

		if (c.minute == 60){

			c.minute = 0;

			c.hour += 1;

			if (c.hour == 24){

				c.hour = 0;

				c.day += 1;

				

				int days_in_month = months[c.month-1];

				if (c.day % days_in_month  == 0) {

					c.day = 1;

					c.month += 1;

					if (c.month == 13){

						c.month = 1;

						c.year += 1;

					}

				}

			}

		}

	}

	return c;

}











int get_hour(struct Clock clock)

{

	if (clock.hour == 0){

		return 12;

	} else if (clock.hour == 12) {

		return 12;

	} else if (clock.hour >= 1 && clock.hour <= 11) {

		return clock.hour;

	} else if (clock.hour > 12 && clock.hour <= 23) {

		return clock.hour - 12;

	}

	return clock.hour; //to get rid of warning, this wasnt here before

}



struct Clock set_flag(struct Clock clock) //does it just change it 

{

	if (clock.hour == 0){

		clock.flag = 0;		//12 AM

	} else if (clock.hour == 12) {

		clock.flag = 1;		//12 PM

	} else if (clock.hour >= 1 && clock.hour <= 11) {

		clock.flag = 0;

	} else if (clock.hour > 12 && clock.hour <= 23) {

		clock.flag = 1;

	}

	return clock ;

}



int playNote(int freq, int dur)

{
	int i, t;
	unsigned int k;
	float period = 1.0/ (freq);
	float halfOfT = period/2.0;
	float x = (halfOfT*20000);
	t = (int)x;
	k = (dur/1000.0) * freq;
	for (i = 0; i < k; ++i) {
		SET_BIT(PORTB, 0); // Writes 1 to PORTB
		wait_sound(t);	//1000);
		
		CLR_BIT(PORTB, 0); // Writes 0 to PORTB
		wait_sound(t);	//1000);
	}
	return dur;
}


/*
void playMusic(struct note *S)

{

	int i;

	for (i = 0; i < sizeof(S); ++i)

	{

		playNote(S[i].f, S[i].d);

	}

}
*/




int check_alarm(struct Clock clock, struct Clock alarm) {

	if ((clock.hour == alarm.hour) && (clock.minute == alarm.minute) && (clock.second == alarm.second) && (clock.flag == alarm.flag)) {
		return 1;
	} else { return 0; }

}



struct Clock display(struct Clock clock)
{
		/*if (displayAlarm == 0) {
			sprintf(mmddyyyy, "%02i/%02i/%04i", clock.month, clock.day, clock.year);
			pos_lcd(0,0);
			puts_lcd2(mmddyyyy);
			} else {
			//displayAlarm = 0;
			char hhmmssAlarm[9];
			alarm = set_flag(alarm);
			char amPm, al;
			if (alarm.flag == 0) {
				amPm = 'A';
				} else {
				amPm = 'P';
			}
			if (alarmOn) {
				al = 'O';
				} else {
				al = 'F';
			}
			sprintf(hhmmssAlarm, "%02i:%02i:%02i %c %c", get_hour(alarm), alarm.minute, alarm.second, amPm, al);
			pos_lcd(0,0);
			puts_lcd2(hhmmssAlarm);
		}*/
		int hr = get_hour(clock);
		clock = set_flag(clock);
		char amPm;
		if (clock.flag == 0) {
			amPm = 'A';
			} else {
			amPm = 'P';
		}
		sprintf(hhmmss, "%02i:%02i:%02i %c", hr, clock.minute, clock.second, amPm);
		pos_lcd(1,0);
		puts_lcd2(hhmmss);
		return clock;
}



int main()

{

	struct Clock c = {1, 1, 0, 0, 0, 0, 0};//flag 0 = AM, 1 = PM
	struct Clock alarm = {1, 1, 0, 0, 0, 0, 0};
	ini_avr();
	ini_lcd();
	clr_lcd();


	while(1){

		//c = display(c);
		if (displayAlarm == 0) {
			sprintf(mmddyyyy, "%02i/%02i/%04i", c.month, c.day, c.year);
			pos_lcd(0,0);
			puts_lcd2(mmddyyyy);
		} else {
			//displayAlarm = 0;
			char hhmmssAlarm[9];
			alarm = set_flag(alarm);
			char amPm, al;
			if (alarm.flag == 0) {
				amPm = 'A';
			} else {
				amPm = 'P';
			} 
			if (alarmOn) {
				al = 'O';
			} else {
				al = 'F';
			}
			sprintf(hhmmssAlarm, "%02i:%02i:%02i %c %c", get_hour(alarm), alarm.minute, alarm.second, amPm, al);
			pos_lcd(0,0);
			puts_lcd2(hhmmssAlarm);
		}
		int hr = get_hour(c);
		c = set_flag(c);
		char amPm;
		if (c.flag == 0) {
			amPm = 'A';
		} else {
			amPm = 'P';
		}
		sprintf(hhmmss, "%02i:%02i:%02i %c", hr, c.minute, c.second, amPm);
		pos_lcd(1,0);
		puts_lcd2(hhmmss);
		///////////////////////////////
		int input = get_input();
		if (input <= 12 && input >= 1) {
			if (setAlarm == 1) {
				alarm = set_struct(alarm, input);
			} else {
				c = set_struct(c, input);
			}
		}
		////////////////alarm
		if(alarmOn) {
			int alarmSound = check_alarm(c, alarm); // determines if the current time matches the alarm that has just been set
			if (alarmSound == 1) {
				int i;
				int time = 0;
				for (i = 0; i < 82; ++i)
				{
					time += playNote(sunshine[i].f, sunshine[i].d);
					if (time >= 1000) {
						c = run_clock(c);
						//c = display(c);
						time = 0;
					}
					c = display(c);
				}	
			}
		}
		wait_avr(1000);
		c = run_clock(c); //advance time/update clock
	}
	return 0;
}