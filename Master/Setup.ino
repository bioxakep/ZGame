void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600); // RS-485
  Serial2.begin(9600); // mp3player A
  Serial3.begin(9600); // mp3player B
  
  /*
  mp3_set_serial(Serial2);
  delay(100);
  mp3_set_volume (29);
  delay(100);
  mp3_play(4); // test hint sound
  delay(200);
  */

  strip.begin();


  Serial.println("TAC Master 05/MAR/2019\n");
  cpz1 = new ArdCPZ(PIN_CPZ1);

  // ---- props ----
  pinMode(phoneOUT , OUTPUT);

  pinMode(radioIN  , INPUT_PULLUP);
  pinMode(radioOUT , OUTPUT);

  pinMode(generIN  , INPUT_PULLUP);
  pinMode(generOUT , OUTPUT);

  pinMode(meterIN  , INPUT_PULLUP);
  pinMode(meterOUT , OUTPUT);

  pinMode(headOUT  , OUTPUT);

  pinMode(alleyIN  , INPUT_PULLUP);
  pinMode(alleyOUT , OUTPUT);

  pinMode(fusesIN  , INPUT_PULLUP);
  pinMode(fusesOUT , OUTPUT);

  pinMode(shelfIN  , INPUT_PULLUP);
  pinMode(shelfOUT , OUTPUT);

  pinMode(crateIN  , INPUT_PULLUP);
  pinMode(crateOUT , OUTPUT);
  pinMode(crateHD  , OUTPUT);

  pinMode(zombiIN  , INPUT_PULLUP);
  pinMode(zombiOUT  , OUTPUT);

  pinMode(triplIN   , INPUT_PULLUP);

  // ---- lights -----
  pinMode(lightR1   , OUTPUT);
  pinMode(lightR2A  , OUTPUT);
  pinMode(lightR2B  , OUTPUT);
  pinMode(lightR3A  , OUTPUT);
  pinMode(lightR3B  , OUTPUT);

  // ----doors locks ------
  pinMode(door1     , OUTPUT);
  pinMode(door2     , OUTPUT);
  pinMode(door3A    , OUTPUT);
  pinMode(door3B    , OUTPUT);
  pinMode(door4     , OUTPUT);

  // ---- locks ------
  pinMode(gunBox    , OUTPUT);
  pinMode(boxed     , OUTPUT);
  pinMode(ladder    , OUTPUT);

  // ----video ------
  pinMode(video1    , OUTPUT);
  pinMode(video2    , OUTPUT);
  pinMode(video3    , OUTPUT);
  pinMode(video4    , OUTPUT);


  digitalWrite(lightR1   , OUTPUT);
  digitalWrite(lightR2A  , OUTPUT);
  digitalWrite(lightR2B  , OUTPUT);
  digitalWrite(lightR3A  , OUTPUT);
  digitalWrite(lightR3B  , OUTPUT);

  digitalWrite(lightR1   , LOW);  // LOW = ON
  digitalWrite(lightR2A  , LOW);  // LOW = ON
  digitalWrite(lightR2B  , HIGH); // LOW = OFF
  digitalWrite(lightR3A  , HIGH);  // LOW = OFF
  digitalWrite(lightR3B  , HIGH); // LOW = OFF
  
  digitalWrite(video1   ,  LOW);
  digitalWrite(video2   ,  LOW);
  digitalWrite(video3   ,  LOW);
  digitalWrite(video4   ,  LOW);

  digitalWrite (phoneOUT,  HIGH);  // HIGH = ON

  digitalWrite (radioOUT,  LOW);
  digitalWrite (generOUT,  LOW);
  digitalWrite (meterOUT,  LOW);
  digitalWrite (headOUT,   LOW);
  digitalWrite (alleyOUT,  LOW);
  digitalWrite (shelfOUT,  LOW);
  digitalWrite (crateOUT,  LOW);
  digitalWrite (crateHD,   LOW);
  digitalWrite (gunBox,    LOW);
  digitalWrite (zombiOUT,  LOW);

  digitalWrite (boxed,     HIGH); // LOW=CLOSED
  digitalWrite (ladder,    LOW);  // FASTLOCK

  digitalWrite(door1     , HIGH);  //open
  digitalWrite(door2     , LOW);  // closed > light Room 1 ON
  digitalWrite(door3A    , LOW);  // open
  digitalWrite(door3B    , LOW);  // open
  digitalWrite(door4     , LOW);  // open
  digitalWrite(gunBox    , LOW);

  Serial.println("radio = " + String(digitalRead(radioIN) ? "HIGH" : "LOW"));
  delay(10);
  Serial.println("gener = " + String(digitalRead(generIN) ? "HIGH" : "LOW"));
  delay(10);
  Serial.println("meter = " + String(digitalRead(meterIN) ? "HIGH" : "LOW"));  
  delay(10);
  Serial.println("alley = " + String(digitalRead(alleyIN) ? "HIGH" : "LOW"));
  delay(10);
  Serial.println("fuses = " + String(digitalRead(fusesIN) ? "HIGH" : "LOW"));
  delay(10);
  Serial.println("shelf = " + String(digitalRead(shelfIN) ? "HIGH" : "LOW"));
  delay(10);
  Serial.println("crate = " + String(digitalRead(crateIN) ? "HIGH" : "LOW"));
  delay(10);
  Serial.println("tripl = " + String(digitalRead(triplIN) ? "HIGH" : "LOW"));
  delay(10);
  Serial.println("zombi = " + String(digitalRead(zombiIN) ? "HIGH" : "LOW"));
  delay(10);
  //Serial.println("door3 = "+String(digitalRead(door3IN) ? "HIGH" : "LOW"));
  delay(10);

  for (int  x = 0; x < 3; x++) {
    strip.setPixelColor(x, 0, 0, 110); // ALL RED (SHOULD START ALL BLACK)
  }
  strip.show();
  
  for (int i = 0; i < gCount; i++)
  {
    operSkips[i] = false;
    gStates[i] = false;
    playerGDone[i] = false;
  }
  
  greenColor = strip.Color(150, 0, 0);
  redColor = strip.Color(0, 150, 0);
  
  
  mp3Set(1);
  mp3_set_volume(15);
  
  Serial.println("Setup OK " + String(millis()));

  // Bridge Connector/
  connectToBridge();
}
