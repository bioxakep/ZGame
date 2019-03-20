import http.requests.*;
boolean game_started = false;
int[] gTimes = new int[10];
int total_scores = 0;
int getStateTime = 0;
byte STATE = 0;
byte WAIT_START = 1;
byte WAIT_NAME = 2;
byte PLAYING = 3;
boolean server_connect = false;
boolean prevMouseState = false;
String command_name = "";
void setup()
{
  size(400, 400);
  background(0);
  fill(100);
  stroke(255);
  textSize(20);
  STATE = WAIT_START;
  String uns = "";
  while (!uns.equals("OK"))
  {
    GetRequest rstGet = new GetRequest("http://127.0.0.1:8484/rst");
    rstGet.send();
    uns = rstGet.getContent();
  }
}

void draw()
{
  background(0);
  if (STATE == WAIT_START)
  {
    fill(100);
    rect(width/4, height/4, width/2, height/2);
    fill(255);
    textAlign(CENTER);
    text("Press to init game", width/2, height/2);
    if (mousePressed && !prevMouseState)
    {
      if (mouseX > width/4 && mouseY > height/4 && mouseX < 3*width/4 && mouseY < 3*height/4)
      {
        while (STATE != WAIT_NAME) initGame();
        println("Server waiting command name from ViewMonitor");
      }
    }
    prevMouseState = mousePressed;
  } else if (STATE == WAIT_NAME) 
  {
    fill(255);
    text("Waiting name from players...", width/2, height/2);
    if (millis() - getStateTime > 1000)
    {
      getName();
      getStateTime += 1000;
    }
  } else if (STATE == PLAYING)
  {
    // playgame
    fill(color(20, 200, 10));
    rect(0, height/4, width, height/2);
    fill(255);
    text("Playing with command:" + command_name, width/2, height/2);
    if (mousePressed && !prevMouseState)
    {
      if (mouseX > 0 && mouseY > height/4 && mouseX < width && mouseY < 3*height/4)
      {
        total_scores = 4000;
        for (int i = 0; i < 10; i++) gTimes[i] = 3;
        while (STATE != WAIT_START) endGame();
        println("Server waiting command name from ViewMonitor");
      }
    }
    prevMouseState = mousePressed;
  }
}

void connectToServer()
{
  if (!server_connect)
  {
    GetRequest testGet = new GetRequest("http://127.0.0.1:8484/test");
    testGet.send();
    while (!server_connect)
    {
      String resp = testGet.getContent();
      if (resp.equals("OK")) server_connect = true;
      else testGet.send();
    }
    println("Server connected");
  } else println("Server connected");
}

void initGame()
{
  PostRequest initPost = new PostRequest("http://127.0.0.1:8484/initgame");
  initPost.addData("password", "master");
  initPost.send();
  String resp = initPost.getContent();
  if (resp.equals("OK")) STATE = WAIT_NAME;
  else STATE = WAIT_START;
}

void getName()
{
  GetRequest nameGet = new GetRequest("http://127.0.0.1:8484/getname");
  nameGet.send();
  String uns = nameGet.getContent();
  if (uns.indexOf("not set") < 0) 
  {
    command_name = uns;
    println("Start playing the game with command:" + uns);
    STATE = PLAYING;
  }
}

void startGame(String commandName)
{
  PostRequest startPost = new PostRequest("http://127.0.0.1:8484/startgame");
  println("Try start game with command name: " + commandName);
  startPost.addData("cname", commandName);
  startPost.send();
  String resp = startPost.getContent();
  println("Server Response: " + resp);
  if (resp.equals(commandName)) game_started = true;
}

void endGame()
{
  println("Sending game results to server...");
  PostRequest endPost = new PostRequest("http://127.0.0.1:8484/endgame");
  endPost.addData("gdata", join(nf(gTimes, 0), ","));
  endPost.addData("scores", str(total_scores));
  endPost.send();
  String resp = endPost.getContent();
  println("Server Response: " + resp);
  if (resp.equals("game stop")) 
  {
    game_started = false;
    STATE = WAIT_START;
  }
}

void keyPressed()
{
  println(keyCode);
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

void showStat()
{
  // write this later
  //
}
