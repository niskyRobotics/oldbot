#ifndef TELEOP_MOVEMENT
#define TELEOP_MOVEMENT


#define DRIVE_TRACK_LEFT motor[p1c1m2]
#define DRIVE_TRACK_RIGHT motor[p1c1m1]
#ifdef ROBOTC
#define DMVT_C99
#endif

#include "JoystickDriver.c"
#include "common.h"
#pragma systemFile

typedef struct {
  int leftDrive;
  int rightDrive;
} twoMotorDrive;


static twoMotorDrive p_averagedValue;


void initDrive(){
	p_averagedValue.leftDrive=0;
	p_averagedValue.rightDrive=0;
}


void updateSpeed(int left, int right, int oldWeight){
	//int lAvg = ((oldWeight*p_averagedValue.leftDrive)+left)/(oldWeight+1);
	//int rAvg = ((oldWeight*p_averagedValue.rightDrive)+right)/(oldWeight+1);
	DRIVE_TRACK_LEFT = left;
	DRIVE_TRACK_RIGHT = right;


}

void bumpMovement(int forward, int rotateRight, int power){
	DRIVE_TRACK_LEFT = (forward-rotateRight)*power;
	DRIVE_TRACK_RIGHT = (forward+rotateRight)*power;

}
#else
#warn "drive movement code included twice."
#endif /* end of include guard: TELEOP_MOVEMENT */
