import http.requests.*;
boolean game_started = false;
int[] gTimes = new int[10];
int total_scores = 0;
String alpha = "ABCDEFGJHIJKLMNOPQRSTUVWXYZ1234567890";
char[] name;
int[] char_ix;
int char_pos = 0;
int name_len = 3;
int getStateTime = 0;
byte STATE = 0;
byte SHOW_STAT = 1;
byte ENTER_NAME = 2;
byte PLAYING = 3;
boolean server_connect = false;
String server_addr = "http://127.0.0.1:8484/";
void setup()
{
  size(400, 400);
  background(0);
  fill(100);
  stroke(255);
  textSize(20);
  name = new char[name_len];
  char_ix = new int[name_len];
  for (int c = 0; c < name_len; c++) name[c] = '*';
  STATE = SHOW_STAT;
  connectToServer();
}

void draw()
{
  background(0);
  if (STATE == SHOW_STAT || STATE == PLAYING) showStat();
  else if (STATE == ENTER_NAME) enterName();
  if (millis() - getStateTime > 1000) 
  {
    STATE = getState();
    getStateTime += 1000;
    //println("STATE = " + (STATE == SHOW_STAT ? "SHOWSTAT":(STATE == PLAYING?"PLAYING":"ENTERNAME")));
  }
}

void connectToServer()
{
  if (!server_connect)
  {
    GetRequest testGet = new GetRequest(server_addr+"test");
    testGet.send();
    while (!server_connect)
    {
      String resp = testGet.getContent();
      if (resp.equals("OK")) server_connect = true;
      else testGet.send();
    }
    println("Server connected");
  }
  else println("Server connected");
}

byte getState()
{
  if (server_connect)
  {
    GetRequest testGet = new GetRequest(server_addr+"getstate");
    testGet.send();
    String resp = testGet.getContent();
    if (resp.equals("WAITNAME")) return ENTER_NAME;
    if (resp.equals("PLAYING")) return PLAYING;
    else return SHOW_STAT;
  } else return SHOW_STAT;
}

void sendName(String commandName)
{
  PostRequest startPost = new PostRequest(server_addr+"sendname");
  println("Sending command name: " + commandName + " to server...");
  startPost.addData("cname", commandName);
  startPost.send();
  String resp = startPost.getContent();
  println("Server Response: " + resp);
  if (resp.equals(commandName)) 
  {
    game_started = true;
    for (int n = 0; n < 3; n++) name[n] = '*';
  }
}

void keyPressed()
{
  //println(keyCode);
  // 37 - left, 38 - up, 39 - right, 40 - down
  if (STATE == ENTER_NAME)
  {
    if (keyCode == 37) char_pos = max(char_pos-1, 0);
    if (keyCode == 39) char_pos = min(char_pos+1, 2);
    if (keyCode == 40) 
    {
      char_ix[char_pos] = max(0, char_ix[char_pos]-1);
      name[char_pos] = alpha.charAt(char_ix[char_pos]);
    }
    if (keyCode == 38) 
    {
      char_ix[char_pos] = min(alpha.length()-1, char_ix[char_pos]+1);
      name[char_pos] = alpha.charAt(char_ix[char_pos]);
    }
    if (keyCode == 10) sendName(toStr(name));
  }
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

void enterName()
{
  float offX = 10+textWidth("NAME: ");
  textAlign(LEFT);
  text("NAME: ", 10, height/2);
  float passWidth = 0.0;
  float tri_hei = 2;
  float char_h = 20;
  for (int i = 0; i < name_len; i++)
  {
    text(name[i], offX + passWidth, height/2);
    if (char_pos == i)
    {
      line(offX + passWidth, height/2 - char_h, offX + passWidth + textWidth(name[i])/2, height/2 - char_h - tri_hei);
      line(offX + passWidth + textWidth(name[i])/2, height/2 - char_h - tri_hei, offX + passWidth + textWidth(name[i]), height/2 - char_h);
      line(offX + passWidth, height/2 + 3, offX + passWidth + textWidth(name[i])/2, height/2 + 5);
      line(offX + passWidth + textWidth(name[i])/2, height/2 + 5, offX + passWidth + textWidth(name[i]), height/2 + 3);
    }
    passWidth += textWidth(name[i]);
  }
}

void showStat()
{
  fill(255);
  textAlign(CENTER);
  text("Show scores while playing...",width/2,height/2);
  //
}
