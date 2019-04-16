// operMonitor of ZGame
// 02.04.2019 Big Update
import processing.serial.*;
import http.requests.*;

Serial arduino;
String portName;
PrintWriter data;
PFont startFont, gadFont, timerFont, digitalFont;

String[] gadgetNames = {"RADIO", "GEN/FUEL", "GEN/RUN", "METER", "CODE", "FUSES", "DOOR", "WINDOW", "GAS", "SHELF", "E.M.P", "MAP", "FLARE", "ZOMBIE"};
String[] passedTimes = new String[14];
int[] gTimes = new int[14];
byte[] passedGadgets = new byte[16];
boolean[] operPressed = new boolean[16];
boolean game_started = false;
boolean sendToBridge = false;
int total_scores = 0;
int getStateTime = 0;
long lastPassedTime = 0;
int gCount = 16;
String server_addr = "http://192.168.0.39:8484/";

boolean server_connect = false;
boolean bridge_connect = false;
boolean master_connect = false;
boolean prevMouseState = false;
boolean currMouseState = false;
boolean debug = true;
String command_name = "";

float scrH, scrW;
float r1x, r1y, r1w, r1h;
float r2x, r2y, r2w, r2h;
float r3x, r3y, r3w, r3h;

float mar, off, r_txt_h, h_off;
float gadW, gadH, th;

color orange, grey, green, yellow, red;
color darkgreen, bkrd, darkblue;

StopWatchTimer t;
int timerHours = 0;
int timerMinutes = 0;  
int timerSeconds = 0;
long gameTime = 0;
long doneTime = 0;
long server_connect_timeout = 5000;
int startScores = 10000;
int currScores = 10000;
int hintDec = 500;

void setup()
{
  fullScreen();
  //size(1200, 600);
  background(0);
  fill(100);
  stroke(255);
  textSize(20);
  portName = "COM3";//"COM4"; // COM3 or /dev/tty.wchusbserial1410 or /dev/tty.wchusbserial1420
  //portName = "/dev/cu.usbmodem14141";
  bridgeConnect();
  serverConnect();

  startFont = createFont("Arial", 10);//Silom
  gadFont = createFont("Arial", 14); //MyanmarMN
  timerFont = createFont("Arial", 14); //MyanmarMN
  //lcdFont = createFont("LCD.vlw", 24);
  digitalFont = createFont("digital-7.ttf", 24);
  orange = color(220, 150, 65);
  red = color(160, 10, 10);
  grey = color(75, 75, 75);
  green = color(120, 170, 90);
  yellow = color(255, 200, 100);

  bkrd = color(230, 210, 170);
  darkgreen = color(170, 220, 180);
  darkblue = color(140, 200, 250);

  scrW = width;
  scrH = height;
  mar = 5;
  off = 2 * mar;
  h_off = 0;
  r_txt_h = 20;
  gadW = (scrW - mar * 10) / 5;
  gadH = (scrH - 2 * r_txt_h - 2 * mar - 5 * off) / 5;
  r1x = 0;
  r1y = 0;
  r1w = gadW + off;
  r1h = r_txt_h + gadH + 2 * mar;
  r2x = r1x + r1w;
  r2y = 0;
  r2w = 4 * gadW + 4 * off;
  r2h = r1h;
  r3x = 0;
  r3y = r1y + r1h;
  r3w = scrW;
  r3h = scrH - r1h;
  th = 25;
  for (int i = 0; i < gCount; i++) 
  {
    passedGadgets[i] = 0;
    operPressed[i] = false;
  }
  for (int j = 0; j < gCount-2; j++)
  {
    passedTimes[j] = "";
    gTimes[j] = 0;
  }
  t = new StopWatchTimer();
  gameTime = t.setStartTime(1, 0, 0);
  command_name = "___";
}

void draw()
{
  playGame();
}
