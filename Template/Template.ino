/*
MH 2013-01-07
4Template


*/


//#include <Arduino.h>
#include <ETimer.h>
#include <Events.h>


//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	int parseEvent();

	// wakeup timer
	oid_t timerID;
	ETimer timer;

};
        
void MyApplication::init()
{

	// case 1. passive thermometer, answer only when someone ask
	timerID = timer.init(MEASURE_DELAY,evTimeToGetTemperature,true);

	addObject(&timer);
#endif
};

int MyApplication::parseEvent()
{
#ifdef CASE1
	if (currentEvent.eventType==evTimeToGetTemperature) {
		//It's time to know the temperature - create info request
		pushEvent(evTellMe,thermometerID);
		return 1;
	}
#endif

	if (currentEvent.eventType==evTemperature) {
		//We've got temperature event - let's print data
		Serial.print("RESULT Temp:");
		Serial.print(currentEvent.eventData);
		Serial.print(" and again:");
		Serial.println(float(currentEvent.eventData) / 100);
		return 1;
	}
  return 0;
};

//====================================================END OF MyApp definition
MyApplication mainApp;

void setup()
{
	Serial.begin(9600);
	Serial.println("Test_ Loading..");

 
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

    



 

