void Start(long t)
{
  /*// DEBUG
  if(Serial.available() > 0)
  {
    delay(100);
    while(Serial.available()) Serial.read();
    startRFWait = false;
  }
  // DEBUG*/
  if (t - lastRFIDCheck > 100)
  {
    lastRFIDCheck = t;
    if (startRFWait) startRFWait = !getStartRFID();
  }
  if (!startRFWait)
  {
    if (startLevel == 1)
    {
      digitalWrite(lightR2A, HIGH);
      digitalWrite(lightR2B, HIGH);
      digitalWrite(door4, LOW); // close last door
      digitalWrite(door2, LOW); // close second door
      digitalWrite(lightR3A, LOW);
      digitalWrite(lightR3B, LOW);
      sendHLms(video1, 100);
      sendHLms(video2, 100);
      //mp3Set(1);
      //mp3_play(1);
      delay(15);
      Serial.print("Level 1 Start 2 OK, Level 1 Done");
      strip.setPixelColor(1, 0, 0, 0);
      strip.show();
      sendByte(0xAA);
      delay(300);
      level = 10;
    }
    else if (startLevel == 0)
    {
      digitalWrite(door1, LOW);
      digitalWrite(door2, HIGH);
      digitalWrite(door4, LOW);
      strip.setPixelColor(2, 0, 0, 0);
      strip.show();
      Serial.print("Level 1 Start 1 OK");
      startLevel++;
      delay(300);
    }
    startRFWait = true;
  }
}

void Box()
{
  if ((!digitalRead(boxIN) || operSkips[box]) && !gStates[box])
  {
    if(operSkips[box]) sendHLms(boxHD, 250);
    else playerGDone[box] = true;
    Serial.println("Box OK");
    level = 11;
  }
}

void Radio()
{
  // ROOM 1 "RADIO">>  wait for signal from radioIN  >> if skipped send signal to radioOUT
  //                                                     signal to Video player 3,
  //                                                     mp3_player1 track2
  //                                                     mp3_player2 track2
  //                                                     turn off ligtsR1
  //                                                     turn on  ligtsR2A
  //                                                     turn off phoneOUT
  if ((!digitalRead(radioIN) || operSkips[radio]) && !gStates[radio])
  {
    if (operSkips[radio]) sendHLms(radioOUT, 250);
    else playerGDone[radio] = true;
    sendHLms(video2, 100);
    //mp3Set(1);
    //mp3_play(2);
    //mp3Set(2);
    //mp3_play(2);
    //digitalWrite(lightR1,  LOW);  // OFF
    digitalWrite(phoneOUT, LOW);  // turn off the phone
    ///  events delayed for 25 seconds
    digitalWrite(door2,   HIGH);  //open door 2
    digitalWrite(lightR2A, LOW);  // ON LIGHT under door 2
    digitalWrite(lightR2B, LOW); // OFF LIGHT under door 3
    digitalWrite(lightR3A, LOW); // OFF LIGHT ROOM 3
    digitalWrite(lightR3B, LOW); // OFF LIGHT ROOM 3
    level = 21;
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
    Serial.println("Radio OK");
  }
}

void Generator()
{
  // ROOM 2 "GENERATOR"
  //                   wait for signal from generIN   >> if skipped send signal to generOUT
  //                                                     send signal to headOUT
  if ((!digitalRead(generIN) || operSkips[gener]) && !gStates[gener])
  {
    if (operSkips[gener]) sendHLms(generOUT, 250);
    else playerGDone[gener] = true;
    sendHLms(headOUT, 100);
    Serial.println("Generator OK");
    level = 22;
  }
}

void Meter()
{
  // ROOM 2 "METER"
  //                   wait for signal from meterIN   >> if skipped send signal to meterOUT
  //                                                     signal to Video player 1,
  //                                                     mp3_player1 track3
  //                                                     turn off ligtsR2A
  //                                                     turn on  ligtsR2B
  //                                                     send signal to alleyOUT
  //                                                     the signal from meterIN is permamnet if lost turn off lightsR2B until restored
  if ((!digitalRead(meterIN) || operSkips[meter]) && !gStates[meter])
  {
    if (operSkips[meter]) sendHLms(meterOUT, 250);
    else playerGDone[meter] = true;
    sendHLms(video1, 100);
    //mp3Set(1);
    //mp3_play(3);
    digitalWrite(lightR1, LOW);
    digitalWrite(lightR2B, HIGH);
    sendHLms(alleyOUT, 250);
    Serial.println("Meter OK");
    level = 23;
  }
  if (gStates[meter]) digitalWrite(lightR2B, !digitalRead(meterIN)); // Узнать это после прохождения или вообще
}

void Code()
{
  // ROOM 2 "CODE"
  //                wait for first signal from fusesIN   >> if skipped send signal to fusesOUT
  //                                                     turn off lightR1
  //                                                     turn off door3A
  //                                                     signal to Video player 3,
  //                                                     mp3_player1 track2
  if ((!digitalRead(fusesIN) || operSkips[code]) && !gStates[code])
  {
    if (operSkips[code]) sendHLms(fusesOUT, 250);
    else playerGDone[code] = true;
    digitalWrite(lightR1, LOW);
    digitalWrite(door3A, LOW);
    sendHLms(video3, 100);
    //mp3Set(1);
    //mp3_play(2);
    Serial.println("Code OK");
    level = 30;
  }
}

void Fuses()
{
  // ROOM 3 "FUSES"
  //                wait for second signal from fusesIN  >> if skipped send signal to fusesOUT
  //                                                     signal to Video player 2,
  //                                                     signal to Video player 1,
  //                                                     mp3_player1 track3
  //                                                     turn on lightR3A and lightR3B
  //                                                     turn on neopixel 1-3 red
  if ((!digitalRead(fusesIN) || operSkips[fuses1]) && !gStates[fuses1])
  {
    if (operSkips[fuses1]) sendHLms(fusesOUT, 250);
    else playerGDone[fuses1] = true;
    sendHLms(video1, 100);
    sendHLms(video2, 100);
    //mp3Set(1);
    //mp3_play(3);
    digitalWrite(lightR3A, HIGH);
    digitalWrite(lightR3B, HIGH);
    for (int i = 0; i < 3; i++) strip.setPixelColor(i, strip.Color(0, 200, 0));
    strip.show();
    Serial.println("Fuses1 OK");
    level = 31;
  }
}

void Head()
{

}

void Lock(long t)
{
  // ROOM 3 "LOCK"
  //                wait for permanent signal from fusesIN        >> if recieved turn neopixel 1 green , lock door3B
  //
  //                wait for 3 second long signal from alleyIN    >> if recieved turn neopixel 2 green , signal to video player 2
  //
  //                wait for first signal from shelfIN            >> if skipped send signal to shelfOUT  (gas open)
  //                wait for permanent signal from shelfIN        >> if skipped send signal to shelfOUT  (gas ignited)
  //                                                                 if recieved turn neopixel 3 green , signal to video player 3
  //
  //                if all done level completed

  // permanent signal waiting
  fusesStates[0] = debounce(fusesIN, fusesStates[1]);
  if (!fusesStates[0] && fusesStates[1]) fusesSigStart = t;
  if (!fusesStates[1] && fusesStates[0]) fusesSigStop = t;
  fusesStates[1] = fusesStates[0];
  if ((t - fusesSigStart > 3000 && fusesSigStart - fusesSigStop > 0 || operSkips[fuses2]) && !gStates[fuses2])
  {
    if (!operSkips[fuses2]) playerGDone[fuses2] = true;
    digitalWrite(door3B, HIGH);
    strip.setPixelColor(0, greenColor);
    strip.show();
    Serial.println("Fuses2 OK");
    // What else if skip or player
  }

  alleyStates[0] = debounce(alleyIN, alleyStates[1]);
  if (!alleyStates[0] && alleyStates[1]) alleySigStart = t;
  if (!alleyStates[1] && alleyStates[0]) alleySigStop = t;
  alleyStates[1] = alleyStates[0];
  if ((t - alleySigStart > 3000 && alleySigStart - alleySigStop > 0 || operSkips[alley]) && !gStates[alley])
  {
    if (!operSkips[alley]) playerGDone[alley] = true;
    sendHLms(video2, 100);
    strip.setPixelColor(1, greenColor);
    strip.show();
    Serial.println("Alley OK");
    // What else if skip or player
  }

  if ((!digitalRead(shelfIN) || operSkips[shelf1]) && !gStates[shelf1])
  {
    if (operSkips[shelf1]) sendHLms(shelfOUT, 250);
    else playerGDone[shelf1] = true;
    Serial.println("Shelf1 OK");
  }

  if ((!digitalRead(shelfIN) || operSkips[shelf2]) && !gStates[shelf2] && gStates[shelf1])
  {
    if (operSkips[shelf2]) sendHLms(shelfOUT, 250);
    else playerGDone[shelf2] = true;
    Serial.println("Shelf2 OK");
    strip.setPixelColor(2, greenColor);
    strip.show();
    sendHLms(video3, 100);
  }

  if (gStates[fuses2] && gStates[alley] && gStates[shelf2])
  {
    level = 32;
    Serial.print("Lock OK");
  }
}


void Crate() // Duplicate
{
  // ROOM 3 "MAP" / CRATE
  //                wait for first signal from crateIN   >> if skipped send signal to crateOUT
  //                                                     signal to Video player 4,
  //                                                     signal to Video player 2,
  //                                                     signal to Video player 2,
  //                                                     open crateHD
  if ((!digitalRead(crateIN) || operSkips[crate1]) && !gStates[crate1])
  {
    if (operSkips[crate1]) sendHLms(crateOUT, 250);
    else playerGDone[crate1] = true;
    digitalWrite(crateHD, LOW);
    Serial.println("Crate1 OK");
  }

  if ((!digitalRead(crateIN) || operSkips[crate2]) && !gStates[crate2] && gStates[crate1])
  {
    if (operSkips[crate2]) sendHLms(crateOUT, 250);
    else playerGDone[crate2] = true;
    sendHLms(video4, 100);
    sendHLms(video2, 100);
    sendHLms(video2, 100);
    // Light and video comands
    digitalWrite(crateHD, LOW);
    Serial.println("Crate2 OK");
    level = 33;
  }
}

void Triple()
{
  if ((!digitalRead(triplIN) || operSkips[tripl]) && !gStates[tripl])
  {
    if (operSkips[tripl]) sendHLms(triplOUT, 250);
    else playerGDone[tripl] = true;
    level = 34;
  }
}

void Gun()
{
  // ROOM 3 "GUN"
  //                wait for first signal from triplIN   >> if skipped ??
  //                                                     turn off boxed
  //                                                     turn on gunBox
  //                                                     turn off lightR3B
  //                                                     signal to zombiOUT
  //                                                     signal to Video player 2,
  //                                                     mp3_player1 track4
  if ((!digitalRead(triplIN) || operSkips[gun]) && !gStates[gun])
  {
    if (!operSkips[gun]) playerGDone[gun] = true;
    digitalWrite(boxed, LOW);
    digitalWrite(gunBox, HIGH);
    sendHLms(zombiOUT, 250);
    sendHLms(video2, 100);
    //mp3Set(1);
    //mp3_play(4);
    Serial.println("Gun OK");
    level = 35;
  }
}

void Zombie()
{
  // ROOM 3 "ZOMBIE"
  //                wait for first signal from zombiIN   >> if skipped send signal to zombiOUT
  //                                                     signal to Video player 3,
  //                                                     signal to Video player 2,
  //                                                     mp3_player2 track3
  //                                                     send signal to ladder
  //                                                     turn off door4
  if ((!digitalRead(zombiIN) || operSkips[zombie]) && !gStates[zombie])
  {
    if (operSkips[zombie]) sendHLms(zombiOUT, 250);
    else playerGDone[zombie] = true;
    sendHLms(video3, 100);
    sendHLms(video2, 100);
    //mp3Set(2);
    //mp3_play(3);
    sendHLms(ladder, 250);
    digitalWrite(door4, LOW);
    Serial.println("Zombie OK");
    level = 50;
  }
}

void gameOver()
{
  Serial.println("Game Over");
  level = 0;
  startLevel = 1;
}
