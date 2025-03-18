/*

*/

#include "Arduino.h"
#include "timer.hpp"

enum AppState
{
  Run,
  Win,
  GameOver
};

uint8_t digital_pin_led_a = PIND3;
uint8_t digital_pin_win = PIND4;
uint8_t digital_pin_game_over = PIND5;

AppState app_state = {};

static skat::Timer timer_a;

void setup()
{
  Serial.begin(9600);
  Serial.println();

  pinMode(digital_pin_led_a, OUTPUT);
  pinMode(digital_pin_win, INPUT_PULLUP);
  pinMode(digital_pin_game_over, INPUT_PULLUP);

  // app_state = Run;
  Serial.println(F("Setup..."));

  skat::TimerInterval interval = {0, 1000, 0};
  timer_a = skat::Timer(interval);
}

void loop()
{
  timer_a.update(millis());
  if (timer_a.get_state() == skat::TimerState::Run) {
    Serial.println(F("TimerState::Run"));
  }

  // Проверка состояний пинов
  //
  if (digitalRead(digital_pin_win) == LOW)
  {
    app_state = Win;
  }

  if (digitalRead(digital_pin_game_over) == LOW)
  {
    app_state = GameOver;
  }

  // if (timer_a.get_state() == TimerState::RUNNING) {
  //   Serial.println(F("COMPLETED"));
  // }

  // if (timer_a.state == TIMER_STATE_RUNNING)
  // {
  //   Serial.println(F("TIMER_STATE_RUNNING"));
  //   digitalWrite(digital_pin_led_a, HIGH);
  // }
  // if (timer_a.state == TIMER_STATE_DONE)
  // {
  //   digitalWrite(digital_pin_led_a, LOW);
  //   Serial.println(F("TIMER_STATE_DONE"));
  //   timer_stop(&timer_a);
  // }

  // Логика зависит от состояний пинов
  //
  switch (app_state)
  {
  case Run:
    break;
  case Win:
    // Serial.println("Win!");
    app_state = Run;
    timer_a.drop();
    break;
  case GameOver:
    // Serial.println("GameOver!");
    app_state = Run;
    break;
  }
}