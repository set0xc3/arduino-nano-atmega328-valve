#include "timer.h"

void
timer_init(Timer *ctx, uint32_t wait, uint32_t run, uint32_t sleep,
           bool cyclic)
{
  ctx->state       = TIMER_STATE_WAITING;
  ctx->wait_delay  = wait;
  ctx->run_delay   = run;
  ctx->sleep_delay = sleep;
  ctx->is_cyclic   = cyclic;
}

void
timer_start(Timer *ctx)
{
  ctx->is_active = true;
}

void
timer_stop(Timer *ctx)
{
  ctx->is_active = false;
}

void
timer_update(Timer *ctx, uint32_t ms_counter)
{
  if (!ctx->is_active) {
    return;
  }

  uint32_t current_time = ms_counter;
  uint32_t elapsed      = current_time - ctx->start_time;

  switch (ctx->state) {
  case TIMER_STATE_WAITING:
    if (elapsed >= ctx->wait_delay) {
      ctx->total_time += elapsed;
      ctx->start_time = current_time;
      ctx->state      = TIMER_STATE_RUNNING;
    }
    break;

  case TIMER_STATE_RUNNING:
    if (elapsed >= ctx->run_delay) {
      ctx->total_time += elapsed;
      ctx->start_time = current_time;
      ctx->state      = TIMER_STATE_SLEEPING;
    }
    break;

  case TIMER_STATE_SLEEPING:
    if (elapsed >= ctx->sleep_delay) {
      ctx->total_time += elapsed;
      if (ctx->is_cyclic) {
        ctx->start_time = current_time;
        ctx->state      = TIMER_STATE_RUNNING;
      } else {
        ctx->state = TIMER_STATE_DONE;
      }
    }
    break;

  case TIMER_STATE_DONE:
    break;
  }
}

void
timer_restart(Timer *ctx, uint32_t counter_ms)
{
  ctx->state      = TIMER_STATE_WAITING;
  ctx->start_time = counter_ms;
  ctx->total_time = 0;
  ctx->is_active  = true;
}

void
timer_reset(Timer *ctx)
{
  ctx->state       = TIMER_STATE_WAITING;
  ctx->start_time  = 0;
  ctx->total_time  = 0;
  ctx->wait_delay  = 0;
  ctx->run_delay   = 0;
  ctx->sleep_delay = 0;
  ctx->is_cyclic   = false;
}