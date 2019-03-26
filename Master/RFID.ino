boolean getStartRFID() {
  cpz1->check();
  if(cpz1->isBeaconConnecting())
  {
    byte addr[6];
    int ficha2 = 0;
    cpz1->getAddress(addr);
    //Serial.print("\nBeacon-Under: ");
    for(int i = 0; i < 6; i++) 
    {                               
      ficha2 = ficha2 + addr[i];    // Serial.print(addr[i], HEX);
    }
    //Serial.print(" sum = ");
    //Serial.println(ficha2);
    if (ficha2 == ficha1ok) 
    {
      Serial.println("RFID Code OK");
      return true;
    }
    else
    {
      Serial.println("RFID Code NOT OK");
      return false;
    }
  }
  else return false;
}
