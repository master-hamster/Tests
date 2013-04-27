
/*
MH 2013-01-05
EThermo testing.
Based on Test_EThermo18B20

Added LCD Support based on PCD8544 Library

Two modes of EThermo use:
 case 1. 
 passive thermometer, answer only when someone ask using evTellMe 
 Timer create evTellme every 5s, eThermo answers with data
 case 2. active: thermometer push event with temperature every 5s
 
 Arduino        Nokia 5110 Display
3.3V   ------------------ 1-VCC
Ground ------------------ 3-SCE
Reset  ------------------ 4-RST
PIN #5 ----R10k---------- 5-D/C
PIN #4 ----R10k---------- 6-DNK(MOSI) (SDIN)
PIN #3 ----R10k---------- 7-SCLK
 
Program works only with first sensor on 1wire bus

https://github.com/master-hamster/Events

Library:
  Dallas 18B20 Arduino Library from  http://milesburton.com/index.php?title=Dallas_Temperature_Control_Library 
  PCD8544 - Interface with Philips PCD8544 (or compatible) LCDs. by Carlos Rodrigues <cefrodrigues@gmail.com> 
  
*/

// define CASE1 if we test case 1, case2 otherwise
#define CASE1

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ETimer.h>
#include <Events.h>
#include <EThermo.h>
#include <EApplication.h>
#include <PCD8544.h>

#define evTimeToGetTemperature 211
// 1wire is plugged into port 7
#define ONE_WIRE_BUS 7
// Measurement cycle delay, ms
#define MEASURE_DELAY 2000

// Setup a oneWire instance
OneWire oneWire( ONE_WIRE_BUS );
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensor( &oneWire );

static const byte sensorPin = 0;
static const byte ledPin = 13;

// The dimensions of the LCD (in pixels)...
static const byte LCD_WIDTH = 84;
static const byte LCD_HEIGHT = 48;

// The number of lines for the temperature chart...
static const byte CHART_HEIGHT = 5;

// A custom "degrees" symbol...
static const byte DEGREES_CHAR = 1;
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

// A bitmap graphic (10x2) of a thermometer...
static const byte THERMO_WIDTH = 10;
static const byte THERMO_HEIGHT = 2;
static const byte thermometer[] = { 0x00, 0x00, 0x48, 0xfe, 0x01, 0xfe, 0x00, 0x02, 0x05, 0x02,
                                    0x00, 0x00, 0x62, 0xff, 0xfe, 0xff, 0x60, 0x00, 0x00, 0x00};

static PCD8544 lcd;

//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	void parseEvent();
        void printTemperature();
        float currentTemperature;
#ifdef CASE1
	// wakeup timer
	oid_t timerID;
	ETimer timer;
#endif

	oid_t thermometerID; 
	EThermoDs1820 thermometer;
        byte xChart;
};
        
void MyApplication::init()
{
        xChart = LCD_WIDTH;

#ifdef CASE1
	// case 1. passive thermometer, answer only when someone ask
	timerID = timer.init( MEASURE_DELAY, evTimeToGetTemperature, true );
	thermometerID = thermometer.init( &sensor, 0, 11 );	 

	addObject(&timer);
#else	
	// case 2. active: thermometer push event with temperature
	thermometerID = thermometer.init( &sensor, MEASURE_DELAY, 11 );	 
#endif
	addObject( &thermometer );
        currentTemperature = thermometer.getTemperature();
};

void MyApplication::parseEvent()
{
#ifdef CASE1
	if (currentEvent.eventType == evTimeToGetTemperature) {
		//It's time to know the temperature - create info request
		pushEvent( evTellMe, thermometerID );
	}
#endif

	if (currentEvent.eventType == evTemperature) {
		//We've got temperature event - let's print data
                currentTemperature = float(currentEvent.eventData) / 100;
		Serial.print("RESULT Temp:");
		Serial.print( currentEvent.eventData );
		Serial.print(" and again:");
		Serial.println( float( currentTemperature ) );

                printTemperature();
	}
};	

void MyApplication::printTemperature()
{
 
   // Print the temperature (using the custom "degrees" symbol)...
  lcd.setCursor( 0, 0 );
  lcd.print( "Temp: " );
  lcd.print( currentTemperature, 1 );
  lcd.print( " \001C " );
  // Wrap the chart's current position...
  if ( xChart >= LCD_WIDTH ) {
    xChart = THERMO_WIDTH + 2;
  }

  // Update the temperature chart...  
  lcd.setCursor( xChart, 1 );
  lcd.drawColumn( CHART_HEIGHT, map( currentTemperature, 20, 30, 0, CHART_HEIGHT*8 ) );  // ...clipped to the 20-30C range.
  lcd.drawColumn( CHART_HEIGHT, 0 );         // ...with a clear marker to see the current chart position.
  Serial.print(" H=");
  Serial.println (map( currentTemperature*10, 200, 300, 0, CHART_HEIGHT*8 ));
  
  xChart++;
        
};

//====================================================END OF MyApp definition
MyApplication mainApp;	


void setup()
{
	uint8_t deviceCount;
	DeviceAddress deviceAddress;
	Serial.begin(9600);

	Serial.println( "Test_EThermo18B20_LCD Loading.." );

        //Init LCD screen        
        lcd.begin( LCD_WIDTH, LCD_HEIGHT );
        lcd.createChar( DEGREES_CHAR, degrees_glyph );
        // Draw the thermometer sign
        lcd.setCursor( 0, LCD_HEIGHT/8 - THERMO_HEIGHT );
        lcd.drawBitmap( thermometer, THERMO_WIDTH, THERMO_HEIGHT );
	Serial.println( "Test_Thermo18b20_LCD Loading done!" );

	Serial.print("Locating devices...");
	sensor.begin();
	Serial.print("Found ");
	deviceCount = sensor.getDeviceCount();
	Serial.print(deviceCount, DEC);
	Serial.println(" devices.");
	// Print all devices on 1wire bus
	for ( uint8_t n = 0; n < deviceCount; n++ ) {
		if ( !sensor.getAddress( deviceAddress, n ) ) {
			Serial.print("Unable to find address for Device");
			Serial.println(n);	
			} else {
				Serial.print("Device ");
				Serial.print(n);
				Serial.print(" address: ");
				for ( uint8_t i = 0; i < 8; i++ ) {
				if ( deviceAddress[i] < 16 ) Serial.print( "0" );
				Serial.print( deviceAddress[i], HEX );
			 }
			 Serial.println();
		}
	}
 
	mainApp.init();
	Serial.println( "Current objects:" );
	mainApp.printNames();

};


void loop()
{ 
	if ( mainApp.getEvent() ) {  
//		mainApp.printEvent();
		mainApp.parseEvent();
		mainApp.handleEvent();
	}
	mainApp.idle();
 
};

    



 

