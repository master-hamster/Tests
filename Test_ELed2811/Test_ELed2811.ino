/*
Test_ELed2811

Simple EButton object


Button press generate event.
Every generated event will be printed at Serial.

If you #define TEST_USER_EVENTS, Button will be generate user-defined event
on single and double press, so you will see state changes according to button action
 - single press - mode change to next
  - double press - mode change to 'LightOff'

*/

// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1
// #define FORCE_SOFTWARE_SPI 1
#include "FastSPI_LED2.h"

#include <EButton.h>
//#include <ERGBLED.h>
#include <Events.h>
#include <EApplication.h>




#define DEBUG
#ifdef DEBUG
#define DPRINT Serial.print
#define DPRINTLN Serial.println
#else
#define DPRINT(x)
#define DPRINTLN(x)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// test code
//
//////////////////////////////////////////////////

#define NUM_LEDS 16

struct CRGB { byte g; byte r; byte b; };

struct CRGB leds[NUM_LEDS];

// TM1809Controller800Mhz<6> LED;
// WS2811Controller800Mhz<12> LED;
 WS2811Controller800Mhz<21> LED;




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



void setup() {

	LED.init();

#ifdef DEBUG
	int start = millis();
	for(int i = 0; i < 1000; i++){ 
		LED.showRGB((byte*)leds, NUM_LEDS);
		// LED2.showRGB((byte*)leds, NUM_LEDS);
	}
	int end = millis();
	DPRINT("Time for 1000 loops: "); DPRINTLN(end - start);
#endif 

	Serial.println( "Test_EButton Loading.." );
 
	mainApp.init();
	Serial.println( "Current objects:" );
	mainApp.printNames();
	Serial.println( "Loading done!" );


}

int count = 0;
long start = millis();

void loop() { 
    for(int i = 0; i < 3; i++) {
        for(int iLed = 0; iLed < NUM_LEDS; iLed++) {
            memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
            switch(i) { 
	      case 0: leds[iLed].r = 128; break;
	      case 1: leds[iLed].b = 128; break;
	      case 2: leds[iLed].b = 128; break;
	    }
            if(count == 0) { 
		start = millis();
	    } 
            if(count++ == 1000) { 
		count = 0;
		DPRINT("Time for 1000 frames: "); DPRINTLN(millis() - start);
	    }
	    LED.showRGB((byte*)leds, NUM_LEDS);;
            delay(100);
	}
    }
}
