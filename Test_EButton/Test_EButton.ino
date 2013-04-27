/*
Test_EButton

Simple EButton object


Button press generate event.
Every generated event will be printed at Serial.

If you #define TEST_USER_EVENTS, Button will be generate user-defined event
on single and double press, so you will see state changes according to button action
 - single press - mode change to next
  - double press - mode change to 'LightOff'

*/

//#include <Arduino.h>
//#include <EAButton.h>
//#include <EAEventButton.h>
//#include <EBeeper.h>
#include <EButton.h>
//#include <ERGBLED.h>
#include <Events.h>
#include <EApplication.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define TEST_USER_EVENTS


//Inputs
//Digital Inputs
#define STATE_BUTTON_PIN      2

#define evChangeState         131
#define evLightOff            132

enum BoxState {
        bsLightOff,
        bsLowLight,
        bsFullLight
};
 


//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	void parseEvent();

        void setLightOff();
        void setLowLight();
        void setFullLight();
        void setNextState();
        
        oid_t stateButtonID;

        EButton stateButton;
private:
        BoxState currentState;
};
        
void MyApplication::init()
{
        stateButtonID   = stateButton.init( STATE_BUTTON_PIN, true);   
#ifdef TEST_USER_EVENTS        
            stateButton.setEvents( evChangeState, evLightOff );
#endif
	addObject( &stateButton );
        currentState = bsLightOff;
};

void MyApplication::parseEvent()
{
        switch ( currentEvent.eventType ){
	case evChangeState:
                setNextState();
                break;
        case evLightOff:
                setLightOff();
                break;
	}
};

void MyApplication::setNextState()
{
#ifdef DEBUG_TEST_EBUTTON
        Serial.print("Change mode from ");
        Serial.println(currentState);
#endif          
        switch ( currentState ) {
        case bsLightOff: 
                setLowLight();
                break;
        case bsLowLight:
                setFullLight();
                break;
        case bsFullLight:
                setLightOff();        
                break;
        }        
}

void MyApplication::setLightOff()
{
        currentState = bsLightOff;
        Serial.println( " bsLightOff" );
};

void MyApplication::setLowLight()
{
        currentState = bsLowLight;
        Serial.println( " bsLowLight" );
};

void MyApplication::setFullLight()
{
        currentState = bsFullLight;
        Serial.println( " bsFullLight" );
};


//====================================================END OF MyApp definition
MyApplication mainApp;

void setup()
{
	Serial.begin(9600);
	Serial.println( "Test_EButton Loading.." );
 
	mainApp.init();
	Serial.println( "Current objects:" );
	mainApp.printNames();
	Serial.println( "Loading done!" );

};


void loop()
{ 
	if ( mainApp.getEvent() ) {  
		mainApp.printEvent();
		mainApp.parseEvent();
		mainApp.handleEvent();
	}
	mainApp.idle();
};

    



 

