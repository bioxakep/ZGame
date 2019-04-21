import http.requests.*;
boolean game_started = false;
int[] gTimes = new int[10];
int total_scores = 0;
String alpha = "ABCDEFGJHIJKLMNOPQRSTUVWXYZ1234567890";
PFont recordsFont;
int name_len = 3;
int getStateTime = 0;
byte STATE = 0;
byte ENTER_NAME = 1;
byte SHOW_RECORD = 2;
byte SHOW_RESULT = 3;
boolean server_connect = false;
boolean can_send = false;
long lastUpdate = 0;
String server_addr = "http://192.168.0.44:8484/";

void setup()
{
  size(400, 400);
  background(0);
  fill(100);
  stroke(255);
  textSize(20);
  recordsFont = createFont("Arial", 20);
  cmd_name = new char[name_len];
  char_ix = new int[name_len];
  for (int c = 0; c < name_len; c++) cmd_name[c] = '*';
  connectToServer();
  STATE = ENTER_NAME;
}

void draw()
{
  background(0);

  if (server_connect) 
  {
    updateState();
    if (STATE == ENTER_NAME) printName();
    else if (STATE == SHOW_RECORD) showRecord();
    else if (STATE == SHOW_RESULT) showResult();
  } else connectToServer();
}

String toStr(char[] a)
{
  int s = a.length;
  String ret = "";
  for (int i = 0; i < s; i++)
  {
    ret += a[i];
  }
  return ret;
}
