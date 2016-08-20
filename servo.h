#ifndef SERVO_6460
#define SERVO_6460
#include "common.h"
#pragma systemFile

#define DISPENSER servo[p2c1s1]
#define DISPENSER_OPEN 252
#define DISPENSER_CLOSED 8
#define DISPENSER_SHOT_TIME 500
#define KNOCKOVER servo[p2c1s2]
#define KNOCKOVER_STOWED 132
#define KNOCKOVER_OPEN 4

// LICENSE ANDREI "HEXAFRACTION" AKHMETOV. ALL UNAUTHORIZED USE STRICTLY FORBIDDEN.

#define DEBUG_MSB servo[p2c1s5]
#define DEBUG_LSB servo[p2c1s6]
#define DEBUG_MSB_CHECK ServoValue[p2c1s5]
#define DEBUG_0 0
#define DEBUG_1 62
#define DEBUG_2 130
#define DEBUG_3 170
#define DEBUG_4 239

int p_dispenserShotMils;
bool p_dispenserInOneShotMode;

int p_waveTime;

#define FLAG_HOLDER servo[p2c2s3]
#define FLAG_0 90
#define FLAG_1 150
#define WAVE_TIME 250
void pollServos(){
  if(p_dispenserInOneShotMode){
    p_dispenserShotMils+=TICK;
    if(p_dispenserShotMils>DISPENSER_SHOT_TIME){
      p_dispenserInOneShotMode=false;
      DISPENSER=DISPENSER_CLOSED;
    }
  }
  p_waveTime+=TICK;
  servoChangeRate[p2c2s3]=1;
  if(p_waveTime>WAVE_TIME){
    FLAG_HOLDER=FLAG_0/**(1-(p_waveTime/WAVE_TIME))+FLAG_1*(p_waveTime/WAVE_TIME)*/;
  }
  if(p_waveTime>(2*WAVE_TIME)){
    FLAG_HOLDER=FLAG_1/**(1-(p_waveTime/WAVE_TIME))+FLAG_0*(p_waveTime/WAVE_TIME)*/;
    p_waveTime=0;
  }
}
void setKnockover(bool val){
	KNOCKOVER=val?KNOCKOVER_OPEN:KNOCKOVER_STOWED;
}
void openDispenser(){
  if(!p_dispenserInOneShotMode){
    DISPENSER=DISPENSER_OPEN;
  }
}

void closeDispenser(){
  if(!p_dispenserInOneShotMode){
    DISPENSER=DISPENSER_CLOSED;
  }
}

void dispenserOneShot(){
		if(!p_dispenserInOneShotMode){
 	 	p_dispenserShotMils=0;
 	 	p_dispenserInOneShotMode=true;
 	 	DISPENSER=DISPENSER_OPEN;
	}
}


#else
#warn "servo code included twice."
#endif
