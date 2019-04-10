void getBridgeData()
{
  digitalWrite(RSTXCNTRL,LOW); //allways LOW besides send mode
  if (Serial1.available() > 0)
  {
    byte input[gCount];
    byte inByte = Serial1.read();
    if (inByte == 0xBD) // Bridge Gadget Data
    {
      printEvent("Operator Skips Recieved:", false);
      delay(380);
      for (int i = 0; i < gCount; i++)
      {
        input[i] = Serial1.read();
        if (input[i] == 0x03) operSkips[i] = true;
        Serial.print(input[i]);
        Serial.print("|");
      }
      byte last = Serial1.read();
      if (last == 0xFF) Serial.println("OK");
    }
    else if (inByte == 0xBC) // Restart connection (clear game data)
    {
      printEvent("Resync with Bridge...", true);
      bridgeConnected = false;
      resetStates();
      lcd.clear();
      lcd.print("Reset...");
      connectToBridge();
    }
    else Serial1.flush();
  }
}


void sendGStates() // Проверяем прошел ли игрок какой-нибудь гаджет или применяем скипы оператора
{
  byte chkSum = 0;
  boolean needSend = false;
  for (int s = 0; s < gCount; s++)
  {
    if ((playerGDone[s] || operSkips[s]) && !gStates[s])
    {
      if (playerGDone[s]) needSend = true;
      gStates[s] = true;
      operSkips[s] = false;
    }
  }
  if (needSend)
  {
    digitalWrite(RSTXCNTRL, HIGH);  // Init Transmitter
    Serial.print("Send States to Operator: ");
    delay(5);
    Serial1.write(0xAD);
    delay(15);
    for (int d = 0; d < gCount; d++)
    {
      if (gStates[d] && !playerGDone[d]) Serial1.write(0x03);
      else if (gStates[d] && playerGDone[d]) Serial1.write(0x05);
      else Serial1.write(0x01);
      delay(15);
      operSkips[d] = false;
      if (gStates[d] && !playerGDone[d]) Serial.print('3');
      else if (gStates[d] && playerGDone[d]) Serial.print('5');
      else Serial.print('1');
    }
    Serial1.write(0xFF);
    delay(25);
    digitalWrite(RSTXCNTRL, LOW);  // Stop Transmitter
    printEvent(", level = " + String(level), true); // DEBUG
  }
}

void connectToBridge()
{
  byte outByte = 0xA1;
  unsigned long tick = millis();
  unsigned long sendTime = tick;
  Serial.print("Connecting to Bridge..");
  while (!bridgeConnected)
  {
    sendTime = tick;
    digitalWrite(RSTXCNTRL, HIGH);
    Serial1.write(outByte);
    delay(15);
    digitalWrite(RSTXCNTRL, LOW);
    delay(5);
    Serial.print(".");
    while (tick - sendTime < 1500)
    {
      tick = millis();
      if (Serial1.available() > 0)
      {
        while (Serial1.available())
        {
          byte inByte = Serial1.read();
          if (inByte == 0xA1 && outByte == 0xA1) 
          {
            outByte = 0xA2;
            Serial.print(".sync.");
          }
          if (inByte == 0xA2 && outByte == 0xA2) bridgeConnected = true;
        }
      }
    }
  }
  lastSyncTime = tick;
  Serial.println("Connected");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bridge connected");
  delay(1000);
}

void resetStates()
{
  level = 0;
  for (int i = 0; i < gCount; i++)
  {
    operSkips[i] = false;
    gStates[i] = false;
    playerGDone[i] = false;
  }
  startLevel = 0;
  fusesStates[0] = fusesStates[1] = HIGH;
  alleyStates[0] = alleyStates[1] = HIGH;
}

void sendByte(byte n)
{
  digitalWrite(RSTXCNTRL, HIGH);
  delay(5);
  Serial1.write(n);
  delay(15);
  digitalWrite(RSTXCNTRL, LOW);
  delay(5);
}
