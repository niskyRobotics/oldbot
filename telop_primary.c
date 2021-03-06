#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  HTServo,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     p1c1m1,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     p1c1m2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     p1c2m1,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     p1c2m2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     p1c3m1,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     p1c3m2,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     p1c4m1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     p1c4m2,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    p2c1s1,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    p2c1s2,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    p2c1s3,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    p2c1s4,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    p2c1s5,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    p2c1s6,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_1,    p2c2s1,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_2,    p2c2s2,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_3,    p2c2s3,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_4,    p2c2s4,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_5,    p2c2s5,               tServoStandard)
#pragma config(Servo,  srvo_S2_C2_6,    p2c2s6,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
// LICENSE ANDREI "HEXAFRACTION" AKHMETOV. ALL UNAUTHORIZED USE STRICTLY FORBIDDEN, AND SHALL BE REPORTED TO COMPETITION AUTHORITIES IMMEDIATELY.
#include "common.h"
#include "lift.h"
#include "debug.h"
#include "drive_movement.h"
#include "servo.h"
#include "harvester.h"
#include "lib6460\joy.h"
#include "lib6460\libfcs.h"
#include "music.h"
#if defined(_Target_Emulator_)
#error "This program is not supported on an emulator."
#endif

#define ALLOW_BUTTON_START 1

void initializeRobot()
{
	initDrive();
	//startTask(music);
	startupDebug();
	initDeCesare();
	stowAssist();
	return;
}


task main(){
	initializeRobot();
	waitForStartOrButton();
	parkDeCesare();
	setKnockover(false);
	int motorLeft = 0;
	int motorRight = 0;
	//nxtDisplayTextLine(6, "Compiled %s %s", __DATE__, __TIME__);
	int harvesterState = 0;
	while(true){
		nxtDisplayTextLine(6, "lift %d", (ENC_LIFT_LEFT+ENC_LIFT_RIGHT)/2);
	//	nxtDisplayTextLine(7, "builders are slow");
		pollJoystick();
		pollServos();
		pollHarvester();
		pollLiftStatus();
		pollDebug(true);
		//playImmediateTone(440, 10);
		motorLeft = joy1y1();
		motorRight = joy1y2();
		if(joy1Btn(8)){
			playImmediateTone(440, 10);
			motorLeft/=(2.5);
			motorRight/=(2.5);
		}
		if((joy1Hat()==-1)){
			updateSpeed(motorRight, motorLeft,0);
//playImmediateTone(440, 10);
		}
		else {

			switch(joy1Hat()){
			case 0:
				bumpMovement(1,0,20);
				break;
			case 1:
				bumpMovement(1,1,40);
				break;
			case 2:
				bumpMovement(0,1,40);
				break;
			case 3:
				bumpMovement(-1,1,40);
				break;
			case 4:
				bumpMovement(-1,0,20);
				break;
			case 5:
				bumpMovement(-1,-1,40);
				break;
			case 6:
				bumpMovement(0,-1,40);
				break;
			case 7:
				bumpMovement(1,-1,40);
				break;
			}
		}

		if(joy1Btn(7)){
			harvesterOn();
		}
		else if(joy1Btn(5)){
			harvesterReverse();
		}
		else{
			harvesterOff();
			}
		if(joy2Btn(2)){
			playImmediateTone(440, 10);
			harvesterToss();
		}
		if(nNxtButtonPressed==3){
			harvesterToss();
			//startTask(music);
		}
		if(joy2Btn(1)){
		grabGoal();
		}
		if(joy2Btn(3)){
			releaseGoal();
		}

		int lift=joy2y1();
		if(joy2Hat()==0){
			lift=40;
		}
		if(joy2Hat()==4){
			lift=-40;
		}

		setKnockover(joy1Btn(2));
		setAssist(joy1Btn(4));
		setLift(lift);

		pollLiftStatus();
	if(joy2Btn(8)){
		openDispenser();
	} else closeDispenser();
	if(joy2Btn(6)){
		dispenserOneShot();
	}
		wait1Msec(TICK);
	}
}
