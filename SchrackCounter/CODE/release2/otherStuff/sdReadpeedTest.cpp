/**
 * Target 125us
 */
/*
files on sd:
/alexiane-aMillionOnMySoul.wav.NAT1BIT
/contextSensitive-hifiChiptuneNotToStudyTo.wav.NAT1BIT
/contextSensitive-nice.wav.NAT1BIT
/norfair-purpleNotions.wav.NAT1BIT
/s3rl-nekoNationAnthem.wav.NAT1BIT
/simonViklund-iWillGiveYouMyAll.wav.NAT1BIT
*/
/**
 * CHECK:
 * TODO:
 * FIX:
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "schrackButtonPins.h"
#include "macrosTimersV2.h"


// Timer.
hw_timer_t* timerHandle = NULL;

void setup() {
  // Serial.
  Serial.begin(115200);
  delay(2000);

  // Sd card setup.
  //SPI.begin(GPIO_SPI_CLK, GPIO_SPI_MISO, GPIO_SPI_MOSI, GPIO_SPI_CS_SDCARD);
  SPI.begin(12, 13, 11, 10);
  while(!SD.begin(GPIO_SPI_CS_SDCARD, SPI)) {
    log_e("Sd card begin error. Next att in 1s.");
    delay(1000);
  }
  
  // Open the file.
  File wavFile = SD.open("/n-pN.BIT", FILE_READ);

  // Set up the timer.
  N_TIMER_BEGIN(timerHandle, 0, 2)  // Timer ticks every 012.5 * 2 (25) ns.

  // Read the file params.
  uint64_t wavFileSizeFullBytes = 0;
  uint8_t wavFileSizePartialBytes = 0;
  uint64_t wavFileSampleRate = 0;
  wavFile.readBytes((char*)&wavFileSizeFullBytes, 4);
  wavFile.readBytes((char*)&wavFileSizePartialBytes, 1);
  wavFile.readBytes((char*)&wavFileSampleRate, 4);

  // Check if 8kHz.
  if(wavFileSampleRate != 8000) {
    wavFile.close();
    Serial.println("Not 8kHz");
    return;
  }


  uint32_t readHeadPos = 0;
  uint8_t readByte = 0;
  uint8_t bitToPlay = 0;
  uint8_t readHeadsubBytePos = 0;
  uint64_t timerVal = 0;


  N_TIMER_RESET(timerHandle)
  N_TIMER_START(timerHandle)
  readHeadPos = wavFile.position();
  wavFile.readBytes((char*)readByte, 1);
  wavFile.seek(readHeadPos);
  bitToPlay = (readHeadPos >> readHeadsubBytePos) & 1;
  N_TIMER_STOP(timerHandle)
  N_TIMER_READ(timerHandle, timerVal, 1)

  Serial.println(timerVal);
}


void loop() {
  delay(1000);
}
