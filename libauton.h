/*
 * UNITS:
 * distance in feet
 * velocity in feet/sec
 * acceleration in feet/sec^2
 * time in seconds
 * motor power normalized to (-100,100)
 * motor ramp rates are powerunits/sec
 *
 *
 */
#include "drive_movement.h"
#include "common.h"
#define ROBOT_VELOCITY 1
#define POWER_CORRECTION_FACTOR 0
#define MAX_RUN_POWER 80
#define RUN_ACCEL 80

bool askBoolean(const string query, const string trueLbl, const string falseLbl){
	return false;
}

void forward(float distance){
	float distanceTraveled=0;
	float time=0;
	float velocity=0;
	float maxVelocity=0;
	float distanceInAccel=0;
	clearTimer(T4);
	while((velocity<=MAX_RUN_POWER)&&distanceTraveled<(abs(distance)/2)){
		time=time1[T4]/1000;
		distanceTraveled=RUN_ACCEL/MAX_RUN_POWER*ROBOT_VELOCITY*0.5*time*time;
		velocity=RUN_ACCEL*time;
		updateSpeed(sgn(distance)*velocity,sgn(distance)*velocity,0);
		distanceInAccel=distanceTraveled;
		maxVelocity=velocity;
	}
	clearTimer(T4);
	while(distanceTraveled<(abs(distance)-distanceInAccel)){
		time=time1[T4]/1000;
		updateSpeed(sgn(distance)*MAX_RUN_POWER, sgn(distance)*MAX_RUN_POWER,0);
		distanceTraveled=distanceInAccel+(ROBOT_VELOCITY*time);
	}
	clearTimer(T4);
	while(velocity>0){
		time=time1[T4]/1000
		velocity=maxVelocity-(MAX_RUN_POWER*time*RUN_ACCEL);
		updateSpeed(sgn(distance)*velocity,sgn(distance)*velocity,0);
	}

}

bool irSeekerDetecting(){
	return false;
}

void turnLeft(int degrees){}
void turnRight(int degrees){}

void arcTurnLeft(float radius, int degrees){}
