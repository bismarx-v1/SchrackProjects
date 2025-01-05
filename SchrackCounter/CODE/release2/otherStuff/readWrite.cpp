#include "sdCardStuff.h"

void setup() {
  IF_SERIAL(Serial.begin(115200);)
  delay(2000);

  sdSetup(PIN_SPI_SD_CS);

  // File.
  String  filenameString = "/temp.csv";
  uint8_t filenameLen    = filenameString.length() + 1; // Include the \0.
  char    filename[filenameLen];
  filenameString.toCharArray(filename, filenameLen);

  String  val1String = "b2";
  uint8_t val1Len    = val1String.length() + 1; // Include the \0. It will be removed quite soon.
  char    val1[val1Len];
  val1String.toCharArray(val1, val1Len);

  String  val2String = "mrrrp mrrow";
  uint8_t val2Len    = val2String.length() + 1; // Include the \0. It will be removed quite soon.
  char    val2[val2Len];
  val2String.toCharArray(val2, val2Len);

  String  val3String = "nyew cell :3";
  uint8_t val3Len    = val3String.length() + 1; // Include the \0. It will be removed quite soon.
  char    val3[val3Len];
  val3String.toCharArray(val3, val3Len);

  uint16_t bufferReadLen[1] = {32};  // Create a buffer to write to.
  uint8_t  bufferRead[bufferReadLen[0]];
  memset(bufferRead, 0, bufferReadLen[0]);


  writeCell(filename, filenameLen, 0, 1, val1, val1Len, 0); // Write a shorter val.

  writeCell(filename, filenameLen, 0, 2, val2, val2Len, 0); // Write a longer val.

  writeCell(filename, filenameLen, 1, 3, val3, val3Len, 1); // Write a new val.

  readCell(filename, 1, 2, bufferRead, bufferReadLen);  // Read a cell.

  Serial.println("Cell value START");  // Print the cell out.
  for(uint16_t i = 0; i < bufferReadLen[0]; i++) {
    Serial.print((char)bufferRead[i]);
  }
  Serial.print("\n");
  Serial.println("Cell value END");


  return;
}

void loop() {
  delay(100);
}
