// Custom fast write macros.

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
