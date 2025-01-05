/**
 * TODO:
 * read a file
 */

/**
 * plays a .wav file from the sd card.
 * 
 * the wav file is defined as 8000 Hz unsigned 8bit pcm
 */


#include <Arduino.h>
#include "macros.h"
#include "circularBuffer.h"
#define IS_PROTOTYPE_LAYOUT 0
#include "sdCardStuffForMusic.h"

#define WAV_FILENAME "/nya.wav"
#define WAV_START_FROM_BYTE 44



// Buffer.
//circularBuffer1bit8 circBfrObj(8000);  // Circular buffer object.
circularBuffer1bit8 circBfrObj(8000);  // Circular buffer object.
// Buffer read.
hw_timer_t*           timerHandle = NULL;  // Circular buffer read head callback handle.
static void IRAM_ATTR timerCallback();     // Circular buffer read head callback.
// Buffer write.
#define CONST_CIRC_BUFFER_WRITE_HEAD_TASK_STACK_SIZE 16000  // Circular buffer write head task stack size.
TaskHandle_t          circBufferWriteHeadFuncHandle;       // Circular buffer write head task handle.
static void IRAM_ATTR circBufferWriteHeadFunc(void*);      // Circular buffer write head task.


/**
 * @brief Sets up the music stuff. Plays music from a file at 8kHz.
 */
void wavMusicSetup(const char* filename) {
  N_BZR_SETUP
  N_TIMER_BEGIN(timerHandle, 0, 8)                          // Set timer to count seconds(e-7).
  timerAttachInterrupt(timerHandle, &timerCallback, true);  // Attach the interrupt.
  timerAlarmWrite(timerHandle, 1250, true);                 // Set trigger to every 125us (= 8kHz). Auto reset = true.
  timerAlarmEnable(timerHandle);                            // Enable the interrupt.


  // Driver task |Task func |Name |Stack size |Parameter of the task |Priority |Task handle |Core
  xTaskCreatePinnedToCore(circBufferWriteHeadFunc, "circBufferWriteHeadFunc", CONST_CIRC_BUFFER_WRITE_HEAD_TASK_STACK_SIZE,
                          (void*)filename, 1, &circBufferWriteHeadFuncHandle, 1);  // Create the circular buffer write head task.
}


/**
 * @brief Takes vals from the buffer and writes them to a gpio pin.
 */
void IRAM_ATTR timerCallback() {
  uint8_t tempVal;
  if(circBfrObj.read(&tempVal) != 0) {
    //log_e("Buffer empty");
  } else {
    N_BZR_WRITE(tempVal)
  }
 
  /*static uint8_t tempVal = 0;
  tempVal = tempVal ^ 1;
  N_BZR_WRITE(tempVal)*/
}


/**
 * @brief Stops playing sound.
 */
void wavMusicStop() {
  N_TIMER_STOP(timerHandle)  // Stop the timer.
}


/** 
 * @brief Periadically check if the buffer has any empty space and fills it.
*/
void IRAM_ATTR circBufferWriteHeadFunc(void* params) {
  static char* filename = (char*)params;
  static File wavFile = SD.open(filename, FILE_READ);

  static uint8_t setupVar = 0;
  if(setupVar == 0) {
    setupVar = 1;
    wavFile.seek(WAV_START_FROM_BYTE);
  }
  
  uint8_t tempVal = 0;

  while(1) {
    if(wavFile.readBytes((char*)&tempVal, 1) == -1) { // Read a byte.
      log_e("endOfFile");
      break;
    }

    if(tempVal > 0xbf) {  // Vals go from 0x0 to 0xff, zero is at 0x80. 0xbf is rounded 0xff / 4 * 3.
      tempVal = 1;
    } else {
      tempVal = 0;
    }

    while(circBfrObj.write(tempVal) == 1) {
      taskYIELD();
    }
  }

  wavFile.close();
  wavMusicStop();
}


/**
 * @brief Starts playing sound.
 */
void wavMusicPlay() {
  N_TIMER_START(timerHandle)  // Start the timer.
}


void setup() {
  String  filenameString = WAV_FILENAME;
  uint8_t filenameLen    = filenameString.length() + 1; // Include the \0.
  char    filename[filenameLen];
  filenameString.toCharArray(filename, filenameLen);

  Serial.begin(115200);  // Serial
  delay(2000);

  sdSetup(GPIO_SPI_CLOCK, GPIO_SPI_MISO, GPIO_SPI_MOSI, GPIO_SPI_SEL_SDCARD);
  if (checkFile(filename) != 0) {
    while(1) {
      log_e("Not a file, or not exists.");
      delay(1000);
    }
  }

  
  
  wavMusicSetup(filename);       // Setup music.
  Serial.println("Setup done");
  delay(2000);
  Serial.println("START");
  //wavMusicPlay();  // Start music.
  delay(2000);
  //wavMusicStop();  // Stop music.
  Serial.println("main done");
}


void loop() {
  delay(1000);
}
