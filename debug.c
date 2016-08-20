#ifndef DEBUG_6460
#define DEBUG_6460
#include "servo.h"
#include "lift.h"
#include "JoystickDriver.c"
#pragma systemFile

#define ERR_PENDING 15
#define ERR_OK 20
#define ERR_INVALID_ERROR 0

#define ERR_SAMANTHA 1
#define ERR_TETRIX_BATT 2
#define ERR_NXT_BATT 4
#define ERR_TETRIX_BATT_DISCONNECTED 3

#define ERR_MOTOR_CHAIN 6
#define ERR_SRVO_CHAIN 7
#define ERR_LIMIT_SWITCH 8
#define ERR_ENCODER 9

#warn "battery voltages"
#define NXT_BATTERY_LOW 2000
#define TETRIX_BATTERY_LOW 11000

void emitError(int errno){
  if(errno<0||errno>24) errno=ERR_INVALID_ERROR;
  switch(errno/5){
    case 0: DEBUG_MSB=DEBUG_0; break;
    case 1: DEBUG_MSB=DEBUG_1; break;
    case 2: DEBUG_MSB=DEBUG_2; break;
    case 3: DEBUG_MSB=DEBUG_3; break;
    case 4: DEBUG_MSB=DEBUG_4; break;
    default: playImmediateTone(440, 10);
  }
  switch(errno%5){
    case 0: DEBUG_LSB=DEBUG_0; break;
    case 1: DEBUG_LSB=DEBUG_1; break;
    case 2: DEBUG_LSB=DEBUG_2; break;
    case 3: DEBUG_LSB=DEBUG_3; break;
    case 4: DEBUG_LSB=DEBUG_4; break;
    default: playImmediateTone(880, 10);
  }
}

 bool dbg_encodersFunctioning; // for lack of a better debug test yet

bool encodersFunctioning(){
	return dbg_encodersFunctioning;
}

bool p_stageOnePassed=true;
void pollDebug(bool inRun){
  bool pass=true;
  if(p_stageOnePassed){
    if(nAvgBatteryLevel<NXT_BATTERY_LOW){
      pass=false;
      emitError(ERR_NXT_BATT);
    }
    if(externalBatteryAvg<TETRIX_BATTERY_LOW){
      pass=false;
      emitError(ERR_TETRIX_BATT);
    }
  }
  if(bDisconnected){
    pass=false;
    emitError(ERR_SAMANTHA);
  }
  if(externalBatteryAvg<0){
    pass=false;
    emitError(ERR_TETRIX_BATT_DISCONNECTED);
  }
  //TODO check samantha
  if((inRun||p_stageOnePassed)&&pass) emitError(ERR_OK);
}
void startupDebug(){
  hogCPU();
  int oldSrvoVal = DEBUG_MSB_CHECK;
  emitError(ERR_PENDING);
  wait1Msec(200);
  if(DEBUG_MSB_CHECK==oldSrvoVal) {
    emitError(ERR_SRVO_CHAIN);
    p_stageOnePassed=false;
  }

  ENC_LIFT_LEFT=0;
  ENC_LIFT_RIGHT=0;
  LIFT_LEFT=100;
  LIFT_RIGHT=100;
  wait1Msec(100);

  if(ENC_LIFT_LEFT==0||ENC_LIFT_RIGHT==0){
    dbg_encodersFunctioning=false;
    emitError(ERR_ENCODER);
    p_stageOnePassed=false;
  }
	LIFT_LEFT=0;
	LIFT_RIGHT=0;
  releaseCPU();
  pollDebug(false);
}


void waitForStartAndDebug()
{
  while (true)
  {
    getJoystickSettings(joystick);
    if (!joystick.StopPgm)
      break;

    pollDebug(false);
  }
  return;
}



void waitForStartOrButtonAndDebug()
{
  while (nNxtButtonPressed>=0) {  // wait for button to be released
  }

  while (true)
  {
    pollDebug(false);
    getJoystickSettings(joystick);
    nxtDisplayCenteredTextLine(2, "  or Button  ");

    if (!joystick.StopPgm)        // normal FCS start condition
      break;
    if (nNxtButtonPressed>=0) {   // manual bypass start condition

      int buttonToggle;  // holds button state for manual start override with another push
      buttonToggle=0;

      // beeps and waits a few seconds to get out of the way before the program starts
      //bDisplayDiagnostics=false;

      eraseDisplay();
      nxtDisplayCenteredBigTextLine(1,"Start");
      nxtDisplayCenteredBigTextLine(3,"In");

      for (int i=3; (i>0)&&(buttonToggle!=2); i--) {
        nxtDisplayCenteredBigTextLine(5,"%d!", i);

        clearTimer(3);

        playImmediateTone(1000,50);

        while (time1[3]<1000) {
          if ((buttonToggle==0)&&(nNxtButtonPressed<0)) {
            buttonToggle=1;
          }
          if ((buttonToggle==1)&&(nNxtButtonPressed>=0)) {
            buttonToggle=2;
            eraseDisplay();
            nxtDisplayCenteredBigTextLine(0,"Abort!");
            nxtDisplayCenteredBigTextLine(2,"Return");
            nxtDisplayCenteredBigTextLine(4,"To");
            nxtDisplayCenteredBigTextLine(6,"Wait!");
            wait1Msec(1000);
            while (nNxtButtonPressed>=0) {  // wait for button to be released
            }
          }
        }
      }

      bDisplayDiagnostics=true;
      eraseDisplay();
      if (buttonToggle!=2)   // start now if no override!
        break;
    }
  }
  return;
}


#endif
