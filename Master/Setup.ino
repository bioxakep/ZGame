void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(38400); // RS-485
  Serial2.begin(9600); // mp3player A
  Serial3.begin(9600); // mp3player B

  strip.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   TEST MODE    ");
  lcd.setCursor(0, 1);
  lcd.print("They are coming!");
  delay(3000);

  Serial.println("TAC Master 27/APR/2019 ");
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

  pinMode(hatchIN   , INPUT_PULLUP);

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
  pinMode(hatchOUT  , OUTPUT);

  // ----video ------
  pinMode(video1    , OUTPUT);
  pinMode(video2    , OUTPUT);
  pinMode(video3    , OUTPUT);
  pinMode(video4    , OUTPUT);
  
  pinMode(RSTXCNTRL, OUTPUT);
  //pinMode(7, INPUT_PULLUP);
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

  digitalWrite (hatchOUT,  LOW);  // LOW = Closed

  digitalWrite (radioOUT,  LOW);
  digitalWrite (generOUT,  LOW);
  digitalWrite (meterOUT,  LOW);
  digitalWrite (headOUT,   LOW);
  digitalWrite (alleyOUT,  LOW);
  digitalWrite (shelfOUT,  LOW);
  digitalWrite (crateOUT,  LOW); 
  digitalWrite (crateHD,   HIGH); //open
  digitalWrite (gunBox,    HIGH); //open
  digitalWrite (zombiOUT,  LOW);

  digitalWrite (boxed,     HIGH); // LOW=CLOSED

  digitalWrite(door1     , HIGH);  //open
  digitalWrite(door2     , LOW);  // closed > light Room 1 ON
  digitalWrite(door3A    , LOW);  // open
  digitalWrite(door3B    , LOW);  // open
  digitalWrite(door4     , LOW);  // open

  digitalWrite(RSTXCNTRL, LOW);

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

  greenColor = strip.Color(0, 150, 0);
  redColor   = strip.Color(150, 0, 0);


  delay(50);
  mp3Set(1);
  delay(50);
  mp3_set_volume(25);
  delay(50);
  mp3_stop();
  delay(50);
  mp3Set(2);
  delay(50);
  mp3_set_volume(30);
  delay(50);
  mp3_stop();
  delay(50);
  
  printEvent("Setup OK", true);
  lcd.clear();

  checkStates();
  connectToBridge();
}

void checkStates() {
  lcd.setCursor(0, 1); // lower row
  lcd.print(String(digitalRead(radioIN) ? " r_" : " R_"));
  lcd.print(String(digitalRead(generIN) ? "g" : "G"));
  lcd.print(String(digitalRead(meterIN) ? "m" : "M"));
  lcd.print(String(digitalRead(alleyIN) ? "a_" : "A_"));
  lcd.print(String(digitalRead(fusesIN) ? "f" : "F"));
  lcd.print(String(digitalRead(shelfIN) ? "s" : "S"));
  lcd.print(String(digitalRead(crateIN) ? "c" : "C"));
  lcd.print(String(digitalRead(triplIN) ? "t_" : "T_"));
  lcd.print(String(digitalRead(zombiIN) ? "z" : "Z"));
  lcd.print(String(digitalRead(hatchIN) ? "_h " : "_H "));
  delay(10);
}
