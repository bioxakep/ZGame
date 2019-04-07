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
  for (int i = 0; i < gCount; i++)
  {
    if (i < 5) h_off = 0;
    else h_off = r_txt_h;
    fill(orange);
    if (passedGadgets[i] == 5) fill(green);
    else if (passedGadgets[i] == 3) fill(yellow);
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
        if (passedGadgets[i] < 1) 
        {
          passedGadgets[i] = 3;
          long passedTime = int((gameTime-t.getElapsedTime())/1000);
          passedTimes[i] = getTime(hours(passedTime), minutes(passedTime), seconds(passedTime));
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
  text("CMD 1", r1x + mar + 3 * (gadW + off) + gadW/2, r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH/2 + mar);
  text("CMD 2", r1x + mar + 4 * (gadW + off) + gadW/2, r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH/2 + mar);
  if (!prevMouseState && currMouseState)
  {
    if (mouseX > r1x + mar + 3 * (gadW + off) && mouseX < r1x + mar + 3 * (gadW + off) + gadW 
      && mouseY > r1y + r_txt_h + mar + h_off + 3 * (gadH + off) && mouseY < r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH) // first rect
    {
      println("rect1 pressed");
    }
    if (mouseX > r1x + mar + 4 * (gadW + off) && mouseX < r1x + mar + 4 * (gadW + off) + gadW 
      && mouseY > r1y + r_txt_h + mar + h_off + 3 * (gadH + off) && mouseY < r1y + r_txt_h + mar + h_off + 3 * (gadH + off) + gadH) // second rect
    {
      println("rect2 pressed");
    }
  }

  // TIMER DRAW
  long elpsTime = t.getElapsedTime();
  String currTime = getTime(hours(elpsTime), minutes(elpsTime), seconds(elpsTime));
  fill(orange);
  if (t.overtime) 
  {
    currTime = "-" + currTime;
    fill(red); //color red
  }
  textFont(startFont, 30);
  float timerTextW = textWidth("COMMAND: " + command_name);
  textAlign(LEFT);
  text("COMMAND: " + command_name, r3x + r3w - timerTextW - mar, r3y + r3h - mar);
  textFont(digitalFont, 35);
  text("TIME:" + currTime, r3x + mar, r3y + r3h - mar);
  float masterTWidth = textWidth("MASTER: ");
  text("MASTER: ", r3x + mar, r3y + r3h - mar - 40);
  float serverTWidth = textWidth("SERVER: ");
  text("SERVER: ", r3x + mar, r3y + r3h - mar - 80);
  ellipseMode(CENTER);
  if (master_connect) fill(green);
  else fill(red);
  ellipse(r3x + 3 * mar + masterTWidth, r3y + r3h - mar - 52, 25, 25);
  if (server_connect) fill(green);
  else fill(red);
  ellipse(r3x + 3 * mar + masterTWidth, r3y + r3h - mar - 92, 25, 25);
  prevMouseState = currMouseState;

  String fromBridge = getInput(true);
  if (!game_started)
  {
    if (server_connect && command_name.equals("___")) waitName();
    if (master_connect) waitRun(fromBridge);
    else waitMaster(fromBridge);
  } else
  {
    if (fromBridge.startsWith("BD") && fromBridge.endsWith("FF"))
    {
      for (int i = 0; i < fromBridge.length()-4; i++)
      {
        //print(fromBridge.charAt(i));
        int data = Integer.parseInt(String.valueOf(fromBridge.charAt(i+2)));
        if (data == 5)
        {
          passedGadgets[i] = 5;
          print("Gadget ");
          print(i);
          println(" done by player");
          long passedTime = int((gameTime-t.getElapsedTime())/1000);
          passedTimes[i] = getTime(hours(passedTime), minutes(passedTime), seconds(passedTime));
        }
      }
    } else if (fromBridge.equals("masterConnected")) resetStates();
  }

  if (sendToBridge)
  {
    print("...sending to bridge...");
    arduino.write("CD");
    for (int s = 0; s < gCount; s++)
    {
      arduino.write(operPressed[s]?"3":"1");
      operPressed[s] = false;
    }
    arduino.write("FF\n");
    println("OK");
    sendToBridge = false;
  }
}


/*
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
 
 */
