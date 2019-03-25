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
    /*digitalWrite(SSerialTxControl, HIGH);  // Init Transmitter
    Serial.print("Send States to Operator: ");
    Serial1.write(0xAD);
    delay(10);
    for (int d = 0; d < 32; d++)
    {
      if (gStates[d] && !playerGDone[d]) Serial1.write(0x03);
      else if (gStates[d] && playerGDone[d]) Serial1.write(0x05);
      else Serial1.write(0x01);
      delay(10);
      operSkips[d] = false;
      if (gStates[d] && !playerGDone[d]) Serial.print('3');
      else if (gStates[d] && playerGDone[d]) Serial.print('5');
      else Serial.print('1');
    }
    Serial1.write(0xFF);
    delay(10);
    digitalWrite(SSerialTxControl, LOW);  // Stop Transmitter
    Serial.println(", level = " + String(level)); // DEBUG
    */
  }
}
