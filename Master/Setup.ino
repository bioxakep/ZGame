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


  digitalWrite(door1     , LOW);
  digitalWrite(door2     , LOW);
  digitalWrite(door3A    , HIGH);
  digitalWrite(door3B    , HIGH);
  digitalWrite(door4     , LOW);
  digitalWrite(gunBox    , LOW);

  digitalWrite(lightR2A  , LOW);  // LOW = ON
  digitalWrite(lightR2B  , HIGH); // LOW = OFF

  digitalWrite(lightR3A  , LOW);
  digitalWrite(lightR3B  , HIGH);
  delay(1300);
  //digitalWrite(lightR3A  ,HIGH);


  Serial.println("radio = " + String(digitalRead(radioIN) ? "HIGH" : "LOW"));
  Serial.println("gener = " + String(digitalRead(generIN) ? "HIGH" : "LOW"));
  Serial.println("meter = " + String(digitalRead(meterIN) ? "HIGH" : "LOW"));
  Serial.println("alley = " + String(digitalRead(alleyIN) ? "HIGH" : "LOW"));
  Serial.println("fuses = " + String(digitalRead(fusesIN) ? "HIGH" : "LOW"));
  Serial.println("shelf = " + String(digitalRead(shelfIN) ? "HIGH" : "LOW"));
  Serial.println("crate = " + String(digitalRead(crateIN) ? "HIGH" : "LOW"));
  Serial.println("tripl = " + String(digitalRead(triplIN) ? "HIGH" : "LOW"));
  Serial.println("zombi = " + String(digitalRead(zombiIN) ? "HIGH" : "LOW"));
  //Serial.println("door3 = "+String(digitalRead(door3IN) ? "HIGH" : "LOW"));

  for (int  x = 0; x < 3; x++) {
    strip.setPixelColor(x, 0, 0, 0); // ALL RED (SHOULD START ALL BLACK)
  }

  
  for (int i = 0; i < gCount; i++)
  {
    operSkips[i] = false;
    gStates[i] = false;
    playerGDone[i] = false;
  }
  greenColor = strip.Color(150, 0, 0);
  redColor = strip.Color(0, 150, 0);
  strip.show();
}
