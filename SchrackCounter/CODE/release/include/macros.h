// Custom macros.


// Building in debug mode?
#define NAT_MODE_DEBUG(IF_CODE)                                                                                                \
  {                                                                                                                            \
    if(NATS_DEBUG == 1) {                                                                                                      \
      IF_CODE                                                                                                                  \
    }                                                                                                                          \
  }


// Using the sd card?
#define NAT_MODE_SD_CARD(IF_CODE)                                                                                              \
  {                                                                                                                            \
    if(NATS_SD_CARD == 1) {                                                                                                    \
      IF_CODE                                                                                                                  \
    }                                                                                                                          \
  }


// Timer channel, clock divisor (clock is 80MHz) (80 should make it returm micros), count up.
#define N_TIMER_BEGIN(N_TIMER_HANDLE, N_TIMER_CHANNEL, N_TIMER_DIVIDER)                                                        \
  {                                                                                                                            \
    N_TIMER_HANDLE = timerBegin(N_TIMER_CHANNEL, N_TIMER_DIVIDER, true);                                                       \
    timerStop(N_TIMER_HANDLE);                                                                                                 \
  }


// Start a timer.
#define N_TIMER_START(N_TIMER_HANDLE)                                                                                          \
  {                                                                                                                            \
    timerRestart(N_TIMER_HANDLE);                                                                                              \
    timerStart(N_TIMER_HANDLE);                                                                                                \
  }


// Read and reset a timer.
#define N_TIMER_READ(N_TIMER_HANDLE, readMicros)                                                                               \
  {                                                                                                                            \
    readMicros = timerReadMicros(N_TIMER_HANDLE);                                                                              \
    timerStop(N_TIMER_HANDLE);                                                                                                 \
    timerRestart(N_TIMER_HANDLE);                                                                                              \
    timerStart(N_TIMER_HANDLE);                                                                                                \
  }


// Stop a timer.
#define N_TIMER_STOP(N_TIMER_HANDLE)                                                                                           \
  { timerStop(N_TIMER_HANDLE); }


// Fast pin mode for gpio 9.
#define N_BZR_SETUP                                                                                                            \
  { REG_WRITE(GPIO_ENABLE_REG, BIT9); }


// Fast write to gpio 9. (!1|1) - (LOW|HIGH).
#define N_BZR_WRITE(N_VAL)                                                                                                     \
  {                                                                                                                            \
    if(N_VAL == 1) {                                                                                                           \
      REG_WRITE(GPIO_OUT_W1TS_REG, BIT9);                                                                                      \
    } else {                                                                                                                   \
      REG_WRITE(GPIO_OUT_W1TC_REG, BIT9);                                                                                      \
    }                                                                                                                          \
  }
