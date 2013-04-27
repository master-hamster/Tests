/*
Test_EChain2811

LED Chain object based on WS2811
Using FastSPI_Led2 Librarys

------------  Wiring  -----------
Board: Arduino Mega
LED String: 16 Leds based on WS2811
Led String PIN: 21

----------- Sketch Description --------
Button press generate event.
Events changing current state of led chain
Every generated event will be printed at Serial.
States are:
  Run - Led chain running 
  Stop - Led chain stopped
  Off - Led Chain Off

*/

// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1
// #define FORCE_SOFTWARE_SPI 1
#include "FastSPI_LED2.h"

#include <EButton.h>
//#include <ERGBLED.h>
#include <Events.h>
#include <EChain.h>
#include <OneWire.h>
#include <EApplication.h>


#define DEBUG
#ifdef DEBUG
#define DPRINT Serial.print
#define DPRINTLN Serial.println
#else
#define DPRINT(x)
#define DPRINTLN(x)
#endif


#define NUM_LEDS 16

struct CRGB { byte g; byte r; byte b; };

struct CRGB leds[NUM_LEDS];

// TM1809Controller800Mhz<6> LED;
// WS2811Controller800Mhz<12> LED;
 WS2811Controller800Mhz<21> LED;

#define TEST_USER_EVENTS

#define STATE_BUTTON_PIN      2

#define evChangeState         131
#define evLightOff            132

enum LedChainState {
        lsRun,
        lsStop,
        lsOn,
        lsOff
};




EChain ledChain;











class MyApplication : public EApplication {
public:
	void init();
	void parseEvent();

        void setLightOff();
        void setLightOn();
        void setLightRun();
        void setLightStop();
        void setNextState();
        
        oid_t stateButtonID;

        EButton stateButton;
private:
        LedChainState currentState;
};
        
void MyApplication::init()
{
        stateButtonID   = stateButton.init( STATE_BUTTON_PIN, true);   
	addObject( &stateButton );

        ledChain.init(0,NUM_LEDS,&leds);
        addObject( &ledChain);
        
        currentState = lsOff;
        
        
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
        case lsRun: 
                setLightRun();
                break;
        case lsStop:
                setLightStop();
                break;
        case lsOff:
                setLightOff();        
                break;
        case lsOn:
                setLightOn();        
                break;
                
        }        
}

void MyApplication::setLightRun()
{
        currentState = lsOff;
        Serial.println( " lsOff" );
};

void MyApplication::setLightStop()
{
        currentState = lsStop;
        Serial.println( " lsStop" );
};

void MyApplication::setLightOn()
{
        currentState = lsOn;
        Serial.println( " lsOn" );
};

void MyApplication::setLightOff()
{
        currentState = lsOff;
        Serial.println( " lsOff" );
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
