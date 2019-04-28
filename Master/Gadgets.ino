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
  startRFWait = digitalRead(7);
  if (!startRFWait)
  {
    if (startLevel == 1)
    {
      digitalWrite(lightR2A, HIGH);
      digitalWrite(lightR2B, HIGH);
      digitalWrite(door4, LOW); // close last door
      digitalWrite(door2, LOW); // close second door
      sendHLms(video1, 100);
      sendHLms(video2, 100);
      mp3Set(1);
      mp3_play(1); // start ambiance sound
      delay(15);
      printEvent("Level 1 Start 2 OK, Level 1 Done", true);
      strip.setPixelColor(1, 0, 0, 0);
      strip.show();
      delay(50);
      digitalWrite(door3A, HIGH);  //LOCKED
      level = 10;
      lcd.clear();
      lcd.setCursor(0, 0); // X, Y
      lcd.print("Level 1 Done");
      delay(50);
      sendByte(0xAA);
      game = true;
      delay(3000);
      digitalWrite(phoneOUT, HIGH);  // turn on the phone after 3 seconds

    }
    else if (startLevel == 0)
    {
      digitalWrite(door1, LOW);
      digitalWrite(door2, HIGH);
      digitalWrite(door4, LOW);
      digitalWrite(door3A, HIGH);  //LOCKED
      strip.setPixelColor(2, 0, 0, 0);
      printEvent("Level 1 Start 1 OK", true);
      startLevel++;
      lcd.setCursor(0, 0);
      lcd.print("Pre-Start Done !");
      delay(900);
      digitalWrite(phoneOUT, LOW);  // turn off the phone
    }
    startRFWait = true;
  }
}

// ROOM 1 "RADIO"

void Radio()
{
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
    delay(100);
    digitalWrite(door2,   HIGH);  //open door 2
    delay(100);
    digitalWrite(lightR2A, LOW);  // ON LIGHT under door 2
    delay(100);
    digitalWrite(lightR2B, LOW); // OFF LIGHT under door 3
    delay(100);
    digitalWrite(lightR3A, LOW); // OFF LIGHT ROOM 3
    delay(100);
    digitalWrite(lightR3B, LOW); // OFF LIGHT ROOM 3
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
    printEvent("Radio OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Radio OK");
    delay(2000);
    level = 21;
  }
}

// ROOM 2 "GENERATOR"

void Generator()
{
  if ((!digitalRead(generIN) || operSkips[gener1]) && !gStates[gener1])
  {
    if (operSkips[gener1]) sendHLms(generOUT, 250);
    else playerGDone[gener1] = true;
    sendHLms(headOUT, 100);
    printEvent("Gen/FUEL OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Gen/FUEL");
    delay(200);
  }
  if ((!digitalRead(generIN) || operSkips[gener2]) && !gStates[gener2] && gStates[gener1])
  {
    if (operSkips[gener2]) sendHLms(generOUT, 250);
    else playerGDone[gener2] = true;
    //    sendHLms(headOUT, 100);  //removed 15/APR
    printEvent("Gen/RUN OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Gen/RUN ");
    delay(200);
    level = 22;
  }
}

// ROOM 2 "METER"

void Meter()
{
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

// ROOM 2 "CODE"

void Code()
{
  if ((!digitalRead(fusesIN) || operSkips[code]) && !gStates[code])
  {
    if (operSkips[code]) sendHLms(fusesOUT, 250);
    else playerGDone[code] = true;
    digitalWrite(lightR1, LOW);
    digitalWrite(door3A, LOW); // UNLOCKED
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

// ROOM 3 "FUSES"

void Fuses()
{
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
    for (int i = 0; i < 3; i++) strip.setPixelColor(i, strip.Color(200, 0, 0));
    strip.show();
    printEvent("Fuses OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Fuses OK");
    delay(250);
    digitalWrite(lightR3A, LOW); // OFF LIGHT ROOM 3
    delay(150);
    digitalWrite(lightR3B, LOW); // OFF LIGHT ROOM 3
    level = 31;
    sendHLms(shelfOUT, 250); // signal to turn on gas prop on SHELF
    delay(150);
    digitalWrite(lightR3A, HIGH);
    delay(150);
    digitalWrite(lightR3B, HIGH);
  }
}

// ROOM 3 "Door"

void Door(long t)
{

  if ((digitalRead(fusesIN) == LOW || operSkips[door]) && !gStates[door])
  {
    shake = true;
    if (!operSkips[door]) playerGDone[door] = true;
    digitalWrite(door3B, HIGH);
    strip.setPixelColor(2, greenColor);
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

void shakeIt() {
  if (locked == false) {
    if ((unlockTime + unlockDelay) < millis()) {
      lockDelay = random(20, 200);
      lockTime = millis();
      digitalWrite(door3A, HIGH);
      locked = true;
    }
  }

  if (locked == true) {
    if ((lockTime + lockDelay) < millis()) {
      digitalWrite(door3A, LOW);
      unlockDelay = random(20, 2000);
      unlockTime = millis();
      locked = false;
    }
  }
}


// ROOM 3 "Window"

void Window(long t)
{
  if ((digitalRead(alleyIN) == LOW || operSkips[window]) && !gStates[window])
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

// ROOM 3 "Gas"

void Gas()
{
  if ((!digitalRead(shelfIN) || operSkips[gas]) && !gStates[gas])
  {
    if (operSkips[gas]) sendHLms(shelfOUT, 250);
    else playerGDone[gas] = true;
    digitalWrite(lightR3A, LOW);
    delay(200);
    digitalWrite(lightR3B, LOW);
    printEvent("Gas OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Gas OK");
    delay(250);
    level = 34;
    digitalWrite(lightR3B, HIGH);
    delay(250);
    digitalWrite(lightR3B, HIGH);
  }
}

// ROOM 3 "Shelf"

void Shelf()
{
  if ((!digitalRead(shelfIN) || operSkips[shelf]) && !gStates[shelf] && gStates[gas])
  {
    if (operSkips[shelf]) sendHLms(shelfOUT, 250);
    else playerGDone[shelf] = true;
    strip.setPixelColor(0, greenColor);
    strip.show();
    sendHLms(video3, 100);
    printEvent("Shelf OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Shelf OK");
    delay(50);
    level = 35;
  }
}

// ROOM 3 "EMP"

void Emp()
{
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
    lcd.print("E.M.P. OK");
    delay(50);
    level = 36;
  }
}

// ROOM 3 "MAP" / CRATE

void World() // Duplicate
{
  if ((!digitalRead(crateIN) || operSkips[world]) && !gStates[world])
  {
    if (operSkips[world]) sendHLms(crateOUT, 250);
    else playerGDone[world] = true;
    digitalWrite(crateHD, LOW);
    printEvent("MAP OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("MAP OK");
    delay(50);
    level = 37;
  }
}

// ROOM 3 "FLARE" / CRATE

void Flare()
{
  if ((!digitalRead(crateIN) || operSkips[flare]) && !gStates[flare] && gStates[world])
  {
    if (operSkips[flare]) sendHLms(crateOUT, 250); else playerGDone[flare] = true;

    sendHLms(video4, 100);
    sendHLms(video2, 100);
    sendHLms(video2, 100);
    digitalWrite(lightR3A, LOW); // OFF LIGHT ROOM 3
    digitalWrite(lightR3B, LOW); // OFF LIGHT ROOM 3

    // Light and video commands
    digitalWrite(crateHD, LOW);
    printEvent("Flare OK", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Flare OK");
    delay(500);
    level = 38;
    digitalWrite(lightR3A, HIGH); // ON LIGHT ROOM 3
    digitalWrite(lightR3B, HIGH); // ON LIGHT ROOM 3

  }
}

// ROOM 3 "ZOMBIE" / CRATE

void Zombie()
{
  // ROOM 3 "ZOMBIE"
  //                wait for first signal from zombiIN   >> if skipped send signal to zombiOUT
  //                                                     signal to Video player 3,
  //                                                     signal to Video player 2,
  //                                                     mp3_player2 track3
  //                                                     turn off door4
  if ((!digitalRead(zombiIN) || operSkips[zombie]) && !gStates[zombie])
  {
    if (operSkips[zombie]) sendHLms(zombiOUT, 250);
    else playerGDone[zombie] = true;
    sendHLms(video3, 100);
    sendHLms(video2, 100);
    mp3Set(2);
    mp3_set_volume(30);
    delay(10);
    mp3_play(3);
    delay(10);
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
    sendByte(0xBB);
    delay(50);
    printEvent("Game Over", true);
    lcd.clear();
    lcd.setCursor(0, 0); // X, Y
    lcd.print("Game Over");
    delay(50);
    game = false;
  }
}

void Head()
{
  if (operSkips[head])
  {
    sendHLms(headOUT, 250);
    operSkips[head] = false;
    delay(100);
  }
}

void Hatch()
{
  // if signal from hatchIN the signal to hatchOUT, or skip
  // 250ms signal to hatchOUT
  if ((!digitalRead(hatchIN) || operSkips[hatch]) && gStates[fuses])
  {
    sendHLms(hatchOUT, 250);
    //delay(300);
  }
}
