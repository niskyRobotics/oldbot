#ifndef LIFT
#define LIFT
#include "lib6460\joy.h"
#define LIFT_RIGHT motor[p1c2m1]
#define LIFT_LEFT motor[p1c2m2]
#define ENC_LIFT_RIGHT nMotorEncoder[p1c2m1]
#define ENC_LIFT_LEFT nMotorEncoder[p1c2m2]
#define LIFT_MAX_VALUE 34600
#define RUN_TO_POS_SPEED 10
#include "common.h"
#include "debug.h"
#pragma systemFile
// LICENSE ANDREI "HEXAFRACTION" AKHMETOV. ALL UNAUTHORIZED USE STRICTLY FORBIDDEN.
typedef struct {
  int desiredSpeed;
  long targetPos;
  bool limitSwitchHit;
  bool runToTarget;
  int preset;
} liftControl;

static liftControl p_lift;


long getLiftPos(){
  // TODO check sign of encoder on reversed motor, adjust below line accordingly
  return (ENC_LIFT_LEFT+ENC_LIFT_RIGHT)/2;
}

void pollLiftStatus(){
	//playImmediateTone(440, 10);



  int actualSpeed = (((p_lift.desiredSpeed>0)&&(getLiftPos()>LIFT_MAX_VALUE))||((p_lift.desiredSpeed<0)&&(getLiftPos()<0)))?0:p_lift.desiredSpeed;
	if(p_lift.runToTarget&&encodersFunctioning()){
    actualSpeed	= RUN_TO_POS_SPEED*sgn(p_lift.targetPos - getLiftPos());
  }
  if(nNxtButtonPressed==1) actualSpeed=80;
	if(nNxtButtonPressed==2) actualSpeed=-80;

	LIFT_LEFT = actualSpeed;
  LIFT_RIGHT = actualSpeed;
}

void setHold(long target){
  p_lift.runToTarget = true;
  p_lift.targetPos = target;
}
void setLift(int speed){
	p_lift.desiredSpeed = speed;
	if((speed==0)&&!p_lift.runToTarget) setHold(getLiftPos());
	else {
  	p_lift.runToTarget = false;
	}
}



#define PRESET_0 0
#define PRESET_1 1440
#define PRESET_2 2880
#define PRESET_3 4320
#define PRESET_4 5760

int getPresetBelow(int pos){
	if(pos<PRESET_1) return 0;
	if(pos<PRESET_2) return 1;
	if(pos<PRESET_3) return 2;
	if(pos<PRESET_4) return 3;
	else return 4;
}

void setPreset(int preset){
	p_lift.preset=preset;
	switch(preset){
		case 0: setHold(PRESET_0); break;
		case 1: setHold(PRESET_1); break;
		case 2: setHold(PRESET_2); break;
		case 3: setHold(PRESET_3); break;
		case 4: setHold(PRESET_4); break;
		default: playImmediateTone(1000,50); emitError(ERR_INTERNAL_FAIL);
	}
}

void liftPresetDown(){
	playImmediateTone(880, 10);
	if(p_lift.runToTarget){
		playImmediateTone(440, 10);
		setPreset(p_lift.preset-1);
	} else setPreset(getPresetBelow(getLiftPos()));
}

void liftPresetUp(){
	playImmediateTone(440, 10);
	if(!p_lift.runToTarget){
		setPreset(p_lift.preset+1);
	} else setPreset(getPresetBelow(getLiftPos()+1));
}

#else
#warn "lift code included twice."
#endif
