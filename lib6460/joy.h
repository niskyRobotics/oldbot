#include "JoystickDriver.c"
//#pragma systemFile
#ifndef JOYSTICK_DRIVER_LOADED
#define JOYSTICK_DRIVER_LOADED
#pragma systemFile
#define SET_BIT(NUM, BIT) NUM |= 1 << BIT
#define CLR_BIT(NUM, BIT) NUM &= ~(1 << BIT)
#define TOGGLE_BIT(NUM, BIT) NUM ^= 1 << BIT
#define ISSET_BIT(NUM, BIT) ((NUM & (1 << BIT))!=0)
#define LONGPRESS_TIME 1000
#define DEAD_ZONE 10
#define SOUND_DEBUG 1
TJoystick mJoy;

/*
* TODOs:
* Possible double-tap handler (release-to-press timing)
* Testing also needs to occur--hopefully done today (2014.10.31)
*/

typedef struct {
	short x1;
	short y1;
	short x2;
	short y2;
} joystickAnalogStatus;

static bool mInitComplete = false;

static int mLastCycleTime = 0;

static int mTopHat1 = -1;
static int mTopHat2 = -1;

static joystickAnalogStatus mJoy1Analog;
static joystickAnalogStatus mJoy2Analog;

/*
* 0: Button is down NOW
* 1: Button has waiting down message
* 2: Button has waiting up message
* 3: Button has waiting tapped message
* 4: Button has waiting long-pressed message
*/
static int mJoy1ButtonFlags[12];
static int mJoy2ButtonFlags[12];

static int mJoy1ButtonTimes[12];
static int mJoy2ButtonTimes[12];

static void mPollJoystick(){
	// Grab non-volatile copy of joystick data into mJoy
	getJoystickSettings(mJoy);
	// work around bug JIRA-AN17
	// set CTHDR section
	// if the preprocessor variable ROBOTC_ON_6460 is set, then set the section to 0, otherwise do nothing.
	mLastCycleTime = time1[T4];
	time1[T4] = 0;

	// Set top-hat variables
	mTopHat1 = mJoy.joy1_TopHat;
	mTopHat2 = mJoy.joy2_TopHat;
	// set analog variables in structs
	mJoy1Analog.x1 = mJoy.joy1_x1;
	mJoy1Analog.y1 = mJoy.joy1_y1;
	mJoy1Analog.x2 = mJoy.joy1_x2;
	mJoy1Analog.y2 = mJoy.joy1_y2;
	mJoy2Analog.x1 = mJoy.joy2_x1;
	mJoy2Analog.y1 = mJoy.joy2_y1;
	mJoy2Analog.x2 = mJoy.joy2_x2;
	mJoy2Analog.y2 = mJoy.joy2_y2;
	/*
	* 0: Button is down NOW
	* 1: Button has waiting down message
	* 2: Button has waiting up message
	* 3: Button has waiting tapped message
	* 4: Button has waiting long-pressed message
	*/

	// joystick 1
	for(int i = 0; i < 12; i++){
		bool oldState = ISSET_BIT(mJoy1ButtonFlags[i], 0);
		bool newState = ISSET_BIT(mJoy.joy1_Buttons, i);
		if(!oldState && newState){
			// button just got pressed. Let's set the down flag.
			SET_BIT(mJoy1ButtonFlags[i], 1);
			// reset timer
			mJoy1ButtonTimes[i] = 0;
			} else if(oldState && !newState){
			// button just got released. Let's set the up flag.
			SET_BIT(mJoy1ButtonFlags[i], 2);
			if(mJoy1ButtonTimes[i] > LONGPRESS_TIME){
				// the button was held down for a while. Setting long-press flag.
				SET_BIT(mJoy1ButtonFlags[i],4);
				} else {
				// the button was only pressed for a short time. Setting tap flag.
				SET_BIT(mJoy1ButtonFlags[i],3);
			}
			// reset timer
			mJoy1ButtonTimes[i] = 0;
			} else {
			mJoy1ButtonTimes[i] = mJoy1ButtonTimes[i] + mLastCycleTime;
		}
	}

	// joystick 2
	for(int i = 0; i < 12; i++){
		bool oldState = ISSET_BIT(mJoy2ButtonFlags[i], 0);
		bool newState = ISSET_BIT(mJoy.joy2_Buttons, i);
		if(!oldState && newState){
			// button just got pressed. Let's set the down flag.
			SET_BIT(mJoy2ButtonFlags[i], 1);
			// reset timer
			mJoy2ButtonTimes[i] = 0;
			} else if(oldState && !newState){
			// button just got released. Let's set the up flag.
			SET_BIT(mJoy2ButtonFlags[i], 2);
			if(mJoy2ButtonTimes[i] > LONGPRESS_TIME){
				// the button was held down for a while. Setting long-press flag.
				SET_BIT(mJoy2ButtonFlags[i],4);
				} else {
				// the button was only pressed for a short time. Setting tap flag.
				SET_BIT(mJoy2ButtonFlags[i],3);
			}
			// reset timer
			mJoy2ButtonTimes[i] = 0;
			} else {
			mJoy2ButtonTimes[i] = mJoy2ButtonTimes[i] + mLastCycleTime;
		}
	}
}

void pollJoystick(){
	hogCPU();
	if(!mInitComplete){
		time1[T4] = 0;
		memset(mJoy1ButtonFlags, 0, sizeof(mJoy1ButtonFlags));
		memset(mJoy2ButtonFlags, 0, sizeof(mJoy2ButtonFlags));
		mInitComplete = true;
	}
	mPollJoystick();
	releaseCPU();
}
//  ###  #####  #   #
//  #  #   #    ##  #
//  ###    #    # # #
//  #  #   #    #  ##
//  ###    #    #   #


int joy1Hat(){
	return mTopHat1;
}

int joy2Hat(){
	return mTopHat2;
}
/*
* 0: Button is down NOW
* 1: Button has waiting down message
* 2: Button has waiting up message
* 3: Button has waiting tapped message
* 4: Button has waiting long-pressed message
*/
bool joy1BtnDown(int btn){
	return ISSET_BIT(mJoy1ButtonFlags[btn-1], 0);
}
bool joy2BtnDown(int btn){
	return ISSET_BIT(mJoy2ButtonFlags[btn-1], 0);
}
//pressed
bool joy1BtnPressed(int btn, bool peek){
	return ISSET_BIT(mJoy1ButtonFlags[btn-1], 1);
	if(!peek){
		CLR_BIT(mJoy1ButtonFlags[btn-1], 1);
	}
}
bool joy2BtnPressed(int btn, bool peek){
	return ISSET_BIT(mJoy2ButtonFlags[btn-1], 1);
	if(!peek){
		CLR_BIT(mJoy2ButtonFlags[btn-1], 1);
	}
}
//released
bool joy1BtnReleased(int btn, bool peek){
	return ISSET_BIT(mJoy1ButtonFlags[btn-1], 2);
	if(!peek){
		CLR_BIT(mJoy1ButtonFlags[btn-1], 2);
	}
}
bool joy2BtnReleased(int btn, bool peek){
	return ISSET_BIT(mJoy2ButtonFlags[btn-1], 2);
	if(!peek){
		CLR_BIT(mJoy2ButtonFlags[btn-1], 2);
	}
}
//tapped
bool joy1BtnTapped(int btn, bool peek){
	return ISSET_BIT(mJoy1ButtonFlags[btn-1], 3);
	if(!peek){
		CLR_BIT(mJoy1ButtonFlags[btn-1], 3);
	}
}
bool joy2BtnTapped(int btn, bool peek){
	return ISSET_BIT(mJoy2ButtonFlags[btn-1], 3);
	if(!peek){
		CLR_BIT(mJoy2ButtonFlags[btn-1], 3);
	}
}
//long-pressed
bool joy1BtnLongPressed(int btn, bool peek){
	return ISSET_BIT(mJoy1ButtonFlags[btn-1], 4);
	if(!peek){
		CLR_BIT(mJoy1ButtonFlags[btn-1], 4);
	}
}
bool joy2BtnLongPressed(int btn, bool peek){
	return ISSET_BIT(mJoy2ButtonFlags[btn-1], 4);
	if(!peek){
		CLR_BIT(mJoy2ButtonFlags[btn-1], 4);
	}
}

//  #   #  #   #
//   # #    # #
//    #      #
//   # #     #
//  #   #    #

int joy1x1(){
	if(abs(mJoy1Analog.x1)<DEAD_ZONE) return 0;
	else return mJoy1Analog.x1;
}
int joy1y1(){
	if(abs(mJoy1Analog.y1)<DEAD_ZONE){ return 0;
		playImmediateTone(440, 2);
	}
	else return mJoy1Analog.y1;
}
int joy1x2(){
	if(abs(mJoy1Analog.x2)<DEAD_ZONE) return 0;
	else return mJoy1Analog.x2;
}
int joy1y2(){
	if(abs(mJoy1Analog.y2)<DEAD_ZONE) return 0;
	else return mJoy1Analog.y2;
}
int joy2x1(){
	if(abs(mJoy2Analog.x1)<DEAD_ZONE) return 0;
	else return mJoy2Analog.x1;
}
int joy2y1(){
	if(abs(mJoy2Analog.y1)<DEAD_ZONE) return 0;
	else return mJoy2Analog.y1;
}
int joy2x2(){
	if(abs(mJoy2Analog.x2)<DEAD_ZONE) return 0;
	else return mJoy2Analog.x2;
}
int joy2y2(){
	if(abs(mJoy2Analog.y2)<DEAD_ZONE) return 0;
	else return mJoy2Analog.y2;
}
#else
#warn "Joystick driver included twice"
#endif /* end of include guard: JOYSTICK_DRIVER_LOADED */
