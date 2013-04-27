
/*

Event Library
MH 2013-01-07

Test Timer Application

*/

#include <DallasTemperature.h>

#include <OneWire.h>

#include <EAButton.h>
#include <EAEventButton.h>
#include <EAnalogInput.h>
#include <EApplication.h>
#include <EBeeper.h>
#include <EButton.h>
#include <ECandle.h>
#include <ELED.h>
#include <ERGBLED.h>
#include <EThermo.h>
#include <ETimer.h>
#include <Events.h>






Timer timer1; // simple timer with no autorestart, autostart
Timer timer2; // simple timer with no autorestart, autostart
Timer timer3; // simple timer with no autorestart, no autostart

bool timer1Expired = false;
bool timer2Expired = false;
bool timer3Expired = false;

void setup()
{
        timer1.init( 5000, TIMER_AUTORESTART, TIMER_AUTOSTART );
        timer2.init( 13000, TIMER_NO_AUTORESTART, TIMER_AUTOSTART ); 
        timer3.init( 2000, TIMER_NO_AUTORESTART, TIMER_NO_AUTOSTART ); 

	Serial.begin(9600);
	Serial.println("Test_Timer Loading.");
        Serial.println ("Timer 1 5s, autorestart, autostart");
        Serial.println ("Timer 2 13s, no_autorestart, autostart");
        Serial.println ("Timer 2 2s, no_autorestart, no_autostart, started by end of timer1 ");
        
};


void loop()
{ 
        if ( timer1.expired() ) {
                if ( !timer1Expired ) {
                        timer1Expired = true;
                        Serial.print ( millis() / 1000 );
                        Serial.println(" - timer1 expired!");
                        timer3.start();
                }
        }  else {
                if ( timer1Expired ) {
//                        Serial.print ( millis() / 1000 );
                        timer1Expired = false;
//                        Serial.println(" - timer1 running!");
                }
        }        

        if ( timer2.expired() ) {
                if ( !timer2Expired ) {
                        timer2Expired = true;
                        Serial.print ( millis() / 1000 );
                        Serial.println(" - timer2 expired!");
                }
        }  else {
                if ( timer2Expired ) {
                        timer2Expired = false;
//                        Serial.print ( millis() / 1000 );
//                        Serial.println(" - timer2 running!");
                }
        }        

        if ( timer3.expired() ) {
                if ( !timer3Expired ) {
                        timer3Expired = true;
                        Serial.print ( millis() / 1000 );
                        Serial.println(" - timer3 expired!");
                }
        }  else {
                if ( timer3Expired ) {
                        timer3Expired = false;
                        Serial.print ( millis() / 1000 );
                        Serial.println(" - timer3 running!");
                }
        }        
                
};

    



 

