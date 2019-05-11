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
      digitalWrite(door1, LOW); // close first door
      digitalWrite(door2, LOW); // close second door
      digitalWrite(door4, LOW); // close last door
      sendHLms(video1, 100);  // first monitor
      printEvent("Start 2 OK", true);
      strip.setPixelColor(1, 0, 0, 0);
      strip.show();
      delay(50);
      level = 10;
      lcd.setCursor(0, 0); lcd.print("   START Done   ");
      sendByte(0xAA);
      game = true;
      startTimer = millis();
    }
    else if (startLevel == 0)
    {
      mp3Set(1);
      mp3_play(111); //startup sound
      delay(50);
  
      digitalWrite(door2, HIGH);
      digitalWrite(door4, LOW);
      digitalWrite(door3A, HIGH);  //LOCKED
      digitalWrite (gunBox,    LOW);
      digitalWrite (crateHD,   LOW ); // LOW=CLOSED
      digitalWrite (boxed,     LOW); // LOW=CLOSED
      digitalWrite(phoneOUT, LOW);  // turn off the phone

      strip.setPixelColor(2, 0, 0, 0);
      printEvent("Level 1 Start 1 OK", true);
      startLevel++;
      lcd.setCursor(0, 0);
      lcd.print(" Pre-Start Done ");
      strip.show();
      delay(900);
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
    radioTimer = millis();
    digitalWrite(phoneOUT, LOW);  // turn off the phone
    delay(200);
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
    printEvent("Radio OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("    Radio OK    ");
    delay(200);
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
 //   sendHLms(video2, 100);
    printEvent("Generator-1 OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("   Gen/FUEL   ");
    strip.setPixelColor(2, 0, 50, 0);
    strip.show();
    delay(200);
  }
  if ((!digitalRead(generIN) || operSkips[gener2]) && !gStates[gener2] && gStates[gener1])
  {
    if (operSkips[gener2]) sendHLms(generOUT, 250);
    else playerGDone[gener2] = true;
    printEvent("Generator-2 OK", true);
    sendHLms(alleyOUT, 250);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("    Gen/RUN     ");
    strip.setPixelColor(1, 0, 50, 0);
    strip.show();
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
    lcd.setCursor(0, 0);
    lcd.print("    Meter OK    ");
    //   sendHLms(video1, 100);
    //    mp3Set(1);
    //    mp3_set_volume(25);
    //    delay(10);
    //    mp3_play(3);
    //    delay(10);
    digitalWrite(lightR1, LOW);
    digitalWrite(lightR2B, HIGH);
    strip.setPixelColor(0, 0, 50, 0);
    strip.show();
//    sendHLms(alleyOUT, 250);
    printEvent("Meter OK", true);
    sendHLms(fusesOUT, 250);
    delay(50);
    sendHLms(zombiOUT, 100);  // zombie display
    delay(50);
    sendHLms(video2, 100);    // start left  window
    sendHLms(video3, 100);    // start right window
    delay(50);
    sendHLms(video4, 100);    // and the top display
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
    lcd.setCursor(0, 0); lcd.print("    Code OK     ");

//    digitalWrite(lightR1, HIGH);
    digitalWrite(door3A, LOW); // UNLOCKED
 
    strip.setPixelColor(0, 0, 0, 0);
    strip.setPixelColor(1, 0, 0, 0);
    strip.setPixelColor(2, 0, 0, 0);
    strip.show();

    //mp3Set(1);
    //mp3_play(2);
    //delay(10);

    printEvent("Code OK", true);
  
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
    mp3Set(1);   
    mp3_play(1);   
    delay(50);
    digitalWrite(lightR3A, HIGH);
    digitalWrite(lightR3B, HIGH);
    for (int i = 0; i < 3; i++) strip.setPixelColor(i, strip.Color(200, 0, 0));
    strip.show();
    printEvent("Fuses2 OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("    Fuses OK    ");
    digitalWrite(door3B, HIGH); // activate horizontal lock
    delay(250);
    digitalWrite(lightR3A, LOW); // OFF LIGHT ROOM 3
    delay(150);
    digitalWrite(lightR3B, LOW); // OFF LIGHT ROOM 3
    level = 31;
    sendHLms(shelfOUT, 250); // signal to turn on gas prop on SHELF
    sendHLms(generOUT, 250); // signal to shift sound file on generator
    delay(150);
    digitalWrite(lightR3A, HIGH);
    delay(250);
    digitalWrite(lightR3B, HIGH);
    if(digitalRead(hatchIN)) hatchSW = true; else hatchSW = false; 
  }
}


// ROOM 3 "WinDoor"

void WinDoor(long t)
{
  if ((digitalRead(fusesIN) == LOW || operSkips[door]) && !gStates[door])
  {
    if (!operSkips[door]) playerGDone[door] = true;
    mp3Set(1);
    mp3_play(2);
    delay(50);
    strip.setPixelColor(2, greenColor);
    strip.show();
    printEvent("Door OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("    Door OK    ");
    delay(50);
    // What else if skip or player
  }

  if ((digitalRead(alleyIN) == LOW || operSkips[window]) && !gStates[window])
  {
    if (!operSkips[window]) playerGDone[window] = true;
    mp3Set(1);
    mp3_play(3);
    delay(50);
    sendHLms(video2, 100);
    strip.setPixelColor(1, greenColor);
    strip.show();
    printEvent("Window OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("   Window OK   ");
    delay(50);
    // What else if skip or player
  }
  if (gStates[door] && gStates[window]) level = 33;
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
    lcd.setCursor(0, 0); // X, Y
    lcd.print("    Gas OK     ");
    delay(250);
    level = 34;
    digitalWrite(lightR3B, HIGH);
//    delay(250);
  }
}

// ROOM 3 "Shelf"

void Shelf()
{
  if ((!digitalRead(shelfIN) || operSkips[shelf]) && !gStates[shelf] && gStates[gas])
  {
    if (operSkips[shelf]) sendHLms(shelfOUT, 250);
    else playerGDone[shelf] = true;
    mp3Set(1);
    mp3_play(4);
    delay(50);
    strip.setPixelColor(0, greenColor);
    strip.show();
    sendHLms(video3, 100);
    printEvent("shelf OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("   Shelf OK    ");
    delay(50);
    level = 35;
  }
}

// ROOM 3 "EMP"

void Emp()
{
  if ((!digitalRead(triplIN) || operSkips[emp]) && !gStates[emp])
  {
    shake = true;
    if (!operSkips[emp]) playerGDone[emp] = true;
    digitalWrite (boxed,     HIGH); // LOW=CLOSED
    //    digitalWrite(gunBox, HIGH); // temporal, later will be opened by timer after helicopter pilot message
    //    sendHLms(zombiOUT, 350);
    //    sendHLms(video2, 100);
    //    mp3Set(1);
    //    mp3_play(4);
    delay(10);
    printEvent("EMP OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("   E.M.P. OK   ");
    delay(350);
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
    digitalWrite(crateHD, HIGH);  //open
    printEvent("Map OK", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("    MAP OK    ");
    delay(50);
    level = 37;
     if (!ambiance) {
      digitalWrite(hatchOUT, HIGH);
      mp3Set(1);
      mp3_play(5);
      ambiance = true;
    }
  }
}

// ROOM 3 "FLARE" / CRATE

void Flare()
{
  if ((!digitalRead(crateIN) || operSkips[flare]) && !gStates[flare] && gStates[world])
  {
    if (operSkips[flare]) sendHLms(crateOUT, 250); else playerGDone[flare] = true;

    sendHLms(video4, 100); //flare lauched and helicopter arrives
    flareTimer = millis();

    printEvent("Flare OK", true);
    lcd.setCursor(0, 0);  
    lcd.print("   Flare OK   ");
    level = 38;
    delay(1500);
    lcd.setCursor(0, 0);  
    lcd.print("   Flare DONE ");
  }
}

// ROOM 3 "ZOMBIE" / CRATE

void Zombie()
{
  if ((!digitalRead(zombiIN) || operSkips[zombie]) && !gStates[zombie])
  {
    if (operSkips[zombie]) sendHLms(zombiOUT, 250);
    else playerGDone[zombie] = true;

    mp3Set(2);
    mp3_stop();
    mp3Set(1);
    mp3_stop();
    delay(50);

    printEvent("Zombie OK", true);
    lcd.setCursor(0, 0);  
    lcd.print("   Zombie OK    ");
    delay(2500);

    sendHLms(video1, 100);  // first monitor : Victory message !
    level = 50;
    delay(5000);
    digitalWrite(door4, LOW);
  }
}

void gameOver()
{
  if (game)
  {
    sendByte(0xBB);
    delay(50);
    printEvent("Game Over", true);
    lcd.setCursor(0, 0); // X, Y
    lcd.print("    Game Over    ");
    delay(50);
    game = false;
    delay(10000);
    digitalWrite(lightR3A, HIGH); // ON LIGHT ROOM 3
    delay(250);
    digitalWrite(lightR3B, HIGH); // ON LIGHT ROOM 3
    digitalWrite(door4     , HIGH);  // open
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
  if (((digitalRead(hatchIN) != hatchSW) || operSkips[hatch])  && gStates[shelf])
  {
    if (!ambiance) {
    digitalWrite(hatchOUT, HIGH);
    mp3Set(1);
   // mp3_set_volume(25);
    delay(50);
    mp3_play(5);
    ambiance = true;
    }
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
