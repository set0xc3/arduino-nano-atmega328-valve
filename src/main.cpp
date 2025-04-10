#include "Arduino.h"
#include "timer.hpp"

#include "DFRobotDFPlayerMini.h"

#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/10, /*tx =*/11);
#define FPSerial softSerial

DFRobotDFPlayerMini myDFPlayer;

class Button
{
public:
  void update()
  {
    m_last_state = m_state;
  }

  void on_event(bool is_state)
  {
    m_state = is_state;
  }

  bool is_pressed()
  {
    return !m_last_state && m_state;
  }

  bool is_release()
  {
    return m_last_state && !m_state;
  }

  bool is_down()
  {
    return m_last_state && m_state;
  }

private:
  bool m_state;
  bool m_last_state;
};

enum AppState
{
  NoGame,
  GameRun,
  GameWin,
  GameOver
};

const uint8_t PIN_LED_RED_OUT = PIND2;
const uint8_t PIN_LED_GREEN_OUT = PIND3;

const uint8_t PIN_WIN_IN = PIND4;
const uint8_t PIN_GAME_OVER_IN = PIND5;

const uint8_t MASTER_BUTTON_PIN = PIN_A0;

Button button;

AppState app_state = NoGame;

void setup()
{
  FPSerial.begin(9600);
  Serial.begin(9600);

  pinMode(PIN_LED_RED_OUT, OUTPUT);
  pinMode(PIN_LED_GREEN_OUT, OUTPUT);

  pinMode(PIN_WIN_IN, INPUT);
  pinMode(PIN_GAME_OVER_IN, INPUT);

  pinMode(MASTER_BUTTON_PIN, INPUT);

  Serial.println(F("Setup..."));
}

void loop()
{
  button.update();

  if (digitalRead(MASTER_BUTTON_PIN) == HIGH)
  {
    button.on_event(true);
  }
  else if (digitalRead(MASTER_BUTTON_PIN) == LOW)
  {
    button.on_event(false);
  }

  switch (app_state)
  {
  case NoGame:
  {
    Serial.println(F("Не играем"));

    if (button.is_release())
    {
      app_state = GameRun;
    }
  }
  break;
  case GameRun:
  {
    Serial.println(F("Играем"));

    if (button.is_release())
    {
      app_state = NoGame;
      Serial.println(F("Drop"));
      break;
    }

    if (digitalRead(PIN_WIN_IN) == LOW)
    {
      app_state = GameWin;
    }
    else if (digitalRead(PIN_GAME_OVER_IN) == LOW)
    {
      app_state = GameOver;
    }
  }
  break;
  case GameWin:
  {
    Serial.println(F("GameWin"));

    if (button.is_release())
    {
      app_state = NoGame;
      Serial.println(F("Drop"));
    }
  }
  break;
  case GameOver:
  {
    Serial.println(F("GameOver"));

    if (button.is_release())
    {
      app_state = NoGame;
      Serial.println(F("Drop"));
    }
  }
  break;
  }
}