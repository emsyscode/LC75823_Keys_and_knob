/**********************************************************************/
/*This code is not clean and OFFCOURSE will be possible do it better! */ 
/*this is only a group of functions to be used as independent code,   */ 
/*and by this way will be possible to any person can do the changes   */ 
/*of code and see what happen.                                        */
/*The code don't depend of any external library or extenal functions  */
/*complicated.                                                        */
/*I'm let the values to sent as binnary, this allow swap bit by bit   */
/* to is possible test segment by segment without convert to HEX      */
/**********************************************************************/
/* This code show how is possible work with the driver LC75823 Sanyo  */
/*Begining of Auto generated code by Atmel studio                     */
/**********************************************************************/

//set your clock speed
#define F_CPU 16000000UL
//these are the include files. They are outside the project folder
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <String.h>

#define VFD_in 7  // This is the pin number 7 on Arduino UNO
#define VFD_clk 8 // This is the pin number 8 on Arduino UNO
#define VFD_ce 9 // This is the pin number 9 on Arduino UNO
// don't define the other pins, but is A0 to input analog signal, pins D2 & D3 is trigger input, 

//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.

bool right = false;
bool left = false;
bool led1= 10;
bool led2= 11;
bool byStep =6;
bool stopCycle = 5;
bool fwd = false;

int seg=-1; // use as global to allow the change by knob button and press buttons.
int val = 0;
bool cyc=false; // Force exit of cycle while
int b = -1; // Controle of the bit in the byte

char strADC[5]; //initializing a character of size 5 for showing the ADC result

//*************************************************//
void send_char(unsigned char a){
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  // the validation of data happen when clk go from LOW to HIGH.
  digitalWrite(VFD_ce, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  digitalWrite(VFD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
  for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(VFD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    //
    digitalWrite(VFD_ce, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  }
}
/*********************************************************************/
// I h've created 3 functions to send bit's, one with strobe, other without strobe and one with first byte with strobe followed by remaing bits.
void send_char_without(unsigned char a)
{
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(VFD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
  }
}
/*********************************************************************/
void send_char_8bit_stb(unsigned char a)
{
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
 int i = -1;
  data=a;
  for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
   i++;
   digitalWrite(VFD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk,HIGH);// 
    delayMicroseconds(1);
    if (i==7){
    //Serial.println(i);
    digitalWrite(VFD_ce, HIGH);
    delayMicroseconds(1);
    }  
  }
}
/***************************** Actions to do selected by input values of ADC **************************************/
void actionButtons(void){
      if((val > 21) & (val<30)){
           seg--;
           b=seg%8;
             if(seg < 0){
             seg=0;
             }
           cyc=false;
           }
      if((val > 50) & (val<65)){
           seg++;
           b=seg%8;    
           cyc=false;
           }
      if((val > 11) & (val < 19)){  //!!! This value must be before of the next, other way the next go inside this if also!
           seg=156;
           b=seg%8;
           }
      if((val > 1) & (val < 9)){
           seg=0; // segment reinitialize
           b=0; // bit position on byte, reinitialize.
           }
             if(val > 990){
             //Serial.println(seg);
             }
             if ((val > 120) & (val < 130)){
                    testADC();
             }
}
/******************************** test the ADC *****************************************/
void testADC(void){
  cyc=true;
  // To go out of cycle while, move the knob, because it will change the status of boolean "cyc"!
             while(cyc){
                String valADC = String(analogRead(A0)); //intailizing a string and storing ADC value in it
                valADC.toCharArray(strADC, 5); // convert the reading to a char array
                //Serial.println(valADC.toInt());
                val=(valADC.toInt());
                Serial.println(val);
             }                           
}
/******************************** to test segments *************************************/
void OneByOne(){
 // This function sent bit by bit to test the segment of LCD
 // on the same byte, until the last bit(156).
 unsigned char data = 0x00; //value to transmit, binary 10101010
 unsigned int mask = 0xFEFF; //our bitmask
 int s = 0; // only to change the order of bit 7~0 to 0~7. (the reversion of "i" not work!!! to chek)
 int block=-1;
 int tmp=-1;
 int var=0;

 seg=-1;
 //
 Serial.println(" Start Cycle to find Segments, until 156");
 Serial.println(" Press Knob Button to advance 1 by one!");
        //for (int seg=0; seg < 160; seg++){
          while(seg < 160){
            cyc=true;
                while(cyc){
                String valADC = String(analogRead(A0)); //intailizing a string and storing ADC value in it
                valADC.toCharArray(strADC, 5); // convert the reading to a char array
                //Serial.println(valADC.toInt());
                val=(valADC.toInt());
                //Serial.println(val);
                actionButtons();
                }  
                         if(seg>160){
                         break;
                         } 
                         if(seg<-1){
                          seg=-1;  
                         }
                allON_LC75823();
               // Serial.println(seg);
          //tmp++;
          //var=(tmp/8); // Determine and fix the Byte Number to apply on the right shift.
          var=(seg/8); // Determine and fix the Byte Number to apply on the right shift.
          digitalWrite(VFD_ce, LOW); //
          delayMicroseconds(1);
          // Position of bit(..76543210)
          send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
          delayMicroseconds(1);
                for(block=0; block < 20; block++){ 
                  if (block==19){
                    send_char_without(0B00011111);  // This is the byte to close the communication of 156 Bits(last 4 of 160 is used to control)
                  }
                  else{
                              if(var==block){ 
                                      if (b<7){
                                      mask = ((0b1111111101111111 >> b) & 0x00FF);
                                      send_char_without(mask);
                                      //b++; // Controlled by the buttons to increment or decrement also i.
                                      }
                                      else{
                                      mask = ((0b1111111101111111 >> b) & 0x00FF);
                                      send_char_without(mask);
                                      //b=0; // Reinitialize the variable "i"
                                      }
                              }
                              else{
                              send_char_without(0B11111111);
                              }                       
                    }
                }                                   
         delayMicroseconds(1);
         digitalWrite(VFD_ce, LOW); // 
         delayMicroseconds(1);
         //
         delay(250);
         // This switch is only to avoid the revert of variable "i"... check it!
             switch (b){  
              case 0: s =7; break;
              case 1: s =6; break;
              case 2: s =5; break;
              case 3: s =4; break;
              case 4: s =3; break;
              case 5: s =2; break;
              case 6: s =1; break;
              case 7: s =0; break;
             }
         Serial.print("Byte & Bit: ");Serial.print(var); Serial.print(" ");Serial.print(s); Serial.print(" Seg: "); Serial.println(seg); // Only to debug     
        }
}
/*********************************** write to LCD  ***************************************/
// Write "HI FOLKS and do the action with the bar's
// This is sample how send the block of 160 bits to write on LCD, The correct way is create a function and send
// to it the array of char to write. The code will stay short and easy, done this way is only to be ease understain!
void wrHiFolks0(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B00000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00000000);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
void wrHiFolks1(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B10000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00000000);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
void wrHiFolks2(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B10000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00000001);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
void wrHiFolks3(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B10000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00000011);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
void wrHiFolks4(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B10000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00000111);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
void wrHiFolks5(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B10000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00001111);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
void wrHiFolks6(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B10000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00011111);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
void wrHiFolks7(void){
  unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00011100);  send_char_without(0B01100100); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B00000000);  send_char_without(0B10000000); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B10100011);  send_char_without(0B11000000); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B00110000);  send_char_without(0B01100110); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B00010000);  send_char_without(0B01110000); //  81:88    -89:96//    Byte A & B
          send_char_without(0B01100000);  send_char_without(0B00101000); //  97:104   -105:112//  Byte C & D
          send_char_without(0B01001110);  send_char_without(0B00000000); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B00000000);  send_char_without(0B10000000); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B00111111);  send_char_without(0B00000000); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
/************************************ using array sample **************************************/
void expArray(void){
// This is a sample how write the compact form of the array binary to HEX. The lines is to short
// Don't forget the last 4 bits belongs to the command!!! On this sample they already is "0". See Datasheet of driver, bits: DR, SC, BU, *.
unsigned char ar7[20]={0x1C,0x64,0x40,0x00,0x00,0x80,0xA3,0xC0,0x30,0x66,0x10,0x70,0x60,0x28,0x4E,0x00,0x00,0x80,0x3F,0x00};
digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
         for(int y=0;y<20;y++){
         send_char_without(ar7[y]);
         }
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);               
}
/***********************************  switch On all seg   ***************************************/
void allON_LC75823(){
unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      // Position of bit(..76543210)
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B11111111);  send_char_without(0B11111111); //   1:8     -9:16//     byte 0 & 1
          send_char_without(0B11111111);  send_char_without(0B11111111); //  17:24    -25:32//    Byte 2 & 3
          send_char_without(0B11111111);  send_char_without(0B11111111); //  33:40    -41:48//    Byte 4 & 5
          send_char_without(0B11111111);  send_char_without(0B11111111); //  49:56    -57:64//    Byte 6 & 7
          send_char_without(0B11111111);  send_char_without(0B11111111); //  65:72    -73:80//    Byte 8 & 9
          send_char_without(0B11111111);  send_char_without(0B11111111); //  81:88    -89:96//    Byte A & B
          send_char_without(0B11111111);  send_char_without(0B11111111); //  97:104   -105:112//  Byte C & D
          send_char_without(0B11111111);  send_char_without(0B11111111); // 113:120   -121:128//  Byte E & F 
          send_char_without(0B11111111);  send_char_without(0B11111111); // 120:136   -137:144//  Byte G & H 
          send_char_without(0B11111111);  send_char_without(0B00011111); // 145:152   -153:160//  Byte I & J
          
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
/*********************************  switch off all seg *****************************************/
void allOFF_LC75823(){
unsigned char group=0;
//send total of 64 bits, the 4 last bits belongs to set of DR, SC, BU, * D'ont care;
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 600 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8     -9:16// 
          send_char_without(0B00000000);  send_char_without(0B00000000); //  17:24    -25:32// 
          send_char_without(0B00000000);  send_char_without(0B00000000); //  33:40    -41:48//
          send_char_without(0B00000000);  send_char_without(0B00000000); //  49:56    -57:64//
          send_char_without(0B00000000);  send_char_without(0B00000000); //  65:72    -73:80//
          send_char_without(0B00000000);  send_char_without(0B00000000); //  81:88    -89:96//
          send_char_without(0B00000000);  send_char_without(0B00000000); //  97:104   -105:112//
          send_char_without(0B00000000);  send_char_without(0B00000000); // 113:120   -121:128// 
          send_char_without(0B00000000);  send_char_without(0B00000000); // 120:136   -137:144// 
          send_char_without(0B00000000);  send_char_without(0B00010000); // 145:152   -153:160//
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
/******************************* write numbers to LCD ************************************/
void wr01234567(void){
//
digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000001); //(0x41) firts 8 bits is address, every fixed as (0B01000001), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00001100);  send_char_without(0B01100011); //   1:8     -9:16//   // 0 // 1
          send_char_without(0B01000000);  send_char_without(0B00000000); //  17:24    -25:32//  // 2 // 3
          send_char_without(0B11000110);  send_char_without(0B00010001); //  33:40    -41:48//  // 4 // 5
          send_char_without(0B11100010);  send_char_without(0B01001100); //  49:56    -57:64//  // 6 // 7
          send_char_without(0B01000001);  send_char_without(0B10100110); //  65:72    -73:80//  // 8 // 9
          send_char_without(0B00111000);  send_char_without(0B01110001); //  81:88    -89:96//  // A // B
          send_char_without(0B10011100);  send_char_without(0B00000000); //  97:104   -105:112//// C // D
          send_char_without(0B11000010);  send_char_without(0B00000000); // 113:120   -121:128//// E // F 
          send_char_without(0B00000000);  send_char_without(0B00000000); // 120:136   -137:144//// G // H 
          send_char_without(0B00000000);  send_char_without(0B00000000); // 145:152   -153:160//// I // J
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
}
/********************************** Functions to ISR *********************************/
void knob1(void){
  if((digitalRead(2)) & (not(digitalRead(3)))){
   left=true;right=false;
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    seg--;
    b=seg%8;
    cyc=false;
  }
}
void knob2(void){
  if((not(digitalRead(2))) & (digitalRead(3))){
   left=false;right=true;
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    seg++;
    b=seg%8;
    cyc=false;
  }
}
/************************************* Setup ****************************************/
void setup() {
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_in, OUTPUT);
  pinMode(VFD_ce, OUTPUT);
 
  pinMode(13, OUTPUT);
  Serial.begin(9600); // only to debug

  attachInterrupt(digitalPinToInterrupt(2),buttonPressed1,RISING);  // Configuration of Interrupt to the pin 2
  attachInterrupt(digitalPinToInterrupt(3),buttonPressed2,RISING);  // Configuration of Interrupt to the pin 3
  sei();
}
/************************************** Loop ****************************************/
void loop() {
testADC();


allON_LC75823(); // All on
delay(500);
allOFF_LC75823(); // All off
delay(500);
allON_LC75823(); // All on
delay(500);
allOFF_LC75823(); // All off
delay(500);
allON_LC75823(); // All on
delay(500);
allOFF_LC75823(); // All off
delay(500);
//
      for(int i=0; i< 3;i++){
          wrHiFolks0();
          delay(500);
          wrHiFolks1();
          delay(100);
          wrHiFolks2();
          delay(100);
          wrHiFolks3();
          delay(100);
          wrHiFolks4();
          delay(100);
          wrHiFolks5();
          delay(100);
          wrHiFolks6();
          delay(100);
          expArray(); //wrHiFolks7();
          delay(100);
          //Revert seq.
          expArray(); //wrHiFolks7();
          delay(100);
          wrHiFolks6();
          delay(100);
          wrHiFolks5();
          delay(100);
          wrHiFolks4();
          delay(100);
          wrHiFolks3();
          delay(100);
          wrHiFolks2();
          delay(100);
          wrHiFolks1();
          delay(100);
      }
    for(int i=0; i< 2;i++){
        wrHiFolks0();
        delay(500);
        wrHiFolks7();
        delay(100);
        wrHiFolks6();
        delay(100);
        wrHiFolks5();
        delay(100);
        wrHiFolks4();
        delay(100);
        wrHiFolks3();
        delay(100);
        wrHiFolks2();
        delay(100);
        wrHiFolks1();
        delay(100);
    }
wr01234567();  // write the numbers to LCD
delay(1000);
//
OneByOne(); // Do the test segment by segment
}
/*****************************************  ISR *************************************/
void buttonPressed1(){
  knob1();  // Call this function                
}

void buttonPressed2(){ 
  knob2(); // Call this function
}
