boolean debounce(boolean prevstate , int pin)
{
  boolean currstate = digitalRead(pin);
  if (currstate != prevstate)
  {
    delay(10);
    currstate = digitalRead(pin);
  }
  return currstate;
}

void sendHLms(int pin, int ms)
{
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
}

void mp3Set(byte n)
{
  delay(100);
  if (n == 1) mp3_set_serial(Serial2);
  else if (n == 2) mp3_set_serial(Serial3);
  delay(100);
}
