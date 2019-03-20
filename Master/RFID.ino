boolean getStartRFID() {
  firstRF->check();
  if(firstRF->isBeaconConnecting())
  {
    byte addr[6];
    int ficha2 = 0;
    firstRF->getAddress(addr);
    //Serial.print("\nBeacon-Under: ");
    for(int i = 0; i < 6; i++) 
    {                               
      ficha2 = ficha2 + addr[i];    // Serial.print(addr[i], HEX);
    }
    //Serial.print(" sum = ");
    //Serial.println(ficha2);
    if (ficha2 == underBeaconA || ficha2 == underBeaconB) return true;
    else return false;
  }
  else return false;
}
