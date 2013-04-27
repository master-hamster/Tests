/*
MH 2013-01-05
EThermo testing.

Two modes of EThermo use:
 case 1. 
 passive thermometer, answer only when someone ask using evTellMe 
 Timer create evTellme every 5s, eThermo answers with data
 case 2. active: thermometer push event with temperature every 5s
 
Program works only with first sensor on 1wire bus

https://github.com/master-hamster/Events
*/

// define CASE1 if we test case 1, case2 otherwise
#define CASE1

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ETimer.h>
#include <Events.h>
#include <EThermo.h>
#include <EApplication.h>

#define evTimeToGetTemperature 211
// 1wire is plugged into port 7
#define ONE_WIRE_BUS 7
// Measurement cycle delay, ms
#define MEASURE_DELAY 5000

// Setup a oneWire instance
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensor(&oneWire);


//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	void parseEvent();

#ifdef CASE1
	// wakeup timer
	oid_t timerID;
	ETimer timer;
#endif

	oid_t thermometerID; 
	EThermoDs1820 thermometer;
};
        
void MyApplication::init()
{

#ifdef CASE1
	// case 1. passive thermometer, answer only when someone ask
	timerID = timer.init(MEASURE_DELAY,evTimeToGetTemperature,true);
	thermometerID = thermometer.init(&sensor,0,11);	 

	addObject(&timer);
#else	
	// case 2. active: thermometer push event with temperature
	thermometerID=thermometer.init(&sensor,MEASURE_DELAY,11);	 
#endif
	addObject(&thermometer);
};

void MyApplication::parseEvent()
{
#ifdef CASE1
	if (currentEvent.eventType == evTimeToGetTemperature) {
		//It's time to know the temperature - create info request
		pushEvent(evTellMe, thermometerID);
	}
#endif

	if (currentEvent.eventType == evTemperature) {
		//We've got temperature event - let's print data
		Serial.print("RESULT Temp:");
		Serial.print(currentEvent.eventData);
		Serial.print(" and again:");
		Serial.println(float(currentEvent.eventData) / 100);
	}
};	 

//====================================================END OF MyApp definition
MyApplication mainApp;	


void setup()
{
	uint8_t deviceCount;
	DeviceAddress deviceAddress;
	Serial.begin(9600);
	Serial.println("Test_EThermo18B20 Loading..");

	Serial.print("Locating devices...");
	sensor.begin();
	Serial.print("Found ");
	deviceCount = sensor.getDeviceCount();
	Serial.print(deviceCount, DEC);
	Serial.println(" devices.");
	// Print all devices on 1wire bus
	for (uint8_t n = 0; n < deviceCount; n++) {
		if (!sensor.getAddress(deviceAddress, n)) {
			Serial.print("Unable to find address for Device");
			Serial.println(n);	
			} else {
				Serial.print("Device ");
				Serial.print(n);
				Serial.print(" address: ");
				for (uint8_t i = 0; i < 8; i++) {
				if (deviceAddress[i] < 16) Serial.print("0");
				Serial.print(deviceAddress[i], HEX);
			 }
			 Serial.println();
		}
	}
 
	mainApp.init();
	Serial.println("Current objects:");
	mainApp.printNames();

	Serial.println("Test_Thermo18B20 Loading done!");
};


void loop()
{ 
	if (mainApp.getEvent()) {  
//		mainApp.printEvent();
		mainApp.parseEvent();
		mainApp.handleEvent();
	}
	mainApp.idle();
};

    



 

