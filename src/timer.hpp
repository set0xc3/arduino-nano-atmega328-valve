#ifndef SKAT_TIMER_H
#define SKAT_TIMER_H

#include <stdbool.h>
#include <stdint.h>

namespace skat
{

  enum class TimerState
  {
    Stop,
    Wait,
    Run,
    Sleep
  };

  struct TimerInterval
  {
    uint32_t waiting;
    uint32_t running;
    uint32_t sleeping;
  };

  class Timer
  {
  public:
    Timer() = default;

    explicit Timer(const TimerInterval &interval)
        : m_interval(interval), m_state(TimerState::Stop), m_start_time(0) {}

    void start()
    {
      if (m_state == TimerState::Stop)
      {
        m_state = TimerState::Wait;
        m_start_time = 0; // Время будет установлено при первом update
      }
    }

    void update(uint32_t current_time)
    {
      if (m_state == TimerState::Stop)
        return;

      if (m_start_time == 0)
      {
        m_start_time = current_time; // Инициализация при первом вызове
        return;
      }

      const uint32_t elapsed = current_time - m_start_time;

      switch (m_state)
      {
      case TimerState::Wait:
        if (elapsed >= m_interval.waiting)
        {
          m_state = TimerState::Run;
          m_start_time = current_time;
        }
        break;

      case TimerState::Run:
        if (elapsed >= m_interval.running)
        {
          m_state = m_interval.sleeping > 0 ? TimerState::Sleep : TimerState::Stop;
          m_start_time = current_time;
        }
        break;

      case TimerState::Sleep:
        if (elapsed >= m_interval.sleeping)
        {
          m_state = TimerState::Stop;
        }
        break;

      default:
        break;
      }
    }

    void reset()
    {
      m_state = TimerState::Stop;
      m_start_time = 0;
    }

    TimerState get_state() const { return m_state; }

  private:
    TimerInterval m_interval{};
    TimerState m_state{TimerState::Stop};
    uint32_t m_start_time{0};
  };

}

#endif