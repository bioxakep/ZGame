boolean getStartRFID() {
  cpz1->check();
  if(cpz1->isBeaconConnecting())
  {
    byte addr[6];
    int ficha2 = 0;
    cpz1->getAddress(addr);
    //Serial.print("\nBeacon-Under: ");
    for(int i = 0; i < 6; i++) ficha2 = ficha2 + addr[i];    // Serial.print(addr[i], HEX);
    Serial.println("RFID OK");
    return true;
  }
  else return false;
}
