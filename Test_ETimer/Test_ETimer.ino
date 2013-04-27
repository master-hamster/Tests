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

/*
MH 2010-02-10
Тестирование таймера
Логика теста:
По умолчанию раз в 10 секунд таймер кидает событие, на которое бипер отзывается писком
нажатие на кнопку запускает таймер
нажатие на двойную кнопку прекращает работу таймера
длинное нажатие прекращает работу таймера
Для работы должны быть подключены
кнопка - на pin 4
пищалка - на pin 13, встроенный диод подтверждает
*/


#include <OneWire.h>
#include <DallasTemperature.h>


#define evChangeMode    120
#define evSmallBeep     121
#define evNoMotion      122

#define BUTTONPIN       4
#define BEEPERPIN       13


class MyBeeper : public EBeeper {
    virtual int handleEvent(Event& tmpEvent);
};  

class MyApplication : public EApplication {
public:
   void init();
   void parseEvent();
   oid_t buttonID;   //тестовая кнопка для разного
      
   oid_t timerID;
   oid_t beeperID;
// собственно объекты      
   EButton button;
   MyBeeper beeper;
   ETimer timer;
};               


MyApplication mainApp;   //основное приложение


void setup()
{
   Serial.begin(9600);
   Serial.println("Test_ETimer Loading..");

   mainApp.init();
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

void MyApplication::init()
{
  //привязка к пинам и инициализация соответствующих устройств 
  buttonID = button.init(BUTTONPIN,true,true);
  addObject(&button);
  
  beeperID = beeper.init(BEEPERPIN);
  addObject(&beeper);

  timerID = timer.init(10000,evNoMotion,true);
  addObject(&timer);
  
};

void MyApplication::parseEvent()
{
   int oldState;
  // oldState = currentState;


   if ((currentEvent.eventType == evKeyPressed && currentEvent.sourceID == buttonID))
   { //нажатие на кнопку
      pushEvent( evTimerStart,timerID );
   };

   if (currentEvent.eventType == evKeyDoublePressed && currentEvent.sourceID == buttonID) {
	 //двойное нажатие на кнопку
      pushEvent(evTimerStop,timerID);
   }	
   
   if (currentEvent.eventType == evKeyHold && currentEvent.sourceID == buttonID) {
      pushEvent(evTimerStop,timerID);
   };
//   if (currentEvent.eventType==evNoMotion && currentState == 1) {

};    


//===========================================================================
int MyBeeper::handleEvent(Event& tmpEvent)
{
   if (tmpEvent.eventType != evNone) {
      EBeeper::handleEvent(tmpEvent);
      EBeeper::beep();
   };   
};
//===========================================================================
