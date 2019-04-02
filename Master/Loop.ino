void loop()
{
  getBridgeData();
  unsigned long tick = millis();

  if (level == 0) Start(tick);
  else if (level == 10) Box();
  else if (level == 11) Radio();
  else if (level == 21) Generator();
  else if (level == 22) Meter();
  else if (level == 23) Code();
  else if (level == 30) Fuses();
  else if (level == 31) Lock(tick);
  else if (level == 32) Crate();// parallel or devide
  else if (level == 33) Triple();
  else if (level == 34) Gun();
  else if (level == 35) Zombie();
  else if (level == 50) gameOver();
  sendGStates();

  if (tick - lastSyncTime > 10000)
  {
    sendByte(0xA9);
    Serial.println("Send Sync signal");
    lastSyncTime = tick;
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
}
