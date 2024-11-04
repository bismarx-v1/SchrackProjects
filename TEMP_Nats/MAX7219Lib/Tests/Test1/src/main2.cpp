#include <Arduino.h>

#define MAX7219_Data_IN 5
#define MAX7219_Chip_Select  17
#define MAX7219_Clock 16

#define OP_NOOP   0x00
#define OP_DIGIT0 0x01
#define OP_DIGIT1 0x02
#define OP_DIGIT2 0x03
#define OP_DIGIT3 0x04
#define OP_DIGIT4 0x05
#define OP_DIGIT5 0x06
#define OP_DIGIT6 0x07
#define OP_DIGIT7 0x08
#define OP_DECODEMODE  0x09
#define OP_INTENSITY   0x0a
#define OP_SCANLIMIT   0x0b
#define OP_SHUTDOWN    0x0c
#define OP_DISPLAYTEST 0x0f

void shift(byte send_to_address, byte send_this_data)
{
  digitalWrite(MAX7219_Chip_Select, LOW);
  shiftOut(MAX7219_Data_IN, MAX7219_Clock, MSBFIRST, send_to_address);
  shiftOut(MAX7219_Data_IN, MAX7219_Clock, MSBFIRST, send_this_data);
  digitalWrite(MAX7219_Chip_Select, HIGH);
}

void setup() {
  pinMode(MAX7219_Data_IN, OUTPUT);
  pinMode(MAX7219_Chip_Select, OUTPUT);
  pinMode(MAX7219_Clock, OUTPUT);
  digitalWrite(MAX7219_Clock, HIGH);
  delay(200);

  //Setup of MAX7219 chip
  shift(OP_DISPLAYTEST, 0x00); //display test register - test mode off
  shift(OP_SHUTDOWN   , 0x01); //shutdown register - normal operation
  shift(OP_SCANLIMIT  , 0x00); //scan limit register - display digits 0 thru 4
  shift(OP_INTENSITY  , 0x0f); //intensity register - max brightness
  shift(OP_DECODEMODE , 0x00); //decode mode register - no decode all digits
  delay(200);
  



  /*
          A
        F   B
          G
        E   C
          D   p

                     pABCDEFG*/
  shift(OP_DIGIT0, 0b00000001);
  //shift(OP_DIGIT1, 0b00110000);
  //shift(OP_DIGIT2, 0b01111111);
  //shift(OP_DIGIT3, 0b01111111);
}

void loop() {
}

#if 0
void loop() {
  // Display Test on
  Serial.println("Display Test");
  shift(OP_DISPLAYTEST , 0x01); // Display test on
  delay(2000);
  shift(OP_DISPLAYTEST , 0x00); // Display test off

  // Display Test on
  Serial.println("Shutdown");
  shift(OP_SHUTDOWN , 0x00); // Display test on
  delay(2000);
  shift(OP_SHUTDOWN , 0x01); // Display test off

  delay(1000);

  Serial.println("Scan Limit test (3)");
  shift(OP_DECODEMODE , 0xff); //decode mode register - CodeB decode all digits
  shift(OP_DIGIT7, 0x08); //digit 7 (leftmost digit) data
  shift(OP_DIGIT6, 0x08);
  shift(OP_DIGIT5, 0x08);
  shift(OP_DIGIT4, 0x08);
  shift(OP_DIGIT3, 0x08);
  shift(OP_DIGIT2, 0x08);
  shift(OP_DIGIT1, 0x08);
  shift(OP_DIGIT0, 0x08); //digit 0 (rightmost digit) data  
  shift(OP_SCANLIMIT, 0x03);
  delay(1000);
  Serial.println("Scan Limit test (7)");
  shift(OP_SCANLIMIT, 0x07);
  delay(1000);

  Serial.println("Intensity test");
  for (int i = 0; i < 16; i++) {
    shift(OP_INTENSITY, 15-i);
    delay(100);
  }
  for (int i = 0; i < 16; i++) {
    shift(OP_INTENSITY, i);
    delay(100);
  }
  for (int i = 0; i < 16; i++) {
    shift(OP_INTENSITY, 15-i);
    delay(100);
  }
  for (int i = 0; i < 16; i++) {
    shift(OP_INTENSITY, i);
    delay(100);
  }
  for (int i = 0; i < 16; i++) {
    shift(OP_INTENSITY, 15-i);
    delay(100);
  }
  for (int i = 0; i < 16; i++) {
    shift(OP_INTENSITY, i);
    delay(100);
  }
  shift(OP_INTENSITY, 0xf);

  //Data transfer
  Serial.println("BCD with decimal point");
  shift(OP_DECODEMODE , 0xff); //decode mode register - CodeB decode all digits
  shift(OP_DIGIT7, 0x80); //digit 7 (leftmost digit) data
  shift(OP_DIGIT6, 0x81);
  shift(OP_DIGIT5, 0x82);
  shift(OP_DIGIT4, 0x83);
  shift(OP_DIGIT3, 0x84);
  shift(OP_DIGIT2, 0x85);
  shift(OP_DIGIT1, 0x86);
  shift(OP_DIGIT0, 0x87); //digit 0 (rightmost digit) data
  delay(1000);

  Serial.println("BCD without decimal point");
  shift(OP_DIGIT7, 0x08); //digit 7 (leftmost digit) data
  shift(OP_DIGIT6, 0x09);
  shift(OP_DIGIT5, 0x0a);
  shift(OP_DIGIT4, 0x0b);
  shift(OP_DIGIT3, 0x0c);
  shift(OP_DIGIT2, 0x0d);
  shift(OP_DIGIT1, 0x0e);
  shift(OP_DIGIT0, 0x0f); //digit 0 (rightmost digit) data
  delay(1000);

  shift(OP_DECODEMODE , 0x00); //decode mode register - CodeB decode all digits

  Serial.println("Cleared segments");
  shift(OP_DIGIT7, 0x00); //digit 7 (leftmost digit) data
  shift(OP_DIGIT6, 0x00);
  shift(OP_DIGIT5, 0x00);
  shift(OP_DIGIT4, 0x00);
  shift(OP_DIGIT3, 0x00);
  shift(OP_DIGIT2, 0x00);
  shift(OP_DIGIT1, 0x00);
  shift(OP_DIGIT0, 0x00); //digit 0 (rightmost digit) data

  delay(1000);

  Serial.println("Segment test");
  uint8_t x = 0x80;
  for (int i = 0; i < 8; i++) {
    shift(OP_DIGIT7, x); //digit 7 (leftmost digit) data
    shift(OP_DIGIT6, x);
    shift(OP_DIGIT5, x);
    shift(OP_DIGIT4, x);
    shift(OP_DIGIT3, x);
    shift(OP_DIGIT2, x);
    shift(OP_DIGIT1, x);
    shift(OP_DIGIT0, x); //digit 0 (rightmost digit) data
    x = x >> 1;
    delay(500);
  }
}
#endif