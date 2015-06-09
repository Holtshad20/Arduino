//============================================================
// Arduino POV Display
//
// Author: Carlos Asmat
// Last Modified: August 17, 2007
//
// Description: this is a quick code for a POV display
//		implemented using the Arduino. It used 6 LEDs
//		connected to the pins.
//		See http://carlitoscontraptions.blogspot.com
//		for more details.
//============================================================

int pins[] = {7,6,5,4,3,2};	// an array of pin numbers
int col_len = 6;		// column lenght

// customizable parameters
int timer1 = 3;			// time between columns
int timer2 = 15;		// time between frames
int timer3 = 0;			// time between drawings
int frame_len = 4;		// frame length
int frame_num = 1;		// number of frames
// data corresponding to the image to be displayed
int data[] = {1,0,0,0,0,1, 1,1,0,0,1,1, 0,1,1,1,1,0, 0,0,1,1,0,0}; 

void setup()
{
	int i;
	for (i = 0; i < col_len; i++)
		pinMode(pins[i], OUTPUT);	// set each pin as an output
}

void loop()
{
	int a,b,c;
	
	// go through all data for all columns in each frame.
	for (a = 0; a < frame_num; a++)
	{
		for (b = 0; b < frame_len; b++)
		{
			for (c = 0; c < col_len; c++)
			{
				if (data[a*frame_len*col_len + b*col_len + c] == 0) {digitalWrite(pins[c], LOW);}
				else {digitalWrite(pins[c], HIGH);}
			}
			delay(timer1);
		}
		for (c = 0; c < col_len; c++)
		{digitalWrite(pins[c], LOW);}
		delay(timer2);
	}
	delay(timer3);
}
