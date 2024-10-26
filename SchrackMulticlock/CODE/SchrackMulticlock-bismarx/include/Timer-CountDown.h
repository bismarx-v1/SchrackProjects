/* CountDown Timer version 2 using Max7219

Created by Yvan / https://Brainy-Bits.com
This code is in the public domain...
You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/

#define Max7219DIN 7  // Pin 7 connected to DIN (DataIN)
#define Max7219CLK 6  // Pin 6 connected to CLK
#define Max7219CS 5   // Pin 5 connected to CS

#define Buzzer 8  // Pin 8 connected to Buzzer +positive pin
int BuzzTrigger=0;  // Variable to store buzzer trigger value

#include "LedControl.h"  // LedControl Library created by Eberhard Fahle at http://playground.arduino.cc/Main/LedControl

LedControl lc=LedControl(Max7219DIN,Max7219CLK,Max7219CS,1);   // Last number represent the number of Max7219 Modules connected 

long int countnumber=24001000 ; // Countdown timer start value HH:MM:SSSS

// Variables to store individual numbers
int  firstnum=0;
int  secondnum=0;
int  thirdnum=0;
int  fournum=0;
int  fivenum=0;
int  sixnum=0;
int  sevennum=0;
int  eightnum=0;

void setup() {
  lc.shutdown(0,false);  // Wake up the display
  lc.setIntensity(0,7); // Set Brightness 0-15
  lc.clearDisplay(0);  // Clear display
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);  //Buzzer Off at startup
  
  
}

void loop() {
  
  for (countnumber; countnumber != -1; countnumber --)
  {        
      String mystring = String(countnumber); // Transform Counter Int to String for manipulation
            
      // Convert number to a time value
      for (int z = 0; z < 6; z++){
             
        if ( mystring.substring(z) == "999999" ) {
          countnumber = (countnumber - 400000);
        }        

        if ( mystring.substring(z) == "9999" ) {
          countnumber = (countnumber - 4000);
        }
      }
  
//  Display number on Display depending on number of digits remaining
  if (countnumber > 9999999) {
    firstnum = ((countnumber/10000000)%10);
    secondnum = countnumber/1000000%10;
    thirdnum = countnumber/100000%10;
    fournum = countnumber/10000%10;
    fivenum = countnumber/1000%10;
    sixnum = countnumber/100%10;
    sevennum = countnumber/10%10;
    eightnum = countnumber%10;

    lc.setDigit(0,7,firstnum,false);
    lc.setDigit(0,6,secondnum,false);
    lc.setDigit(0,5,thirdnum,false);
    lc.setDigit(0,4,fournum,false);
    lc.setDigit(0,3,fivenum,false);
    lc.setDigit(0,2,sixnum,false);
    lc.setDigit(0,1,sevennum,false);
    lc.setDigit(0,0,eightnum,false);    
  }
    else {
      if (countnumber > 999999) {
//      firstnum = ((countnumber/10000000)%10);
        secondnum = countnumber/1000000%10;
        thirdnum = countnumber/100000%10;
        fournum = countnumber/10000%10;
        fivenum = countnumber/1000%10;
        sixnum = countnumber/100%10;
        sevennum = countnumber/10%10;
        eightnum = countnumber%10;

        lc.setChar(0,7,'-',false);
        lc.setDigit(0,6,secondnum,false);
        lc.setDigit(0,5,thirdnum,false);
        lc.setDigit(0,4,fournum,false);
        lc.setDigit(0,3,fivenum,false);
        lc.setDigit(0,2,sixnum,false);
        lc.setDigit(0,1,sevennum,false);
        lc.setDigit(0,0,eightnum,false);
      }   
        else {
          if (countnumber > 99999) {
//          firstnum = ((countnumber/10000000)%10);
//          secondnum = countnumber/1000000%10;
            thirdnum = countnumber/100000%10;
            fournum = countnumber/10000%10;
            fivenum = countnumber/1000%10;
            sixnum = countnumber/100%10;
            sevennum = countnumber/10%10;
            eightnum = countnumber%10;

            lc.setChar(0,7,'-',false);
            lc.setChar(0,6,'-',false);
            lc.setDigit(0,5,thirdnum,false);
            lc.setDigit(0,4,fournum,false);
            lc.setDigit(0,3,fivenum,false);
            lc.setDigit(0,2,sixnum,false);
            lc.setDigit(0,1,sevennum,false);
            lc.setDigit(0,0,eightnum,false);
          }
            else {
              if (countnumber > 9999) {
 //             firstnum = ((countnumber/10000000)%10);
 //             secondnum = countnumber/1000000%10;
 //             thirdnum = countnumber/100000%10;
                fournum = countnumber/10000%10;
                fivenum = countnumber/1000%10;
                sixnum = countnumber/100%10;
                sevennum = countnumber/10%10;
                eightnum = countnumber%10;

                lc.setChar(0,7,'-',false);
                lc.setChar(0,6,'-',false);
                lc.setChar(0,5,'-',false);
                lc.setDigit(0,4,fournum,false);
                lc.setDigit(0,3,fivenum,false);
                lc.setDigit(0,2,sixnum,false);
                lc.setDigit(0,1,sevennum,false);
                lc.setDigit(0,0,eightnum,false);
              } 
              else {
                if (countnumber > 999) {
  //              firstnum = ((countnumber/10000000)%10);
  //              secondnum = countnumber/1000000%10;
  //              thirdnum = countnumber/100000%10;
  //              fournum = countnumber/10000%10;
                  fivenum = countnumber/1000%10;
                  sixnum = countnumber/100%10;
                  sevennum = countnumber/10%10;
                  eightnum = countnumber%10;
  
                  lc.setChar(0,7,'-',false);
                  lc.setChar(0,6,'-',false);
                  lc.setChar(0,5,'-',false);
                  lc.setChar(0,4,'-',false);
                  lc.setDigit(0,3,fivenum,false);
                  lc.setDigit(0,2,sixnum,false);
                  lc.setDigit(0,1,sevennum,false);
                  lc.setDigit(0,0,eightnum,false);
                }
                else {
  //              firstnum = ((countnumber/10000000)%10);
  //              secondnum = countnumber/1000000%10;
  //              thirdnum = countnumber/100000%10;
  //              fournum = countnumber/10000%10;
  //              fivenum = countnumber/1000%10;
                  sixnum = countnumber/100%10;
                  sevennum = countnumber/10%10;
                  eightnum = countnumber%10;
  
                  lc.setChar(0,7,'-',false);
                  lc.setChar(0,6,'-',false);
                  lc.setChar(0,5,'-',false);
                  lc.setChar(0,4,'-',false);
                  lc.setChar(0,3,'-',false);
                  lc.setDigit(0,2,sixnum,false);
                  lc.setDigit(0,1,sevennum,false);
                  lc.setDigit(0,0,eightnum,false);
             }
               
            }
          }
        } 
      }
      
// If one second as gone by sound buzzer
      if (BuzzTrigger == 99){
          digitalWrite (Buzzer, HIGH) ;// Buzzer On
          delay (9) ;// Delay 2ms
          digitalWrite (Buzzer, LOW) ;// Buzzer Off 
      BuzzTrigger = 0; //  Trigger for countdown sound
      }
      else {
        delay (9);
        BuzzTrigger = BuzzTrigger + 1;
      }

// If countdown at zero sound alarm and flash display      
      if (countnumber == 0) {
        for (int y = 0; y < 8; y++){
          digitalWrite (Buzzer, HIGH) ;// Buzzer On
          for (int x = 0; x < 8; x++) 
          {
            lc.setDigit(0,x,0,false);
          } 
          delay (100) ;// Delay 1ms          
          digitalWrite (Buzzer, LOW) ;// Buzzer Off
          for (int x = 0; x < 8; x++) 
          {
            lc.setChar(0,x,'-',false);
          } 
          delay (300) ;// delay 1ms
      }
      }    
    }
}
