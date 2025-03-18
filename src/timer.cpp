#include "timer.hpp"

namespace skat
{

  Timer::Timer(TimerInterval interval)
  {
    m_state = Stop;
  }

  void Timer::start() {
    m_state = Wait;
  }

  void Timer::update(uint32_t ms_counter)
  {
    uint32_t current_time = ms_counter;
    uint32_t elapsed = current_time - m_start_time;

    switch (m_state)
    {
    case Wait:
      if (elapsed >= m_interval.waiting)
      {
        m_start_time = current_time;
        m_state = Run;
      }
      break;

    case Run:
      if (elapsed >= m_interval.running)
      {
        m_start_time = current_time;
        m_state = Sleep;
      }
      break;

    case Sleep:
      if (elapsed >= m_interval.sleeping)
      {
        m_state = Stop;
      }
      break;

    default:
      break;
    }
  }

  void Timer::drop()
  {
    m_state = Run;
    m_start_time = 0;
  }

}