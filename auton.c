#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     p1c1m1,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     p1c1m2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     p1c2m1,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     p1c2m2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     p1c3m1,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     p1c3m2,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     p1c4m1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     p1c4m2,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    p2c1s1,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    p2c1s2,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    p2c1s3,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    p2c1s4,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    p2c1s5,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    p2c1s6,               tServoStandard)
#include "common.h"
#include "lift.h"
#include "debug.h"
#include "drive_movement.h"
#include "servo.h"
#include "lib6460\joy.h"
#include "lib6460\libfcs.h"
#include "libauton.h"
#include "field.h"
#if defined(_Target_Emulator_)
#error "This program is not supported on an emulator."
#endif

#define ALLOW_BUTTON_START 1
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an TETRIX robot
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
bool p_startOnFloor;
void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	initDrive();
	startupDebug();
	// ask user for start position
	p_startOnFloor=askBoolean("Start position?", "Floor", "Ramp");
  return;
}

void runtimePoll(){
		pollDebug(true);
  	pollServos();
}

task main()
{
  initializeRobot();

  waitForStart(); // Wait for the beginning of autonomous phase

  if (p_startOnFloor){
  	forward(CENTER_ELEM_HALFWAY);
  	turnRight(90);
  	if (irSeekerDetecting()){
  		forward(POST_SENSE_BUMP);
  		turnLeft(90);
  		forward(SENSE_TO_POLE);
  		turnRight(90);
  	}
  	else {
  		arcTurnLeft(CENTER_GOAL_PASS_RADIUS,45);
  		if (irSeekerDetecting()) {
  			forward(POST_SENSE_BUMP);
  			turnLeft(90);
  			forward(SENSE_TO_POLE);
  			turnRight(90);
  		}
  		else {
  			arcTurnLeft(CENTER_GOAL_PASS_RADIUS,45);
  			forward(POST_SENSE_BUMP);
  			turnLeft(90);
  			forward(SENSE_TO_POLE);
  			turnRight(90);
  		}
  	}
  }
  else{
  	forward(RAMP_TO_FLOOR);
  	turnLeft(180);
  	forward(-TO_ROLLING_GOAL);
  	grabGoal();
  	turnRight(30);
  	forward(TO_PARKING_AREA);
  	turnRight(150);
  	releaseGoal();
  }

}
