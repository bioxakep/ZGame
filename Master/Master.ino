//           15/APR/2019 late version
//TAC_Master 05/MAR/2019 Beta
//           21/MAR/2019 organized / pins tested
//           02/04/2019 Works until end of game
// First Run Server.py
// Second PowerUp Bridge and Master
// Then run MasterMonitor
// i2c  16x2 display for debug pending

#include <OneWire.h>
#include <ArdCPZ.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

#define PIXELS 3
#define STRIPPIN 26 // wire 24 R(+) BK(-) G(SIG)
#define RSTXCNTRL 3   //RS485 Direction control
#define PIN_CPZ1 28

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the lcd address to 0x27 for a 16 chars and 2 line display
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, STRIPPIN, NEO_GRB + NEO_KHZ800);
ArdCPZ *cpz1;

uint32_t greenColor, redColor;

boolean bridgeConnected = false;
int level = 0;

byte startLevel = 0;
long lastRFIDCheck = 0;
boolean startRFWait = true;

boolean operSkips[16];
boolean gStates[16];
boolean playerGDone[16];

bool ok1 = false;
boolean game = false;
int phoneOUT = 5;  // R+B- WIRE

//int boxIN = 9;
//int boxHD = 36;

int radioIN  = 53; // wire 33
int radioOUT = 51;

int generIN  = 49;  // wire 23
int generOUT = 47;

int meterIN  = 45;  // wire 34
int meterOUT = 43;

int headOUT  = 23; // wire 18 R(+) B(-) G(OUT)
 
int alleyIN  = A14; // wire 20
int alleyOUT = A15;

int fusesIN  = 25; // wire 19
int fusesOUT = 27;

// int door3IN  = 9; 
unsigned long lastSyncTime = 0;

unsigned long fusesSigStart = 0;
unsigned long windowSigStart = 0;

unsigned long fusesSigStop = 0;
unsigned long windowSigStop = 0;

boolean fusesStates[2] = {HIGH, HIGH};
boolean windowStates[2] = {HIGH, HIGH};

int shelfIN  = 37; // wire 21
int shelfOUT = 35;

int crateIN  = 33; // wire 30 W(IN) R(+)B(-)   >>>  previous called MAP
int crateOUT = 31; // wire 30 G(OUT)

int zombiIN  = 41; // wire 27 W(IN) R(+)B(-)
int zombiOUT = 39; // wire 27 G(OUT)

int lightR1  = A11; // 
int lightR2A =  A5; // RELAY 2 WIRE 25 G(+)W(-)
int lightR2B =   7; // TIP PWM
int lightR3A = A13; // SSR 3A
int lightR3B = A12; // SSR 3A

int door1    =  A8; // RELAY 5 WIRE 32 R(+)B(-)
int door2    =  A4; // RELAY 1 WIRE 25 R(+)B(-) 
int crateHD  =  A6; // RELAY 3 WIRE 29 R(+)B(-)
int door3A   =  34; // wire 24 SHDR(PWR/GND)
int door3B   =  32; // WIRE 19 SHIELD(PWR/GND)  R(+)
int door4    =  A7; // RELAY 4 WIRE 26 R(+)B(-)
int gunBox   = A10; // RELAY 7 WIRE 29 R(+) W(-) >>> to be connected
int boxed    =  A9; // RELAY 6 WIRE 35 (+)(-)

int hatchIN  =  9;   // button
int hatchOUT = A11; // RELAY 8 wire 37 SPK roof

int triplIN  =  8;
int triplOUT = 12;

int video1   = A0;
int video2   = A1;
int video3   = A2;
int video4   = A3;

byte radio  =  0;
byte gener1 =  1;
byte gener2 =  2;
byte meter  =  3;
byte code =    4;
byte fuses =   5;
byte door =    6;
byte window =  7;
byte gas =     8;
byte shelf =  9;
byte emp =    10;
byte world = 11;
byte flare = 12;
byte zombie = 13;
byte head =   14;
byte hatch =  15;
byte gCount = 16;
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
