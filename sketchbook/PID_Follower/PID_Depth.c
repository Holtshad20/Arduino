
#define _POSIX_C_SOURCE 199309L

/*
 *  PID_Depth.c
 *  Seawolf
 *
 *  PID Controller Code for Depth
 *
 *  Created by Matthias Welsh on 3/20/09.
 *  Copyright 2009 NCSU Underwater. All rights reserved.
 *
 *  Some of the commands such as the get commands are 
 *  from SeaSQL our custom MySQL database implimentation
 *
 */
#include <stdio.h>
#include <time.h>
#include <seawolf.h>

#define K_p 10
#define K_i 10
#define K_d 10

float getDepthHeading();
float getDepth();
void dataOut(float MV);
float proportional(float e_current);
float integral(float e_current, float I_out);
float derivative(float e_current, float e_previous);
float difftimespec(struct timespec* current, struct timespec* previous);

//timer vars
struct timespec IT_current, IT_previous, DT_current, DT_previous;

int main(){
	//SeaSQL Setup
	SeaSQL_configFile("../libseawolf/conf/seasql.conf");
	Seawolf_init("PID Depth");
	char Action[32];
	char Data[32];
	
	//PID vars
	float SP; //set point
        float SP_tmp; //new SP
	float PV; //process variable
	float e_current = 0;  //error
	float e_previous;
	float P_out = 0;
	float I_out = 0;
	float D_out = 0;
	float MV;

        IT_previous.tv_sec = 0;
        IT_previous.tv_nsec = 0;

	Notify_filter(FILTER_MATCH,"UPDATED Depth");
	
	// get desired depth
	SP = getDepthHeading();
	
	while(1){
		if((SP_tmp = getDepthHeading()) != SP){
			//if the set point changes we need to flush the integral.
			SP = SP_tmp;
			I_out = 0;
			D_out = 0;
		}
		
		//This is part of the SeaSQL library
		//When we see a notify indicating updated depth:
		Notify_get(Action,Data);
		//get new depth
		PV = getDepth();
		
		//PID Calculations
		e_current = SP - PV; //Error
		P_out = proportional(e_current); //proportional
		I_out = integral(e_current,I_out); //integral
		D_out = derivative(e_current,e_previous); //derivative
                //printf("(%.1f %.1f %.1f| %.3f)\n", P_out, I_out, D_out, difftimespec(&IT_current, &IT_previous)); //Debug commands
		MV = P_out + I_out + D_out; //PID
		

		//no need to spit out values larger than this for our purposes
		//comment out after tuning.
		if(MV>63)
			MV = 63;
		if(MV<-63)
			MV = -63;
		
		//On the first loop through ouput won't make sense (delta_T won't be right)
		if(IT_previous.tv_sec){
			dataOut(MV);
		}
		
		//var bumps current to previous
		e_previous = e_current;
			IT_previous.tv_sec = IT_current.tv_sec;
			IT_previous.tv_nsec = IT_current.tv_nsec;
			DT_previous.tv_sec = DT_current.tv_sec;
			DT_previous.tv_nsec = DT_current.tv_nsec;
	}
	
	return 0;
}

//Get the depth heading
float getDepthHeading(){
	return SeaSQL_getDepthHeading();
}

//Get current depth
float getDepth(){
	return SeaSQL_getDepth();
}

//Spit out some data
void dataOut(float MV){
	SeaSQL_setAft(MV);
	SeaSQL_setPortY(MV);
	SeaSQL_setStarY(MV);

	return;
}

//Calculate Proportion
float proportional(float e_current){
	float P_out;

	P_out = K_p * e_current;

	return P_out;
}

float difftimespec(struct timespec* current, struct timespec* previous) {
	return ((float)current->tv_sec - previous->tv_sec) + (((float)current->tv_nsec - previous->tv_nsec)/1e9);
}

//Calculate Integral
float integral(float e_current, float I_out){
	//generate a delta t
	float delta_T;
        clock_gettime(CLOCK_REALTIME, &IT_current);
	delta_T = difftimespec(&IT_current, &IT_previous);
	
	I_out = K_i*(e_current + I_out) * delta_T;
	return I_out;
}

//Calculate Derivative
float derivative(float e_current, float e_previous){
	//generate a delta t
	float delta_T;
        clock_gettime(CLOCK_REALTIME, &DT_current);
	delta_T = difftimespec(&DT_current, &DT_previous);
	
	float D_out;
	
	D_out = K_d*(e_previous-e_current) / delta_T;
	return D_out;
}
