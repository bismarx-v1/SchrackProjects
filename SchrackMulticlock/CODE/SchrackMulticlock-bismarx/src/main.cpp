#include <SPI.h>
#include <Arduino.h>
const byte MAX7219_REG_NOOP = 0x0;
// codes 1 to 8 are digit positions 1 to 8
const byte MAX7219_REG_DECODEMODE = 0x9;
const byte MAX7219_REG_INTENSITY = 0xA;
const byte MAX7219_REG_SCANLIMIT = 0xB;
const byte MAX7219_REG_SHUTDOWN = 0xC;
const byte MAX7219_REG_DISPLAYTEST = 0xF;

void sendByte(const byte reg, const byte data)
{
  digitalWrite(SS, LOW);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(SS, HIGH);
} // end of sendByte

void setup()
{
  SPI.begin();
  sendByte(MAX7219_REG_SCANLIMIT, 7);     // show 8 digits
  sendByte(MAX7219_REG_DECODEMODE, 0xFF); // use digits (not bit patterns)
  sendByte(MAX7219_REG_DISPLAYTEST, 0);   // no display test
  sendByte(MAX7219_REG_INTENSITY, 7);     // character intensity: range: 0 to 15
  sendByte(MAX7219_REG_SHUTDOWN, 1);      // not in shutdown mode (ie. start it up)
} // end of setup

void number(const unsigned long num)
{

  char buf[9];
  sprintf(buf, "%08ld", min(max(num, 0), 99999999));

  // send all 8 digits
  for (int digit = 0; digit < 8; digit++)
  {
    byte c = buf[digit];
    if (c == ' ')
      c = 0xF; // code for a blank
    else
      c -= '0';
    sendByte(8 - digit, c);
  }
} // end of number

unsigned long i;

void loop()
{
  number(i++);
} // end of loop