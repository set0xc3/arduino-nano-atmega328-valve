#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  TIMER_STATE_WAITING,
  TIMER_STATE_RUNNING,
  TIMER_STATE_SLEEPING,
  TIMER_STATE_DONE
} TimerState;

typedef struct {
  TimerState state;
  uint32_t   start_time;
  uint32_t   total_time;
  uint32_t   wait_delay;
  uint32_t   run_delay;
  uint32_t   sleep_delay;
  bool       is_cyclic;
  bool       is_active;
} Timer;

void timer_init(Timer *ctx, uint32_t wait, uint32_t run, uint32_t sleep,
                bool cyclic);
void timer_start(Timer *ctx);
void timer_stop(Timer *ctx);
void timer_update(Timer *ctx, uint32_t ms_counter);
void timer_restart(Timer *ctx, uint32_t ms_counter);
void timer_reset(Timer *ctx);

#endif