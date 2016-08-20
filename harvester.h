#pragma systemFile
#include "common.h"
#define HARVESTER_LEFT motor[p1c3m1]
#define HARVESTER_RIGHT motor[p1c3m2]
#warn "Rolling goal holder not set"
#define ROLLING_GOAL servo[p2c1s3]
#define RG_OPEN 239
#define RG_CLOSED 59
#define HARVESTER_SPEED 50
// LICENSE ANDREI "HEXAFRACTION" AKHMETOV. ALL UNAUTHORIZED USE STRICTLY FORBIDDEN.
#warn "Ball pusher values not set"
#define HARVESTER_1 servo[p2c2s1]
#define HARVESTER_2 servo[p2c2s2]
#define RELEASE_HACKY_PIECE_OF_CRAP_SINCE_BUILDERS_HAVE_NO_SPATIAL_REASONING servo[p2c2s6]
#define HACKY_CRAP_IN 19
#define HACKY_CRAP_OUT 25

#define HARVEST_ASSIST servo[p2c1s4]
#define ASSIST_ON 77
#define ASSIST_OFF 255
#define ASSIST_TIME 200
int p_harvesterSetting=0;
bool p_tossing=false;
int p_tossTime;
void harvesterOn(){
	p_harvesterSetting=HARVESTER_SPEED;
}

void harvesterOff(){
	p_harvesterSetting=0;
}

void harvesterReverse(){
	p_harvesterSetting=-HARVESTER_SPEED;
}


#define W(x) wait1Msec(x);
#define G(x,y) HARVESTER_1 = x; HARVESTER_2=y;

void grabGoal(){
	ROLLING_GOAL=RG_CLOSED;
}

void releaseGoal(){
	if(!p_tossing){
		ROLLING_GOAL=RG_OPEN;
	}
}

task thrower(){
	grabGoal();
	W(100)
		G(0,255)
		W(200)
		G(0,255)
		W(200)
		G(6,255)
		W(400)
		G(86,255)
		W(400)
		G(184,156)
		W(200)
		G(184,69)
		W(200)
		G(220,69)
		W(200)
		G(220,0)
		W(200)
		G(202,0)
		W(200)
		G(220,0)
		W(200)
		G(202,0)
		W(1200)
		G(220,0)
		W(200)
		G(220,69)
		W(200)
		G(184,69)
		W(300)
		G(184,156)
		W(300)
		G(86,255)
		W(300)
		G(6,255)
		W(300)
		G(0,255)
		p_tossing=false;
}
void harvesterToss(){
	startTask(thrower);
	p_tossing=true;
	p_tossTime=0;
}



#warn "TODO h1,h2 off pos"
#define H1OFF 0
#define H2OFF 255

void initDeCesare(){
	HARVESTER_1=0;
	HARVESTER_2=213;
	releaseGoal();
	servoChangeRate[p2c1s4]=255;
}

void parkDeCesare(){
	HARVESTER_1=0;
	HARVESTER_2=255;
}

int p_assistTime;
bool p_assist=false;
void setAssist(bool assist){
	p_assist=assist;
}
void stowAssist(){
	HARVEST_ASSIST=ASSIST_ON;
}
void pollHarvester(){
	if(!p_tossing){
		HARVESTER_1=H1OFF;
		HARVESTER_2=H2OFF;
		HARVESTER_LEFT=p_harvesterSetting;
		HARVESTER_RIGHT=p_harvesterSetting;
	} else {
		HARVESTER_LEFT=0;
		HARVESTER_RIGHT=0;
	}
	p_assistTime+=TICK;
	if(p_assist){
		HARVEST_ASSIST=ASSIST_ON;
	} else {
		HARVEST_ASSIST=ASSIST_OFF;
	}

}
