#include "Arduino.h"
#include "timer.hpp"

#include "DFRobotDFPlayerMini.h"

#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/10, /*tx =*/11);
#define FPSerial softSerial

DFRobotDFPlayerMini myDFPlayer;

enum AppState
{
  Run,
  Win,
  GameOver
};

const uint8_t PIN_LED_RED = PIND2;
const uint8_t PIN_LED_GREEN = PIND3;
const uint8_t PIN_WIN = PIND4;
const uint8_t PIN_GAME_OVER = PIND5;

AppState app_state = Run;

skat::Timer win_timer({0, 2000, 500});
skat::Timer game_over_timer({0, 2000, 500});
skat::Timer pwm_timer({0, 80, 80});

void setup()
{
  FPSerial.begin(9600);
  // Serial.begin(9600);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_WIN, INPUT_PULLUP);
  pinMode(PIN_GAME_OVER, INPUT_PULLUP);

  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true))
  {
    // Serial.println(F("Unable to begin:"));
    while (true)
    {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  // Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(5); // Set volume value. From 0 to 30
}

void loop()
{
  // win_timer.update(millis());
  // game_over_timer.update(millis());
  // pwm_timer.update(millis());

  // // Управление ШИМ через таймер
  // if (win_timer.getState() == skat::TimerState::Run)
  // {
  //   game_over_timer.drop();
  //   pwm_timer.start();

  //   if (pwm_timer.getState() == skat::TimerState::Run)
  //   {
  //     analogWrite(PIN_LED_GREEN, 255); // 100% яркость
  //   }
  //   else
  //   {
  //     analogWrite(PIN_LED_GREEN, 0); // 0% яркость
  //   }
  // }
  // else if (win_timer.getState() == skat::TimerState::Stop)
  // {
  //   analogWrite(PIN_LED_GREEN, 0);
  // }

  // if (game_over_timer.getState() == skat::TimerState::Run)
  // {
  //   win_timer.drop();
  //   pwm_timer.start();

  //   if (pwm_timer.getState() == skat::TimerState::Run)
  //   {
  //     analogWrite(PIN_LED_RED, 255); // 100% яркость
  //   }
  //   else
  //   {
  //     analogWrite(PIN_LED_RED, 0); // 0% яркость
  //   }
  // }
  // else if (game_over_timer.getState() == skat::TimerState::Stop)
  // {
  //   analogWrite(PIN_LED_RED, 0);
  // }

  // Обработка кнопок
  if (digitalRead(PIN_WIN) == LOW)
  {
    if (app_state != Win)
    {
      app_state = Win;
      win_timer.start();

      myDFPlayer.play(2);
    }
  }

  if (digitalRead(PIN_GAME_OVER) == LOW)
  {
    if (app_state != GameOver)
    {
      app_state = GameOver;
      game_over_timer.start();
      myDFPlayer.play(3);
    }
  }

  if (app_state != Run)
  {
    app_state = Run;
  }
}