#include <OneWire.h>

#include <DallasTemperature.h>

#include <EAButton.h>
#include <EAEventButton.h>
#include <EAnalogInput.h>
#include <EBeeper.h>
#include <EButton.h>
#include <ECandle.h>
#include <ELED.h>
#include <ERGBLED.h>
#include <EThermo.h>
#include <ETimer.h>
#include <Events.h>

/*
MH 2013-01-20
Test ERGBLed

Change state and colors of RGB LED according to timers

One timer generate events for blinking
Another timer generate events for color changing

*/


//#include <Arduino.h>
#include <ETimer.h>
#include <Events.h>
#include <ERGBLED.h>


#define evTimeToToggle      121
#define evTimeToChangeColor 122

#define TOGGLE_DELAY  250
#define CHANGE_COLOR_DELAY 1000

//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	int parseEvent();

	// wakeup timer
	oid_t toggleTimerID;
	oid_t changeTimerID;
	ETimer toggleTimer;
	ETimer changeTimer;

};
        
void MyApplication::init()
{

	toggleTimerID = toggleTimer.init(TOGGLE_DELAY,evTimeToToggle,true);
	addObject(&toggleTimer);
	changeTimerID = changeTimer.init(CHANGE_COLOR_DELAY,evTimeToChangeColor,true);
	addObject(&changeTimer);

};

int MyApplication::parseEvent()
{
	if (currentEvent.eventType == evTimeToToggle) {
		//We've got toggleEvent - lets toggle LED
		Serial.print("Toggle:");
		return 1;
	}
	if (currentEvent.eventType == evTimeToChangeColor) {
		//We've got toggleEvent - lets change LED color
		Serial.print("Change Color:");
		return 1;
	}
  return 0;
};

//====================================================END OF MyApp definition
MyApplication mainApp;

void setup()
{
	Serial.begin(9600);
	Serial.println("Test_ERGBLED Loading..");

 
	mainApp.init();
	Serial.println("Current objects:");
	mainApp.printNames();

	Serial.println("Loading done!");
};


void loop()
{ 
	if (mainApp.getEvent()) {  
		mainApp.printEvent();
		mainApp.parseEvent();
		mainApp.handleEvent();
	}
	mainApp.idle();
};

    



 

