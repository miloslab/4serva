#include <Arduino.h>

#include <EEPROM.h>
#include <Bounce2.h>  // !!!!!!!!!!! nepoužívat delay() uvnitř loop() !!!!!!!!!!!!!!
#include <Wire.h>
#include <Servo.h>
/*
 * 
 */
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

#define eeAddress 10

#define tl01Pin 2
#define tl02Pin 4
#define tl03Pin 6
#define tl04Pin 8
#define tl05Pin 10
#define servo01Pin 3
#define servo02Pin 5
#define servo03Pin 7
#define servo04Pin 9
#define servo05Pin 11
//#define stopPin 3
//#define ledPin 13

byte servo01Min, servo01Max, servo02Min, servo02Max, servo03Min, servo03Max, servo04Min, servo04Max, servo05Min, servo05Max;
int servo04T, servo05T; 
long cas4, cas5;
boolean semafor4=false, semafor5;

struct MyObject{
  byte field11;
  byte field12;
  byte field21;
  byte field22;
  byte field31;
  byte field32;
  byte field41;
  byte field42;
  int field43;
  byte field51;
  byte field52;
  int field53;
};

MyObject eepromVar; //Variable to store custom object read from EEPROM.

Bounce tl01 = Bounce();
Bounce tl02 = Bounce();
Bounce tl03 = Bounce();
Bounce tl04 = Bounce();
Bounce tl05 = Bounce();
//Bounce tlStop = Bounce();

Servo servo01;  // create servo object to control a servo
Servo servo02;  // create servo object to control a servo
Servo servo03;  // create servo object to control a servo
Servo servo04;  // create servo object to control a servo
Servo servo05;  

void cteni(){
  EEPROM.get( eeAddress, eepromVar );
  servo01Min=eepromVar.field11;
  servo01Max=eepromVar.field12;
  servo02Min=eepromVar.field21;
  servo02Max=eepromVar.field22;
  servo03Min=eepromVar.field31;
  servo03Max=eepromVar.field32;
  servo04Min=eepromVar.field41;
  servo04Max=eepromVar.field42;
  servo04T=eepromVar.field43;
  servo05Min=eepromVar.field51;
  servo05Max=eepromVar.field52;  
  servo05T=eepromVar.field53;
}

void zapis()  {
    //Data to store.
  MyObject eepromVar = {
    0,
    180,
    0,
    180,
    0,
    180,
    0,
    180,
    500,
    0,
    180,
    500
  };
  eepromVar.field11=servo01Min;
  eepromVar.field12=servo01Max;
  eepromVar.field21=servo02Min;
  eepromVar.field22=servo02Max;
  eepromVar.field31=servo03Min;
  eepromVar.field32=servo03Max;
  eepromVar.field41=servo04Min;
  eepromVar.field42=servo04Max;
  eepromVar.field43=servo04T;
  eepromVar.field51=servo05Min;
  eepromVar.field52=servo05Max;
  eepromVar.field53=servo05T;
 
  EEPROM.put( eeAddress, eepromVar );
  Serial.print( "Nastavení serva jsou zapsána!  " );
  Serial.println(sizeof(eepromVar));
}

void help() {
 Serial.println(F("Nastaveni serva: ( end line CRLF or LF)"));
 Serial.println(F("A0, 180, nastaví servo A (01) na rozsah 0-180°"));
 Serial.println(F("D0, 180, 500 nastaví servo01 na rozsah -180/2 prodlení 0,5s a 180°"));
 Serial.println(F("E0, 180, 500 nastaví servo01 na rozsah 0-180 prodlení 0,5s a návrat"));
 Serial.println(F("V vypíše aktuální hodnoty serva")); 
 Serial.println(F("Z zapíše aktuální hodnoty do EEPROM")); 
 Serial.println(F("P vypíše Pomoc"));
}

void vypis()  {
  Serial.print("Servo A "); Serial.print(servo01Min); Serial.print(" "); Serial.println(servo01Max);  
  Serial.print("Servo B "); Serial.print(servo02Min); Serial.print(" "); Serial.println(servo02Max);
  Serial.print("Servo C "); Serial.print(servo03Min); Serial.print(" "); Serial.println(servo03Max);
  Serial.print("Servo D "); Serial.print(servo04Min); Serial.print(" "); Serial.print(servo04Max); Serial.print(" "); Serial.println(servo04T);
  Serial.print("Servo E "); Serial.print(servo05Min); Serial.print(" "); Serial.print(servo05Max); Serial.print(" "); Serial.println(servo05T);
  Serial.println();
}

void process_line() {
 char cmd = Serial.read();
 if(cmd>'Z') cmd-=32;
 switch(cmd) {
  case 'A': servo01Min=Serial.parseInt();
            Serial.println(servo01Min);
            servo01Max=Serial.parseInt(); 
            Serial.println(servo01Max); 
            break;
  case 'B': servo02Min=Serial.parseInt();
            Serial.println(servo02Min);
            servo02Max=Serial.parseInt(); 
            Serial.println(servo02Max); 
            break;
  case 'C': servo03Min=Serial.parseInt();
            Serial.println(servo03Min);
            servo03Max=Serial.parseInt(); 
            Serial.println(servo03Max); 
            break;
  case 'D': servo04Min=Serial.parseInt();
            Serial.println(servo04Min);
            servo04Max=Serial.parseInt(); 
            Serial.println(servo04Max);
            servo04T=Serial.parseInt(); 
            Serial.println(servo04T); 
            break;
  case 'E': servo05Min=Serial.parseInt();
            Serial.println(servo05Min);
            servo05Max=Serial.parseInt(); 
            Serial.println(servo05Max); 
            servo05T=Serial.parseInt(); 
            Serial.println(servo05T); 
            break;
  case 'Z': zapis();  break;
  case 'P': help(); break;
  case 'H': help(); break;
  case 'V': vypis(); 
  }
// while(Serial.read()!=10); // dump extra characters till LF is seen (you can use CRLF or just LF)
}

void setup()
{
  Serial.begin (115200);
  cteni();
  help();
  vypis();
  
  //nastavení Bounce:
  tl01.attach(tl01Pin);
  tl01.interval(50);
  tl02.attach(tl02Pin);
  tl02.interval(50);
  tl03.attach(tl03Pin);
  tl03.interval(50);
  tl04.attach(tl04Pin);
  tl04.interval(50);
  tl05.attach(tl05Pin);
  tl05.interval(50);
  
  servo01.attach(servo01Pin);  // servo01.attach(13, 545, 2399);
  servo01.write(servo01Max);  //první servo na ventil jede obráceně
  servo02.attach(servo02Pin);  
  servo02.write(servo02Min);  
  servo03.attach(servo03Pin);  
  servo03.write(servo03Min);  
  servo04.attach(servo04Pin);  
  servo04.write((servo04Max+servo04Min)/2);
  servo05.attach(servo05Pin);  
  servo05.write(servo05Min);  

  pinMode(tl01Pin, INPUT_PULLUP);
  pinMode(tl02Pin, INPUT_PULLUP);
  pinMode(tl03Pin, INPUT_PULLUP);
  pinMode(tl04Pin, INPUT_PULLUP);
  pinMode(tl05Pin, INPUT_PULLUP);
}

void loop()
{
  tl01.update();
  tl02.update();
  tl03.update();
  tl04.update();
  tl05.update();

  if (tl01.fell())  {
    servo01.write(servo01Min);
  //  Serial.println(servo01Max);
  }
  if (tl01.rose())  { 
    servo01.write(servo01Max);
  //  Serial.println(servo01Min); 
   }
  if (tl02.fell())  {
    servo02.write(servo02Max);
  }
  if (tl02.rose())  { 
    servo02.write(servo02Min); 
   }
  if (tl03.fell())  {
    servo03.write(servo03Max);
  }
  if (tl03.rose())  { 
    servo03.write(servo03Min); 
   }
  if (tl04.fell())  {
    servo04.write(servo04Max);
    cas4=millis();
    semafor4= true;
  }
  if ((millis() > (cas4+servo04T) ) and semafor4) { 
    servo04.write(servo04Min);
    semafor4=false;
  } 
  if (tl04.rose()and!semafor4)  { 
    servo04.write((servo04Max+servo04Min)/2);
     }
  if (tl05.fell())  {
    servo05.write(servo05Max);
    cas5 = millis();
  
   }
  if (millis() > (cas5+servo05T) )  { 
    servo05.write(servo05Min); 
   }

// ***************************************/
//    if(Serial.available()) process_line(); // it may induce a glitch to move motion, so use it sparingly 

/*  tlStop.update();
    if (tlStop.rose()) {
      servo01.detach(); 
      //Serial.println("Rose");
      }
      
    if (tlStop.fell()) {
      servo01.attach(servo01Pin, 500, 2300);  // servo01.attach(13, 545, 2399);
      //servo01.write(servo01Max);  
    }
    
    if (((cas4 + servo04T) < millis()) && (semafor4 == true)) {
      servo04.write(servo04Min);
      Serial.println(servo04Min); 
      semafor4 = false;
      //else {semafor4 = true
    }*/
 }
