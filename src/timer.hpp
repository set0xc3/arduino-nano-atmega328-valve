#ifndef SKAT_TIMER_H
#define SKAT_TIMER_H

#include <stdbool.h>
#include <stdint.h>

namespace skat
{
  enum TimerState
  {
    Stop,
    Wait,
    Run,
    Sleep,
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
    Timer(TimerInterval timer);

    TimerState get_state() const { return m_state; }

    void start();
    void update(uint32_t ms_counter);
    void drop();

  private:
    TimerState m_state;
    TimerInterval m_interval;
    uint32_t m_start_time;
    bool m_is_active;
  };
}

#endif