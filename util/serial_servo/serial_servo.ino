#include <Servo.h>

Servo servo;

String inString = "";
const int pinServo=9;

void setup() {
  Serial.begin(115200);
  servo.attach(pinServo);
}

void loop() {
  if(Serial.available()>0){
    int inChar=Serial.read();

    if(inChar=='\n'){
      int valor =inString.toInt();

      if(valor<175){
        servo.write(valor);
        Serial.println(valor);
      }

      if(valor>5){
        servo.write(valor);
        Serial.println(valor);
      }
      
      inString = ""; 
    }
    
    else{
      inString+=(char)inChar;
    }
  }
} 
