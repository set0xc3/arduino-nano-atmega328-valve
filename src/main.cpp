#include "Arduino.h"
#include "timer.hpp"

enum AppState
{
  Run,
  Win,
  GameOver
};

const uint8_t PIN_LED = 3;
const uint8_t PIN_WIN = 4;
const uint8_t PIN_GAME_OVER = 5;

AppState app_state = Run;

skat::Timer main_timer({0, 2000, 1000});
skat::Timer pwm_timer({0, 50, 50});

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_WIN, INPUT_PULLUP);
  pinMode(PIN_GAME_OVER, INPUT_PULLUP);
}

void loop()
{
  main_timer.update(millis());
  pwm_timer.update(millis());

  // Управление ШИМ через таймер
  if (main_timer.getState() == skat::TimerState::Run)
  {
    pwm_timer.start();

    if (pwm_timer.getState() == skat::TimerState::Run)
    {
      analogWrite(PIN_LED, 255); // 100% яркость
    }
    else
    {
      analogWrite(PIN_LED, 0); // 0% яркость
    }
  }
  else
  {
    analogWrite(PIN_LED, 0); // Выключить при остановке
  }

  // Обработка кнопок
  if (digitalRead(PIN_WIN) == LOW)
  {
    app_state = Win;
    main_timer.start();
  }

  if (digitalRead(PIN_GAME_OVER) == LOW)
  {
    app_state = GameOver;
    main_timer.drop();
  }

  if (app_state != Run)
  {
    app_state = Run;
  }
}