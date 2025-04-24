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

static const uint8_t PIN_LED_RED_OUT = PIND2;
static const uint8_t PIN_LED_GREEN_OUT = PIND3;

static const uint8_t PIN_WIN_IN = PIND4;
static const uint8_t PIN_GAME_OVER_IN = PIND5;

static const uint8_t MASTER_BUTTON_PIN = PIN_A0;

static Button master_button;

static AppState app_state = NoGame;
static skat::Timer leds_timer({0, 500, 500});

static bool is_play_sound = false;

void setup()
{
  Serial.println(F("Setup..."));

  FPSerial.begin(9600);
  // Serial.begin(9600);

  pinMode(PIN_LED_RED_OUT, OUTPUT);
  pinMode(PIN_LED_GREEN_OUT, OUTPUT);

  pinMode(PIN_WIN_IN, INPUT);
  pinMode(PIN_GAME_OVER_IN, INPUT);

  pinMode(MASTER_BUTTON_PIN, INPUT);

  Serial.println();
  Serial.println("DFPlayer Mini Demo");
  Serial.println("Initializing DFPlayer...");
  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true))
  {
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
    while (true)
      ;
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(10); // 0-30 Value
}

void loop()
{
  master_button.update();

  if (digitalRead(MASTER_BUTTON_PIN) == HIGH)
  {
    master_button.on_event(true);
  }
  else if (digitalRead(MASTER_BUTTON_PIN) == LOW)
  {
    master_button.on_event(false);
  }

  switch (app_state)
  {
  case NoGame:
  {
    Serial.println(F("Не играем"));

    leds_timer.start();
    leds_timer.update(millis());

    if (leds_timer.getState() == skat::TimerState::Run)
    {
      digitalWrite(PIN_LED_RED_OUT, true);
      digitalWrite(PIN_LED_GREEN_OUT, false);
    }
    else if (leds_timer.getState() == skat::TimerState::Sleep)
    {
      digitalWrite(PIN_LED_RED_OUT, false);
      digitalWrite(PIN_LED_GREEN_OUT, true);
    }

    if (master_button.is_release())
    {
      app_state = GameRun;

      digitalWrite(PIN_LED_RED_OUT, false);
      digitalWrite(PIN_LED_GREEN_OUT, false);
    }
  }
  break;
  case GameRun:
  {
    Serial.println(F("Играем"));

    if (master_button.is_release())
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
    digitalWrite(PIN_LED_GREEN_OUT, true);

    if (!is_play_sound)
    {
      is_play_sound = true;
      myDFPlayer.play(2);
    }

    if (master_button.is_release())
    {
      is_play_sound = false;
      app_state = NoGame;
      Serial.println(F("Drop"));
    }
  }
  break;
  case GameOver:
  {
    Serial.println(F("GameOver"));
    digitalWrite(PIN_LED_RED_OUT, true);

    if (!is_play_sound)
    {
      is_play_sound = true;
      myDFPlayer.play(3);
    }

    if (master_button.is_release())
    {
      is_play_sound = false;
      app_state = NoGame;
      Serial.println(F("Drop"));
    }
  }
  break;
  }
}