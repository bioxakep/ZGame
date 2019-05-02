boolean debounce(boolean prevstate , int pin)
{
  boolean currstate = digitalRead(pin);
  if (currstate != prevstate)
  {
    delay(15);
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
  delay(50);
  if (n == 1) mp3_set_serial(Serial2);
  else if (n == 2) mp3_set_serial(Serial3);
  delay(50);
}

void printEvent(String e, boolean endline)
{
  unsigned long t = millis();
  String h_str = String(hours(t));
  if (h_str.length() < 2) h_str = "0" + h_str;
  String m_str = String(minutes(t));
  if (m_str.length() < 2) m_str = "0" + m_str;
  String s_str = String(seconds(t));
  if (s_str.length() < 2) s_str = "0" + s_str;
  if(endline) Serial.println(String(h_str + ":" + m_str + ":" + s_str) + ": " + e);
  else Serial.print(String(h_str + ":" + m_str + ":" + s_str) + ": " + e);
}

int seconds(unsigned long t) {
  unsigned long dev = t/1000;
  return int(dev % 60);
}

int minutes(unsigned long t) {
  unsigned long dev = t/1000;
  return int((dev / 60) % 60);
}

int hours(unsigned long t) {
  unsigned long dev = t/1000;
  return int((dev / 3600) % 24);
}
