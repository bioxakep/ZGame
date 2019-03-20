/*
0. PRE-START   >>> ALL DOORS  (2,3,4) LOCK
1. RFID for start / start button   >>> DOOR 1 LOCK  >>> SEND SIGNAL TO VIDEO PLAYER 1
2. RADIO    >>> DOOR 2 UNLOCK  >>> LIGHTS  1 CONTROL 
3. GENERATOR 1
4. GENERATOR 2  >>> LIGHTS 2 CONTROL  >>> SIGNAL2ALLEY
5. HEAD (SEND SIGNAL ONLY)
6. METER >>> LIGHTS 3 CONTROL  >>> SIGNAL2FUSES
7. FUSES 1. (KEYPAD CODE)   >>> DOOR 3 UNLOCK  
8. FUSES 2. (FUSES LOAD) >>> ACTIVATE DOOR 3 GUARD >>> LIGHT CONTROL 4
9A.  DOOR 3 GUARD ON  (TURN ON FASTLED A)
9B.  ALLEY(WINDOW)   (TURN ON FASTLED B)
9C. SHELF 1   
9D. SHELF 2   (TURN ON FASTLED C)
10. MAP 1 >>> UNLOCK MAPHOLDER
11. MAP 2 >>> SIGNAL TO VIDEO PLAYER 2 >> UNOCK GUNHOLDER 
12. TARGET >>> LIGHT CONTROL 5  >>> SIGNAL TO VIDEO PLAYER 2 >>> TOP LOCK >>> EXIT DOOR
___________________________________________________
RADIO  IN/OUT
GENERATOR IN/OUT (2 SIGNALS FROM)
ALLEY IN/OUT
FUSE IN/OUT (2 SIGNALS FROM)
SHELF IN/OUT (2 SIGNALS FROM)
MAP IN/OUT (2 SIGNALS FROM)
TARGET IN/OUT 
*/

int door1Pin = 7;
int door2Pin = 8;
int door3Pin = 9;
int door4Pin = 10;
int startRFID = 11;
int startBtn = 12;
boolean startStates[2] = {HIGH, HIGH};
unsigned long lastRFIDCheck = 0;
boolean startRFWait = true;
void setup()
{
  pinMode(door1Pin, OUTPUT);
  pinMode(door2Pin, OUTPUT);
  pinMode(door3Pin, OUTPUT);
  pinMode(door4Pin, OUTPUT);
  pinMode(startBtn, INPUT_PULLUP);
  digitalWrite(door1Pin, LOW);
  digitalWrite(door2Pin, LOW);
  digitalWrite(door3Pin, LOW);
  digitalWrite(door4Pin, LOW);
  ArdCPZ *firstRF; // RFIDReader
  
}

void loop()
{
	unsigned long tick = millis();
  if (level == 1)
  {
    if(startGame(tick))
    {
      digitalWrite(door1Pin, LOW);
      //Send Signal to video player//
    }
  }
  elif (level == 2)
  {
    if(radioGadget())
    {
      
    }
  }

}
