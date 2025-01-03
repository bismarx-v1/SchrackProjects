// Custom timer macros.

/**
 * @param N_TIMER_HANDLE  Timer handle, created with "hw_timer_t* timerHandle = NULL;".
 * @param N_TIMER_CHANNEL Timer channel (0-15).
 * @param N_TIMER_DIVIDER Clock divider. Default clock speed is 80MHz.
 * @brief Creates and stops a count-up timer.
 */
#define N_TIMER_BEGIN(N_TIMER_HANDLE, N_TIMER_CHANNEL, N_TIMER_DIVIDER)                                                        \
  {                                                                                                                            \
    N_TIMER_HANDLE = timerBegin(N_TIMER_CHANNEL, N_TIMER_DIVIDER, true);                                                       \
    timerStop(N_TIMER_HANDLE);                                                                                                 \
  }


/**
 * @param N_TIMER_HANDLE Timer handle.
 * @brief Start a timer.
 */
#define N_TIMER_START(N_TIMER_HANDLE)                                                                                          \
  {                                                                                                                            \
    timerStart(N_TIMER_HANDLE);                                                                                                \
  }


/**
 * @param N_TIMER_HANDLE Timer handle.
 * @brief Start a timer.
 */
#define N_TIMER_RESTART(N_TIMER_HANDLE)                                                                                          \
  {                                                                                                                            \
    timerRestart(N_TIMER_HANDLE);                                                                                              \
    timerStart(N_TIMER_HANDLE);                                                                                                \
  }


/**
 * @param N_TIMER_HANDLE Timer handle.
 * @brief Reset a timer to 0.
 */
#define N_TIMER_RESET(N_TIMER_HANDLE)                                                                                          \
  {                                                                                                                            \
    timerRestart(N_TIMER_HANDLE);                                                                                                \
  }


/**
 * @param N_TIMER_HANDLE Timer handle.
 * @param N_READ_VAL Var to store the value to (uint64_t).
 * @param N_DIVIDER Secondary divider.
 * @brief Read a value from the timer.
 */
#define N_TIMER_READ(N_TIMER_HANDLE, N_READ_VAL, N_DIVIDER)                                                                    \
  { N_READ_VAL = timerReadMicros(N_TIMER_HANDLE) / N_DIVIDER; }


/**
 * @param N_TIMER_HANDLE Timer handle.
 * @param N_WRITE_VAL Val to write to the timer (uint64_t).
 * @param N_MULTIPLIER Secondary multiplier.
 * @brief Write a value to the timer.
 */
#define N_TIMER_WRITE(N_TIMER_HANDLE, N_WRITE_VAL, N_MULTIPLIER)                                                                             \
  { timerWrite(N_TIMER_HANDLE, (N_WRITE_VAL * N_MULTIPLIER * 80) / timerGetDivider(N_TIMER_HANDLE)); }


/**
 * @param N_TIMER_HANDLE Timer handle.
 * @brief Reastarts a timer.
 */
#define N_TIMER_STOP_RESTART(N_TIMER_HANDLE)                                                                                        \
  {                                                                                                                            \
    timerStop(N_TIMER_HANDLE);                                                                                                 \
    timerRestart(N_TIMER_HANDLE);                                                                                              \
    timerStart(N_TIMER_HANDLE);                                                                                                \
  }


/**
 * @param N_TIMER_HANDLE Timer handle.
 * @brief Stops a timer.
 */
#define N_TIMER_STOP(N_TIMER_HANDLE)                                                                                           \
  { timerStop(N_TIMER_HANDLE); }
