

// 29.03.2019 Bridge in ZGame
// 02/04/2019 Big Update
#include <SoftwareSerial.h>
#define RSTXCNTRL 10
//0xAA from Master to Bridge
//0xBB from Bridge to other
//0xCC from Operator to Bridge

SoftwareSerial masterSerial(9, 11);

byte gStates[16]; //1 3 5

boolean monConnected = false;
boolean masterConnected = false;

unsigned long mConnectTime = 0;
unsigned long mDisconnectTime = 0;
unsigned long mLastSync = 0;
unsigned long mConnTimeOut = 20000;

int gCount = 16;

void setup() 
{
  masterSerial.begin(9600);
  Serial.begin(9600);
  pinMode(RSTXCNTRL, OUTPUT);
  digitalWrite(RSTXCNTRL, LOW);
  for (int s = 0; s < gCount; s++) gStates[s] = 0x01;
  connectToMonitor();
}

void loop()
{
  unsigned long tick = millis();
  // ==================== ПОДКЛЮЧЕНИЕ ====================
  if (!masterConnected)
  {
    unsigned long startConnect = tick;
    unsigned long whileTick = tick;
    byte inByte = 0;
    int connectWaitCount = 0;
    int connCount = 0;
    while (!masterConnected && (whileTick - startConnect < mConnTimeOut))
    {
      whileTick = millis();
      if (whileTick - (startConnect + connectWaitCount * 1000) > 1000 && inByte == 0)
      {
        connectWaitCount++;
        if (!monConnected) Serial.print(String(connectWaitCount) + "...");
      }
      if (masterSerial.available())
      {
        inByte = masterSerial.read();
        delay(15);
        if (inByte == 0xA9)
        {
          delay(350);
          while (masterSerial.available()) masterSerial.read();
          digitalWrite(RSTXCNTRL, HIGH);  // Init Transmitter
          digitalWrite(13, HIGH);
          masterSerial.write(0xBC);
          delay(15);
          digitalWrite(RSTXCNTRL, LOW);  // Stop Transmitter
          digitalWrite(13, LOW);
          Serial.println("Send reSync to Master");
        }
        else if (inByte == 0xA1 || inByte == 0xA2)
        {
          if (!monConnected) Serial.println("\nConnecting... recieved and sending back: " + String(inByte, HEX) + " in " + String(whileTick));
          digitalWrite(RSTXCNTRL, HIGH);  // Init Transmitter
          masterSerial.write(inByte);
          delay(15);
          digitalWrite(RSTXCNTRL, LOW);  // Stop Transmitter
        }
        if (inByte == 0xA2)
        {
          masterConnected = true;
          mLastSync = whileTick;
          Serial.println("masterConnected");
        }
      }
    }
    if (!masterConnected)
    {
      if (!monConnected) Serial.println("MASTER DISCONNECTED LONG TIME");
      else Serial.println("MDLT");
    }
  }
  else // Master Connected
  {
    // =========== КОМАНДЫ ОТ ОПЕРАТОРА ===========
    if (Serial.available() > 0)
    {
      String input = Serial.readStringUntil('\n');
      if (input.startsWith("CD")) // информация о гаджетах
      {
        for (int i = 0; i < gCount; i++)
        {
          if (input[i + 2] == '3') gStates[i] = 0x03;
          else gStates[i] = 0x01;
        }
        if (input.endsWith("FF")) // Данные приняты от оператора
        {
          // Prepare to send states to Master
          digitalWrite(RSTXCNTRL, HIGH);  // передаем состояния мастеру
          masterSerial.write(0xBD);
          delay(15);
          //Sending...
          for (int d = 0; d < gCount; d++)
          {
            digitalWrite(13, HIGH);
            masterSerial.write(gStates[d]);
            delay(15);
            digitalWrite(13, LOW);
          }
          masterSerial.write(0xFF); // конец передачи состояний мастеру
          delay(15);
          digitalWrite(RSTXCNTRL, LOW);  // Stop Transmitter
        }
      }
      else if (input.startsWith("CC")) // прием команды оператора на сброс состояний
      {
        digitalWrite(13, HIGH);
        // Prepare to send states to Master
        digitalWrite(RSTXCNTRL, HIGH);  // Init
        masterSerial.write(0xBC);
        delay(15);
        digitalWrite(RSTXCNTRL, LOW);  // Stop
        digitalWrite(13, LOW);
      }
      else Serial.flush();
    }
    // =========== КОМАНДЫ ОТ МАСТЕРА ===========
    if (masterSerial.available() > 0) //прием команд от мастера
    {
      
      byte inByte = masterSerial.read();

      if (inByte == 0xA1) // переподключение к мастеру
      {
        masterConnected = false;
        for (int s = 0; s < gCount; s++) gStates[s] = 0x01;
      }
      else if (inByte == 0xA9) mLastSync = tick; // контроль подключения (периодический сигнал синхронизации)
      else if (inByte == 0xAC) // сброс состояний со стороны мастера
      {
        for (int s = 0; s < gCount; s++) gStates[s] = 0x01;
        if (!monConnected) Serial.println("Send to operator clear states and run");
        Serial.write("masterStart\n");
      }
      else if (inByte == 0xAA) {Serial.println("Rungame");} //  старт игры
      else if (inByte == 0xAD) // Принимаем информацию о гаджетах с мастера
      {
        byte input[gCount];
        delay(350);
        if (!monConnected) Serial.print("Recieving data from master: ");
        for (int i = 0; i < gCount; i++)
        {
          input[i] = masterSerial.read();
          gStates[i] = input[i];
        }
        if (!monConnected) Serial.println();
        byte last = masterSerial.read();
        //Serial.println("Last byte is " + String(last));
        if (last == 0xFF)
        {
          if (!monConnected) Serial.print("Send to operator: "); // Отправляем информацию о гаджетах оператору
          Serial.write("BD");
          for (int d = 0; d < gCount; d++)
          {
            if (gStates[d] == 0x05) Serial.write("5");
            else if (gStates[d] == 0x03) Serial.write("3");
            else Serial.write("1");
          }
          Serial.write("FF\n");
          if (!monConnected) Serial.println(); // for test only
        }
      }
      while (masterSerial.available()) masterSerial.read();
      if (tick - mLastSync > mConnTimeOut) // при отсутствии контрольного сигнала переподключаемся к мастеру
      {
        digitalWrite(RSTXCNTRL, HIGH);  // Init Transmitter
        masterSerial.write(0xBC);
        delay(15);
        digitalWrite(RSTXCNTRL, LOW);  // Stop Transmitter
      }
    }
  }
}
