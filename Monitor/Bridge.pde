void bridgeConnect()
{
  bridge_connect = false;
  print("Connecting to Bridge: ");
  try { 
    arduino = new Serial(this, portName, 38400);
    long startConnect = millis();
    while (!bridge_connect && (millis() - startConnect < 60000))
    {
      arduino.write("startMonitor\n");
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
  //text("WAIT MASTER CONNECT", scrW/2 - scrW/6, scrH/2 + scrH/4);
  if (data.equals("masterConnected"))
  {
    master_connect = true;
    resetGame();
    println("Master Connected OK");
  }
}

void waitRun(String data)
{
  fill(orange);
  textFont(startFont, 24);
  textAlign(CENTER);
  //text("READY TO START", scrW/2 - scrW/6, scrH/2 + scrH/4);
  if (data.equals("Rungame"))
  {
    game_started = true;
    t.start();
    last_passed_time = 0;
    println("Running game");
    sendStart(); // DEBUG
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
