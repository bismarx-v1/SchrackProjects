// Custom macros.

// Building in debug mode?
#define NAT_MODE_DEBUG(IF_CODE)                                                                                                    \
  {                                                                                                                            \
    if(NATS_DEBUG == 1) {                                                                                                      \
      IF_CODE                                                                                                                  \
    }                                                                                                                          \
  }


#define NAT_MODE_SD_CARD(IF_CODE)                                                                                                    \
  {                                                                                                                            \
    if(NATS_SD_CARD == 1) {                                                                                                      \
      IF_CODE                                                                                                                  \
    }                                                                                                                          \
  }
