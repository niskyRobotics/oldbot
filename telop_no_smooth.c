#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     p1c1m1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     p1c1m2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     p1c2m1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     p1c2m2,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drive_movement.h"
#include "lib6460\joy.h"
#include "lib6460\libfcs.h"
#if defined(_Target_Emulator_)
#error "This driver is not supported on an emulator."
#endif

#define ALLOW_BUTTON_START 0

void initializeRobot()
{
	// noop
	return;
}


task main()
{

	initializeRobot();
	// TODOS:
	// Refactor bump power to a macro
	// bump left and right using left and right triggers
	// control harvester using buttons right above triggers
#if ALLOW_BUTTON_START
	waitForStartOrButton();
#else
	waitForStart();
#endif
	int motorLeft = 0;
	int motorRight = 0;
	//nxtDisplayTextLine(6, "Compiled %s %s", __DATE__, __TIME__);
	int harvesterState = 0;
	while(true){
		int harvesterNow;
		pollJoystick();
		getJoystickSettings(joystick);
		wait1Msec(10);
		motorLeft = joystick.joy1_y1;
		motorLeft = joystick.joy1_y2;
		if((joy1Hat()==-1)){
			DRIVE_LEFT = motorLeft;
			DRIVE_RIGHT = motorRight;

		}
		else {
			//playImmediateTone(440, 10);

			switch(joy1Hat()){
			case 0:
				bumpMovement(1,0,20,20);
				break;
			case 1:
				bumpMovement(1,1,20,20);
				break;
			case 2:
				bumpMovement(0,1,20,20);
				break;
			case 3:
				bumpMovement(-1,1,20,20);
				break;
			case 4:
				bumpMovement(-1,0,20,20);
				break;
			case 5:
				bumpMovement(-1,-1,20,20);
				break;
			case 6:
				bumpMovement(0,-1,20,20);
				break;
			case 7:
				bumpMovement(1,-1,20,20);
				break;
			}
		}
	}
		if(joy1BtnTapped(8, false)){
			if(harvesterState == 1){
				harvesterState = 0;
			} else { harvesterState = 1;}
		}
		if(joy1BtnTapped(7, false)){
			if(harvesterState == -1){
				harvesterState = 0;
			} else { harvesterState = -1;}
		}
		int harvesterNow = harvesterState;
		if(joy1BtnDown(8)){
			harvesterNow = 1;
		}
		else if(joy1BtnDown(7)){
			harvesterNow = -1;
		}
		HARVESTER = harvesterNow * 80;

	}
