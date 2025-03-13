/*

*/

#include "Arduino.h"
#include "timer.h"

enum AppState
{
  Running,
  Win,
  GameOver
};

uint8_t digital_pin_led_a = PIND3;
uint8_t digital_pin_win = PIND4;
uint8_t digital_pin_game_over = PIND5;

AppState app_state = {};
Timer timer_a = {};

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("Setup..."));

  timer_start(&timer_a);

  pinMode(digital_pin_led_a, OUTPUT);
  pinMode(digital_pin_win, INPUT_PULLUP);
  pinMode(digital_pin_game_over, INPUT_PULLUP);

  digitalWrite(digital_pin_led_a, HIGH);

  app_state = Running;
  Serial.println(F("Setup..."));
}

void loop()
{
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

  // Логика зависит от состояний пинов
  //
  switch (app_state)
  {
  case Running:
    break;
  case Win:
    Serial.println("Win!");
    break;
  case GameOver:
    Serial.println("GameOver!");
    break;
  }
}