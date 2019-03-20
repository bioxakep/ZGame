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
