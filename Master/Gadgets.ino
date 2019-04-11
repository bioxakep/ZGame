// START > RADIO > GEN/FUEL > GEN/RUN > METER > CODE  >   FUSES  >  DOOR   >  WINDOW  > GAS     >  SHELF  > E.M.P. > MAP  >  FLARE  >  ZOMBIE
// ROOM1---radioIN |          |         |       |         |         |         |       |         |         |        |         |         |
// ROOM2-----------generIN-1  |         |       |         |         |         |       |         |         |        |         |         |
//                            generIN-2 |       |         |         |         |       |         |         |        |         |         |
//                                      meterIN |         |         |         |       |         |         |        |         |         |
//                                              fusesIN-1 |         |         |       |         |         |        |         |         |
// ROOM3--------------------------------------------------fusesIN-2 |         |       |         |         |        |         |         |
//                                                                  fusesIN-3 |       |         |         |        |         |         |
//                                                                            alleyIN |         |         |        |         |         |
//                                                                                    shelfIN-1 |         |        |         |         |
//                                                                                              shelfIN-2 |        |         |         |
//                                                                                                        tripleIN |         |         |
//                                                                                                                 crateIN-1 |         |
//                                                                                                                           crateIN-2 |
//_____________________________________________________________________________________________________________________________________zombieIN
// [HEAD ] works anywhere
// [HATCH] unlock ladder works anywhere


void Start(long t)
{
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
      mp3Set(1);
      mp3_play(1);
      delay(15);
      printEvent("Level 1 Start 2 OK, Level 1 Done", true);
      strip.setPixelColor(1, 0, 0, 0);
      strip.show();
      delay(50);
      level = 10;
      lcd.clear();
      lcd.setCursor(0, 0); // X, Y
      lcd.print("Level 1 Done");
      delay(50);
      sendByte(0xAA);
      game = true;
      delay(50);
      lcd.setCursor(0, 1); // X, Y
      lcd.print("Send Start to M");
      delay(50);
    }
    else if (startLevel == 0)
    {
      digitalWrite(door1, LOW);
      digitalWrite(door2, HIGH);
      digitalWrite(door4, LOW);
      strip.setPixelColor(2, 0, 0, 0);
      printEvent("Level 1 Start 1 OK", true);
      startLevel++;
      lcd.setCursor(0, 0);
      lcd.print("Pre-Start Done !");
      delay(800);
    }
    startRFWait = true;
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
    mp3Set(1);
    mp3_play(2);
    delay(10);
    mp3Set(2);
    mp3_play(2);
    delay(10);
    digitalWrite(phoneOUT, LOW);  // turn off the phone
    digitalWrite(door2,   HIGH);  //open door 2
    digitalWrite(lightR2A, LOW);  // ON LIGHT under door 2
    digitalWrite(lightR2B, LOW); // OFF LIGHT under door 3
    digitalWrite(lightR3A, LOW); // OFF LIGHT ROOM 3
    digitalWrite(lightR3B, LOW); // OFF LIGHT ROOM 3
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
    printEvent("Radio OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Radio OK");
    delay(50);
    level = 21;
  }
}

void Generator()
{
  // ROOM 2 "GENERATOR"
  //                   wait for signal from generIN   >> if skipped send signal to generOUT
  //                                                     send signal to headOUT
  if ((!digitalRead(generIN) || operSkips[gener1]) && !gStates[gener1])
  {
    if (operSkips[gener1]) sendHLms(generOUT, 250);
    else playerGDone[gener1] = true;
    sendHLms(headOUT, 100);
    printEvent("Generator-1 OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Generator/FUEL");
    delay(200);
  }
  if ((!digitalRead(generIN) || operSkips[gener2]) && !gStates[gener2] && gStates[gener1])
  {
    if (operSkips[gener2]) sendHLms(generOUT, 250);
    else playerGDone[gener2] = true;
    sendHLms(headOUT, 100);
    printEvent("Generator-2 OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Generator/RUN ");
    delay(200);
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
    mp3Set(1);
    mp3_set_volume(15);
    delay(10);
    mp3_play(3);
    delay(10);
    digitalWrite(lightR1, LOW);
    digitalWrite(lightR2B, HIGH);
    sendHLms(alleyOUT, 250);
    printEvent("Meter OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Meter OK");
    sendHLms(fusesOUT, 250);
    delay(50);
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
    mp3_play(2);
    delay(10);
    printEvent("Code OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Code OK");
    delay(50);
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
  if ((!digitalRead(fusesIN) || operSkips[fuses]) && !gStates[fuses])
  {
    if (operSkips[fuses]) sendHLms(fusesOUT, 250);
    else playerGDone[fuses] = true;
    sendHLms(video1, 100);
    sendHLms(video2, 100);
    //mp3Set(1);
    mp3_play(3);
    delay(10);
    digitalWrite(lightR3A, HIGH);
    digitalWrite(lightR3B, HIGH);
    for (int i = 0; i < 3; i++) strip.setPixelColor(i, strip.Color(0, 200, 0));
    strip.show();
    printEvent("Fuses2 OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Fuses2 OK");
    delay(50);
    level = 31;
  }
}

void Door(long t)
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
  if ((t - fusesSigStart > 3000 && fusesSigStart - fusesSigStop > 0 || operSkips[door]) && !gStates[door])
  {
    if (!operSkips[door]) playerGDone[door] = true;
    digitalWrite(door3B, HIGH);
    strip.setPixelColor(0, greenColor);
    strip.show();
    printEvent("Door OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Door OK");
    delay(50);
    level = 32;
    // What else if skip or player
  }
}

void Window(long t)
{
  windowStates[0] = debounce(alleyIN, windowStates[1]);
  if (!windowStates[0] && windowStates[1]) windowSigStart = t;
  if (!windowStates[1] && windowStates[0]) windowSigStop = t;
  windowStates[1] = windowStates[0];
  if ((t - windowSigStart > 3000 && windowSigStart - windowSigStop > 0 || operSkips[window]) && !gStates[window])
  {
    if (!operSkips[window]) playerGDone[window] = true;
    sendHLms(video2, 100);
    strip.setPixelColor(1, greenColor);
    strip.show();
    printEvent("Window OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Window OK");
    delay(50);
    level = 33;
    // What else if skip or player
  }
}

void Gas()
{
  if ((!digitalRead(shelfIN) || operSkips[gas]) && !gStates[gas])
  {
    if (operSkips[gas]) sendHLms(shelfOUT, 250);
    else playerGDone[gas] = true;
    printEvent("Gas OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Gas OK");
    delay(50);
    level = 34;
  }
}

void Shelf()
{
  if ((!digitalRead(shelfIN) || operSkips[shelf]) && !gStates[shelf] && gStates[gas])
  {
    if (operSkips[shelf]) sendHLms(shelfOUT, 250);
    else playerGDone[shelf] = true;
    strip.setPixelColor(2, greenColor);
    strip.show();
    sendHLms(video3, 100);
    printEvent("shelf OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Shelf OK");
    delay(50);
    level = 35;
  }
}

void Emp()
{
  // ROOM 3 "GUN"
  //                wait for first signal from triplIN   >> if skipped ??
  //                                                     turn off boxed
  //                                                     turn on gunBox
  //                                                     turn off lightR3B
  //                                                     signal to zombiOUT
  //                                                     signal to Video player 2,
  //                                                     mp3_player1 track4
  if ((!digitalRead(triplIN) || operSkips[emp]) && !gStates[emp])
  {
    if (!operSkips[emp]) playerGDone[emp] = true;
    digitalWrite(boxed, LOW);
    digitalWrite(gunBox, HIGH);
    sendHLms(zombiOUT, 250);
    sendHLms(video2, 100);
    //mp3Set(1);
    mp3_play(4);
    delay(10);
    printEvent("EMP OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("EMP OK");
    delay(50);
    level = 36;
  }
}

void World() // Duplicate
{
  // ROOM 3 "MAP" / CRATE
  //                wait for first signal from crateIN   >> if skipped send signal to crateOUT
  //                                                     signal to Video player 4,
  //                                                     signal to Video player 2,
  //                                                     signal to Video player 2,
  //                                                     open crateHD
  if ((!digitalRead(crateIN) || operSkips[world]) && !gStates[world])
  {
    if (operSkips[world]) sendHLms(crateOUT, 250);
    else playerGDone[world] = true;
    digitalWrite(crateHD, LOW);
    printEvent("Crate1 OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Crate1 OK");
    delay(50);
    level = 37;
  }
}

void Flare()
{
  if ((!digitalRead(crateIN) || operSkips[flare]) && !gStates[flare] && gStates[world])
  {
    if (operSkips[flare]) sendHLms(crateOUT, 250);
    else playerGDone[flare] = true;
    sendHLms(video4, 100);
    sendHLms(video2, 100);
    sendHLms(video2, 100);
    // Light and video comands
    digitalWrite(crateHD, LOW);
    printEvent("Crate2 OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Crate2 OK");
    delay(50);
    level = 38;
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
    mp3Set(2);
    mp3_set_volume(15);
    delay(10);
    mp3_play(3);
    delay(10);
    //sendHLms(ladder, 250); ???????
    digitalWrite(door4, LOW);
    printEvent("Zombie OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Zombie OK");
    delay(50);
    level = 50;
  }
}

void gameOver()
{
  if (game)
  {
    delay(100);
    sendByte(0xBB);
    delay(50);
    printEvent("Game Over", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Game Over");
    delay(50);
    game = false;
  }
  else
  {;}
}

void Head()
{
  if (operSkips[head])
  {
    sendHLms(headOUT, 250);
    operSkips[head] = false;
  }
}

void Hatch()
{
  // if signal from hatchIN the signal to hatchOUT, or skip
  // 250ms signal to hatchOUT
  if ((!digitalRead(hatchIN) || operSkips[hatch]) && gStates[fuses])
  {
    sendHLms(hatchOUT, 250);
  }
}
