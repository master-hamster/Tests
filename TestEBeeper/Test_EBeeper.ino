
#include <ETimer.h>
#include <EBeeper.h>
#include <EButton.h>
#include <Event.h>

/*
ВЩ 2010-02-10
Тестирование таймера
Логика теста:
По умолчанию раз в 10 секунд таймер кидает событие, на которое бипер отзывается писком
нажатие на кнопку запускает таймер
нажатие на двойную кнопку прекращает работу таймера
длинное нажатие прекращает работу таймера

*/


#define evChangeMode    120
#define evSmallBeep     121
#define evNoMotion      122

#define BUTTONPIN       4
#define BEEPERPIN       11


class MyBeeper : public EBeeper {
    virtual int handleEvent(Event& tmpEvent);
};  

class MyApplication : public EApplication {
public:
   void init();
   int parseEvent();
   int buttonID;   //тестовая кнопка для разного
      
   int timerID;
   int beeperID;
// собственно объекты      
   EButton button;
   MyBeeper beeper;
   ETimer timer;
};               


MyApplication mainApp;   //main application


void setup()
{
   Serial.begin(9600);
   Serial.println("Test_ETimer Loading...");

   mainApp.init();
   mainApp.printNames();

   Serial.println("Loading done!");
};


void loop()
{
  if (mainApp.getEvent()) {  //get event if exist any
    mainApp.printEvent(); // for debug purpose
    mainApp.parseEvent();
    mainApp.handleEvent();
  }
  mainApp.idle();
};

void MyApplication::init()
{
  //привязка к пинам и инициализация соответствующих устройств 
  // all object initialization
  buttonID=button.init(BUTTONPIN,true,true);
  addObject(&button);
  
  beeperID=beeper.init(BEEPERPIN);
  addObject(&beeper);

  timerID=timer.init(10000,evNoMotion,true);
  addObject(&timer);
  
};

int MyApplication::parseEvent()
{
   int oldState;
  // oldState = currentState;


   if ((currentEvent.eventType==evKeyPressed && currentEvent.sourceID==buttonID))
   { //нажатие на кнопку
      pushEvent(evTimerStart,timerID);
   };

   if (currentEvent.eventType==evKeyDoublePressed && currentEvent.sourceID==buttonID) {
	 //двойное нажатие на кнопку
      pushEvent(evTimerStop,timerID);
   }	
   
   if (currentEvent.eventType==evKeyHold && currentEvent.sourceID==buttonID) {
      pushEvent(evTimerStop,timerID);
   };
//   if (currentEvent.eventType==evNoMotion && currentState == 1) {

};    


//===========================================================================
int MyBeeper::handleEvent(Event& tmpEvent)
{
   if (tmpEvent.eventType!=evNone) {
      EBeeper::handleEvent(tmpEvent);
      EBeeper::beep();
   };   
};
//===========================================================================
