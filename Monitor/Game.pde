void playGame()
{
  currMouseState = mousePressed;
  background(orange);
  strokeWeight(3);
  stroke(orange);
  fill(grey);
  rect(r1x, r1y, r1w, r1h); // room 1 rect
  rect(r2x, r2y, r2w, r2h); // room 2 rect
  rect(r3x, r3y, r3w, r3h); // room 3 rect
  fill(orange);
  textFont(startFont, 19);
  textAlign(LEFT);
  text("ROOM 1", r1x + mar, r1y + r_txt_h);
  text("ROOM 2", r2x + mar, r2y + r_txt_h);
  text("ROOM 3", r3x + mar, r3y + r_txt_h);
  //draw gadget rect
  fill(grey); // orange
  strokeWeight(0);
  textAlign(CENTER);
  textFont(gadFont, 20);
  for (int i = 0; i < gCount-2; i++)
  {
    if (i < 5) h_off = 0;
    else h_off = r_txt_h;
    fill(orange);
    if (passed_gadgets[i] == 5) fill(green);
    else if (passed_gadgets[i] == 3) fill(yellow);
    rect(r1x + mar + (i%5) * (gadW + off), r1y + r_txt_h + mar + h_off + (i/5) * (gadH + off), gadW, gadH); // gadget rects
    fill(grey);
    stroke(grey);
    strokeWeight(1);
    line(r1x + mar + (i%5) * (gadW + off), r1y + r_txt_h + mar + h_off + (i/5) * (gadH + off) + gadH - th, 
      r1x + mar + (i%5) * (gadW + off) + gadW, r1y + r_txt_h + mar + h_off + (i/5) * (gadH + off) + gadH - th); // gadget time line

    strokeWeight(0);
    textFont(gadFont, 20);
    text(gadgetNames[i], r1x + mar + (i%5) * (gadW + off) + gadW/2, r1y + r_txt_h + mar + h_off + (i/5) * (gadH + off) + gadH/2 + mar);
    textAlign(CENTER);
    textFont(gadFont, 17);
    if (passedTimes[i].length() > 0) text(passedTimes[i], r1x + mar + (i%5) * (gadW + off) + gadW/2, r1y + r_txt_h + mar + h_off + (i/5) * (gadH + off) + gadH - mar); // passed time text
    if (!prevMouseState && currMouseState)
    {
      if (game_started && mouseX > r1x + mar + (i%5) * (gadW + off) && mouseX < r1x + mar + (i%5) * (gadW + off) + gadW 
        && mouseY > r1y + r_txt_h + mar + h_off + (i/5) * (gadH + off) && mouseY < r1y + r_txt_h + mar + h_off + (i/5) * (gadH + off) + gadH)
      {
        if (passed_gadgets[i] < 1)
        {
          passed_gadgets[i] = 3;
          long now = t.getPassedTime();
          long passedTime = now - last_passed_time;
          last_passed_time = now;
          passedTimes[i] = getTime(hours(passedTime), minutes(passedTime), seconds(passedTime));
          gTimes[i] = int(passedTime/1000);
          print("Gadget "); 
          print(i); 
          print(" skipped at ");
          print(passedTimes[i]);
        }
        operPressed[i] = true;
        sendToBridge = true;
      }
    }
  }

  // SYSTEM RECTS
  fill(orange);
  rect(r1x + mar + 3 * (gadW + off), r1y + r_txt_h + mar + h_off + 3 * (gadH + off), gadW, gadH); // gadget rects
  rect(r1x + mar + 4 * (gadW + off), r1y + r_txt_h + mar + h_off + 3 * (gadH + off), gadW, gadH); // gadget rects
  fill(grey);
  text("HEAD", r1x + mar + 3 * (gadW + off) + gadW/2, r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH/2 + mar);
  text("HATCH", r1x + mar + 4 * (gadW + off) + gadW/2, r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH/2 + mar);
  if (!prevMouseState && currMouseState)
  {
    if (mouseX > r1x + mar + 3 * (gadW + off) && mouseX < r1x + mar + 3 * (gadW + off) + gadW 
      && mouseY > r1y + r_txt_h + mar + h_off + 3 * (gadH + off) && mouseY < r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH) // first rect
    {
      if (passed_gadgets[14] < 1) passed_gadgets[14] = 3;
      print("Gadget HEAD skipped");
      operPressed[14] = true;
      sendToBridge = true;
    }
    if (mouseX > r1x + mar + 4 * (gadW + off) && mouseX < r1x + mar + 4 * (gadW + off) + gadW 
      && mouseY > r1y + r_txt_h + mar + h_off + 3 * (gadH + off) && mouseY < r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH) // second rect
    {
      if (passed_gadgets[15] < 1) passed_gadgets[14] = 3;
      print("Gadget HATCH skipped");
      operPressed[15] = true;
      sendToBridge = true;
    }
  }

  // TIMER DRAW
  long elpsTime = t.getElapsedTime();
  if (game_over) elpsTime = t.getPassedTime();
  String currTime = getTime(hours(elpsTime), minutes(elpsTime), seconds(elpsTime));
  fill(orange);
  if (t.overtime) 
  {
    currTime = "-" + currTime;
    fill(red); //color red
  }
  textFont(startFont, 30);
  float timerTextW = textWidth("TEAM: " + command_name);
  textAlign(LEFT);
  text("TEAM: " + command_name, r3x + r3w - timerTextW - mar, r3y + r3h - mar);
  textFont(digitalFont, 35);
  text("TIME:" + currTime, r3x + mar, r3y + r3h - mar);
  float masterTWidth = textWidth("MASTER: ");
  text("MASTER: ", r3x + mar, r3y + r3h - mar - 40);
  //float serverTWidth = textWidth("SERVER: ");
  text("SERVER: ", r3x + mar, r3y + r3h - mar - 80);
  text("STATUS: ", r3x + mar, r3y + r3h - mar - 120);
  float status_width = textWidth("STATUS: ");
  if(!master_connect || !server_connect) text("CONNECTING", r3x + 2*mar + status_width, r3y + r3h - mar - 120);
  else
  {
    if(game_started) 
    {
      if(game_over) text("GAME OVER", r3x + 2*mar + status_width, r3y + r3h - mar - 120);
      else text("PLAYING", r3x + 2*mar + status_width, r3y + r3h - mar - 120);
    }
    else 
    {
      if(command_name == "___") text("WAIT TEAM NAME", r3x + 2*mar + status_width, r3y + r3h - mar - 120);
      else text("WAIT MASTER START", r3x + 2*mar + status_width, r3y + r3h - mar - 120);
    }
  }
  ellipseMode(CENTER);
  if (master_connect) fill(green);
  else fill(red);
  ellipse(r3x + 3 * mar + masterTWidth, r3y + r3h - mar - 52, 25, 25);
  if (server_connect) fill(green);
  else fill(red);
  ellipse(r3x + 3 * mar + masterTWidth, r3y + r3h - mar - 92, 25, 25);
  prevMouseState = currMouseState;

  String fromBridge = getInput(false);
  if (!game_started)
  {
    if (server_connect && command_name.equals("___")) waitName();
    else if(!server_connect) serverConnect();
    if (master_connect) waitRun(fromBridge);
    else waitMaster(fromBridge);
  } else
  {
    if (fromBridge.startsWith("BD") && fromBridge.endsWith("FF"))
    {
      for (int i = 0; i < fromBridge.length() - 4; i++)
      {
        //print(fromBridge.charAt(i));
        int data = Integer.parseInt(String.valueOf(fromBridge.charAt(i+2)));
        if (data == 5 && passed_gadgets[i] < 3)
        {
          passed_gadgets[i] = 5;
          long now = t.getPassedTime();
          long passedTime = now - last_passed_time;
          last_passed_time = now;
          passedTimes[i] = getTime(hours(passedTime), minutes(passedTime), seconds(passedTime));
          gTimes[i] = int(passedTime/1000);
          print("Gadget "); 
          print(i);
          print(" done by player at ");
          print(passedTimes[i]);
          print(", last_passed_time="); 
          println(str(last_passed_time));
        }
      }
    } else if (fromBridge.equals("masterConnected")) resetGame();
    else if (fromBridge.equals("Stopgame"))
    {
      t.stop();
      println("Stop Game");
      sendEnd();
    }
  }

  if (sendToBridge)
  {
    arduino.write("CD");
    for (int s = 0; s < gCount; s++)
    {
      arduino.write(operPressed[s]?"3":"1");
      operPressed[s] = false;
    }
    arduino.write("FF\n");
    sendToBridge = false;
    println(", sent to master");
  }
}

void resetGame()
{
  t = new StopWatchTimer();
  gameTime = t.setStartTime(1, 0, 0);
  game_started = false;
  for (int g = 0; g < gCount; g++)
  {
    passed_gadgets[g] = 0;
    operPressed[g] = false;
  }
  for (int j = 0; j < gCount-2; j++)
  {
    passedTimes[j] = "";
    gTimes[j] = 0;
  }
  command_name = "___";
  game_over = false;
  server_connect = false;
  serverConnect();
}
