#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     p1c1m1,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     p1c1m2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     p1c2m1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     p1c2m2,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define DRIVE_LEFT motor[p1c1m1]
#define DRIVE_RIGHT motor[p1c1m2]
#define HARVESTER motor[p1c2m1]
#define BUMP_POWER 10;