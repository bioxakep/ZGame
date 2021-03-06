void loop()
{
  getBridgeData();
  unsigned long tick = millis();
  Hatch();
  Head();
  if (level == 0) Start(tick);
  else if (level == 10) Radio();
  else if (level == 21) Generator();
  else if (level == 22) Meter();
  else if (level == 23) Code();
  else if (level == 30) Fuses(); 
  else if (level == 31) WinDoorGasShelf();
  else if (level == 35) Emp();
  else if (level == 36) World();
  else if (level == 37) Flare();
  else if (level == 38) Zombie();
  else if (level == 50) gameOver();

  if (level > 34 && level < 50) shakeIt();
  
  sendGStates();
  checkStates();
  if (shake) shakeIt();

 if (startTimer > 0 && ((millis() - startTimer) > startDelay))
  {
    digitalWrite(phoneOUT, HIGH);  // turn on the phone after 3 seconds
    mp3Set(2);
    mp3_set_volume(25);
    delay(50);
    mp3_play(1); // start ambiance in room #2
    startTimer = 0;
  }

 if (radioTimer > 0 && ((millis() - radioTimer) > radioDelay))
  {
    sendHLms(video1, 100);
    digitalWrite(phoneOUT, LOW);  // turn off the phone
    digitalWrite(lightR2A, LOW);  // ON LIGHT under door 2 and EXIT sign in room 1
    digitalWrite(door2,   HIGH);  //open door 2
   // delay(200);
    digitalWrite (xmonitor, LOW);  // HIGH = ON
    digitalWrite(lightR2B, LOW); // OFF LIGHT under door 3
    delay(200);
    digitalWrite(lightR3A, LOW); // OFF LIGHT ROOM 3
    digitalWrite(lightR3B, LOW); // OFF LIGHT ROOM 3
    delay(200);
    radioTimer = 0;
  }

 if (shelfTimer > 0 && ((millis() - shelfTimer) > shelfDelay))
  {
    mp3Set(1);
    mp3_play(44); // last window secured
    delay(50);
    shelfTimer = 0;
  }

 if (flareTimer > 0 && ((millis() - flareTimer) > flareDelay))
  {
    sendHLms(video1, 100);  // first monitor pulse message
    flareTimer = 0;
    witchTimer = millis();
    digitalWrite(lightR2A, HIGH);  // OFF LIGHT above door 2
    digitalWrite(lightR2B, LOW);  // OFF LIGHT 
    lcd.setCursor(0, 0);  
    lcd.print("  pilot MSG #1  ");

  }

 if (witchTimer > 0 && ((millis() - witchTimer) > witchDelay))
  {
    delay(500);
    digitalWrite(gunBox, HIGH);
    delay(2300);
    mp3Set(1);
    mp3_play(6);
    delay(200);
    digitalWrite(lightR3A, LOW); 
    delay(150);
    digitalWrite(lightR3B, LOW); 
    delay(250);
    digitalWrite(lightR3A, HIGH);
    delay(150);
  
    digitalWrite(lightR3B, HIGH);

    strip.setPixelColor(0, 0, 0, 0);
    strip.setPixelColor(1, 0, 0, 0);
    strip.setPixelColor(2, 0, 0, 0);
    strip.show();
  
    delay(200);
    digitalWrite(lightR3A, LOW); 
    delay(100);
    digitalWrite(lightR3B, LOW); 
    delay(250);
    digitalWrite(lightR3B, HIGH);
    delay(50);
    digitalWrite(lightR3A, HIGH);
    delay(150);
    digitalWrite(lightR3A, LOW); 
    digitalWrite(lightR3B, LOW);   
   
    
    sendHLms(zombiOUT, 350);
    lcd.setCursor(0, 0);  
    lcd.print(" Witch out  ");
    delay(17000);
    sendHLms(video1, 100);  // first monitor breach message
    witchTimer = 0;
    lcd.setCursor(0, 0);  
    lcd.print(" Breach MSG  ");
  }

  if (tick - lastSyncTime > 10000)
  {
    sendByte(0xA9);
    printEvent("Send Sync signal", true);
    lastSyncTime = tick;
  }

}

  //----------------------------------------------------------------------------------------
  // first start  >>  lock the doors
  // OK
  //----------------------------------------------------------------------------------------
  // second start  >>  turn off the lights (lightR2A, lightR2B, lightR3A, lightR3B),
  //                   signal to Video players 1 and 2,
  //                   mp3_player1 track1
  // OK
  //----------------------------------------------------------------------------------------
  // GAME STARTED  >>
  //
  // ROOM 1 "RADIO">>  wait for signal from radioIN  >> if skipped send signal to radioOUT
  //                                                     signal to Video player 3,
  //                                                     mp3_player1 track2
  //                                                     mp3_player2 track2
  //                                                     turn off ligtsR1
  //                                                     turn on  ligtsR2A
  //                                                     turn off phoneOUT
  //----------------------------------------------------------------------------------------
  // ROOM 2 "GENERATOR"
  //                   wait for signal from generIN   >> if skipped send signal to generOUT
  //                                                     send signal to headOUT
  //----------------------------------------------------------------------------------------
  // ROOM 2 "METER"
  //                   wait for signal from meterIN   >> if skipped send signal to meterOUT
  //                                                     signal to Video player 1,
  //                                                     mp3_player1 track3
  //                                                     turn off ligtsR2A
  //                                                     turn on  ligtsR2B
  //                                                     send signal to alleyOUT
  //                                                     the signal from meterIN is permamnet if lost turn off lightsR2B until restored
  //----------------------------------------------------------------------------------------
  // ROOM 2 "CODE"
  //                wait for first signal from fusesIN   >> if skipped send signal to fusesOUT
  //                                                     turn off ligtsR1
  //                                                     turn off door3A
  //                                                     signal to Video player 3,
  //                                                     mp3_player1 track2
  //----------------------------------------------------------------------------------------
  // ROOM 3 "FUSES"
  //                wait for second signal from fusesIN  >> if skipped send signal to fusesOUT
  //                                                     signal to Video player 2,
  //                                                     signal to Video player 1,
  //                                                     mp3_player1 track3
  //                                                     turn on lightR3A and lightR3B
  //                                                     turn on neopixel 1-3 red
  //----------------------------------------------------------------------------------------
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
  //----------------------------------------------------------------------------------------
  // ROOM 3 "MAP" / CRATE
  //                wait for first signal from crateIN   >> if skipped send signal to crateOUT
  //                                                     signal to Video player 4,
  //                                                     signal to Video player 2,
  //                                                     signal to Video player 2,
  //                                                     open crateHD
  //----------------------------------------------------------------------------------------
  // ROOM 3 "GUN"
  //                wait for first signal from triplIN   >> if skipped
  //                                                     turn off boxed
  //                                                     turn on gunBox
  //                                                     turn off lightR3B
  //                                                     signal to zombiOUT
  //                                                     signal to Video player 2,
  //                                                     mp3_player1 track4
  //----------------------------------------------------------------------------------------
  // ROOM 3 "ZOMBIE"
  //                wait for first signal from zombiIN   >> if skipped send signal to zombiOUT
  //                                                     signal to Video player 3,
  //                                                     signal to Video player 2,
  //                                                     mp3_player2 track3
  //                                                     send signal to ladder
  //                                                     turn off door4
  //-----------------------------------------------------------------------------------------
  // GAME OVER
