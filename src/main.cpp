/*
  
*/

enum AppState {
  Initializing,
  Running,
  Win,
  GameOver
};

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/10, /*tx =*/11);
#define FPSerial softSerial

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int digitalPinWin = PIND4;
int digitalPinGameOver = PIND5;

void setup()
{
  pinMode(digitalPinWin, INPUT_PULLUP);
  pinMode(digitalPinGameOver, INPUT_PULLUP);

  Serial.begin(9600);

  Serial.println();
  Serial.println(F("Initializing..."));

  // if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true))
  // {
  //   Serial.println(F("Unable to begin:"));
  //   while (true)
  //   {
  //     delay(0); // Code to compatible with ESP8266 watch dog.
  //   }
  // }
  // Serial.println(F("DFPlayer Mini online."));

  // myDFPlayer.volume(10); // Set volume value. From 0 to 30
  // myDFPlayer.play(1);    // Play the first mp3
}

void loop()
{
  if (digitalRead(digitalPinWin) == LOW) {
    Serial.println("НWin!");
  } else  if (digitalRead(digitalPinGameOver) == LOW) {
    Serial.println("GameOver!");
  }

  // delay(100);

  //  static unsigned long timer = millis();

  //  if (millis() - timer > 5000) {
  //    timer = millis();
  //    myDFPlayer.next();  //Play next mp3 every 3 second.
  //  }

  // if (myDFPlayer.available())
  // {
  //   printDetail(myDFPlayer.readType(), myDFPlayer.read()); // Print the detail message from DFPlayer to handle different errors and states.
  // }
}

void printDetail(uint8_t type, int value)
{
  switch (type)
  {
  case TimeOut:
    Serial.println(F("Time Out!"));
    break;
  case WrongStack:
    Serial.println(F("Stack Wrong!"));
    break;
  case DFPlayerCardInserted:
    Serial.println(F("Card Inserted!"));
    break;
  case DFPlayerCardRemoved:
    Serial.println(F("Card Removed!"));
    break;
  case DFPlayerCardOnline:
    Serial.println(F("Card Online!"));
    break;
  case DFPlayerUSBInserted:
    Serial.println("USB Inserted!");
    break;
  case DFPlayerUSBRemoved:
    Serial.println("USB Removed!");
    break;
  case DFPlayerPlayFinished:
    Serial.print(F("Number:"));
    Serial.print(value);
    Serial.println(F(" Play Finished!"));
    break;
  case DFPlayerError:
    Serial.print(F("DFPlayerError:"));
    switch (value)
    {
    case Busy:
      Serial.println(F("Card not found"));
      break;
    case Sleeping:
      Serial.println(F("Sleeping"));
      break;
    case SerialWrongStack:
      Serial.println(F("Get Wrong Stack"));
      break;
    case CheckSumNotMatch:
      Serial.println(F("Check Sum Not Match"));
      break;
    case FileIndexOut:
      Serial.println(F("File Index Out of Bound"));
      break;
    case FileMismatch:
      Serial.println(F("Cannot Find File"));
      break;
    case Advertise:
      Serial.println(F("In Advertise"));
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}
