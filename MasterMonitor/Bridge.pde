void bridgeConnect()
{
  bridge_connect = false;
  print("Connecting to Bridge: ");
  try { 
    arduino = new Serial(this, portName, 9600);
    long startConnect = millis();
    while (!bridge_connect && (millis() - startConnect < 60000))
    {
      arduino.write("startMonitor\n");
      //println("Connecting...");
      long now = millis();
      while (millis() - now < 1000) {
        ;
      }
      String input = getInput(false);
      if (input.length() > 4)
      {
        if (input.equals("startBridge")) 
        {
          bridge_connect = true;
          println("OK");
        }
      }
    }
  }
  catch (Exception e) {
    println(e);
  }
  if (!bridge_connect) { 
    noLoop();
    println("NO CONNECTION");
  }
}

void waitMaster(String data)
{
  fill(orange);
  textFont(startFont, 24);
  textAlign(CENTER); 
  text("WAIT MASTER CONNECT", scrW/2 - scrW/6, scrH/2 + scrH/4);
  if (data.equals("masterConnected"))
  {
    game_started = false;
    master_connect = true;
    resetStates();
    wait(1);
  }
}

void waitRun(String data)
{
  fill(orange);
  textFont(startFont, 24);
  textAlign(CENTER);
  text("WAIT START FROM MASTER", scrW/2 - scrW/6, scrH/2 + scrH/4);
  if (data.equals("Rungame"))
  {
    game_started = true;
    for (int g = 0; g < gCount; g++)
    {
      passedGadgets[g] = 0;
      operPressed[g] = false;
      passedTimes[g] = "";
    }
    t.start();
    println("Run game");
    sendStart();
  }
}

String getInput(boolean debug)
{
  if (arduino.available() > 0)
  {
    String inp = arduino.readStringUntil('\n');
    if (inp != null)
    {
      if (inp.length() > 1 && debug) println(inp);
      inp = inp.trim();
      return inp;
    } else return " ";
  } else return " ";
}


void resetStates()
{
  t = new StopWatchTimer();
    gameTime = t.setStartTime(1, 20, 0);
    println("Connecting to Master: true");
    for (int g = 0; g < gCount; g++)
    {
      passedGadgets[g] = 0;
      operPressed[g] = false;
    }
}
