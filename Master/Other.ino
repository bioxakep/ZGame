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
  delay(100);
  if (n == 1) mp3_set_serial(Serial2);
  else if (n == 2) mp3_set_serial(Serial3);
  delay(100);
}

void printEvent(String e)
{
  unsigned long t = millis();
  String h_str = String(hours(t));
  if (h_str.length() < 2) h_str = "0" + h_str;
  String m_str = String(minutes(t));
  if (m_str.length() < 2) m_str = "0" + m_str;
  String s_str = String(seconds(t));
  if (s_str.length() < 2) s_str = "0" + s_str;
  Serial.println(String(h_str + ":" + m_str + ":" + s_str) + ": " + e);
}

int seconds(unsigned long t) {
  return int((t / 1000) % 60);
}

int minutes(unsigned long t) {
  return int((t / (1000 * 60)) % 60);
}

int hours(unsigned long t) {
  return int((t / (1000 * 60 * 60)) % 24);
}
