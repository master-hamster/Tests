
/*
ВЩ 2010-02-25
Тестирование аналоговой кнопки
Логика теста:
Создается аналоговая кнопка с несколькими уровнями сигнала.
В зависимости от нажатой кнопки генерируется соответствующее событие,
в соответствии с которым запускается таймер на определенное количество секунд писка
*/
#include <OneWire.h>

#include <DallasTemperature.h>

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
    virtual void parseEvent();
      int buttonID;   //тестовая кнопка для разного
      
      int timerID;
      int beeperID;
// собственно объекты      
      EButton button;

      MyBeeper beeper;
      ETimer timer;
};               



void MyApplication::init()
{
  //привязка к пинам и инициализация соответствующих устройств 
  buttonID=button.init(BUTTONPIN,true,true);
  addObject(&button);
  
  beeperID=beeper.init(BEEPERPIN);
  addObject(&beeper);

  timerID=timer.init(10000,evNoMotion,true);
  addObject(&timer);
  
};

void MyApplication::parseEvent()
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

MyApplication mainApp;   //основное приложение


void setup()
{
   Serial.begin(9600);
   Serial.println("FSM_Hall Loading..");

   mainApp.init();
   mainApp.printNames();

   Serial.println("Loading done!");
};


void loop()
{
  if (mainApp.getEvent()) {  //получаем событие
    mainApp.printEvent();
    mainApp.parseEvent();
    mainApp.handleEvent();
  }
  mainApp.idle();
};

