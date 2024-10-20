#include <Arduino.h>
#define bt_start  A0
#define bt_stop   A1
#define bt_reset  A2

#define buzzer 13

#define MAX7219_Clock 2
#define MAX7219_Chip_Select  3
#define MAX7219_Data_IN 4

int hh=0, mm=0, ss=0;
word MilliSecond = 0; 
bool timerStart = false;

void setup() { // put your setup code here, to run once 

pinMode(bt_start, INPUT_PULLUP);
pinMode(bt_stop,  INPUT_PULLUP);
pinMode(bt_reset, INPUT_PULLUP);

pinMode(buzzer, OUTPUT);

pinMode(MAX7219_Data_IN, OUTPUT);
pinMode(MAX7219_Chip_Select, OUTPUT);
pinMode(MAX7219_Clock, OUTPUT);
digitalWrite(MAX7219_Clock, HIGH);

delay(200);

//Setup of MAX7219 chip
shift(0x0f, 0x00); //display test register - test mode off
shift(0x0c, 0x01); //shutdown register - normal operation
shift(0x0b, 0x07); //scan limit register - display digits 0 thru 7
shift(0x0a, 0x0f); //intensity register - max brightness
shift(0x09, 0xff); //decode mode register - CodeB decode all digits

 noInterrupts();         // disable all interrupts
 TCCR1A = 0;             // set entire TCCR1A register to 0  //set timer1 interrupt at 1kHz  // 1 ms
 TCCR1B = 0;             // same for TCCR1B
 TCNT1  = 0;             // set timer count for 1khz increments
 OCR1A = 1999;           // = (16*10^6) / (1000*8) - 1
 //had to use 16 bit timer1 for this bc 1999>255, but could switch to timers 0 or 2 with larger prescaler
 // turn on CTC mode
 TCCR1B |= (1 << WGM12); // Set CS11 bit for 8 prescaler
 TCCR1B |= (1 << CS11);  // enable timer compare interrupt
 TIMSK1 |= (1 << OCIE1A);
 interrupts();           // enable

}

void loop() {

if(digitalRead (bt_start) == 0){ 
digitalWrite(buzzer, HIGH);   
timerStart = true;
delay(100);
}

if(digitalRead (bt_stop) == 0){ 
digitalWrite(buzzer, HIGH);   
timerStart = false;
delay(100);
}

if(digitalRead (bt_reset) == 0){ 
digitalWrite(buzzer, HIGH);   
MilliSecond = 0;
ss=0, mm=0, hh=0;
delay(100);
}
  
shift(0x08,(hh/10)%10);  //digit 7 (rightmost digit) data
shift(0x07, hh%10);
shift(0x06, 10);
shift(0x05,(mm/10)%10);
shift(0x04, mm%10);
shift(0x03, 10);
shift(0x02,(ss/10)%10);
shift(0x01, ss%10);      //digit 0 (leftmost digit) data

delay(100);
digitalWrite(buzzer, LOW);
}

void shift(byte send_to_address, byte send_this_data){
  digitalWrite(MAX7219_Chip_Select, LOW);
  shiftOut(MAX7219_Data_IN, MAX7219_Clock, MSBFIRST, send_to_address);
  shiftOut(MAX7219_Data_IN, MAX7219_Clock, MSBFIRST, send_this_data);
  digitalWrite(MAX7219_Chip_Select, HIGH);
}

ISR(TIMER1_COMPA_vect){   
if(timerStart == true){MilliSecond++;
 if(MilliSecond >= 1000){MilliSecond = 0;
   ss=ss+1;
if(ss>59){ss=0; mm=mm+1;}
if(mm>59){mm=0; hh=hh+1;}
  }  
 }
}
