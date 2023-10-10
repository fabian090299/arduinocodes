
//________________________________________________________________________________________________________________________________---
//#define clk 2
//#define led 9
//
//int counter = 0;
//int counter_ant = 0;
//
//void setup(){
//  pinMode(clk, INPUT);
//  pinMode(led, OUTPUT);
//  attachInterrupt(digitalPinToInterrupt(clk), function, RISING);
//  Serial.begin(9600);
//}
//
//void function(){
//  volatile unsigned long act = millis();
//  static unsigned long ant = 0;
//
//  if(act - ant > 10){
//    counter++;  
//    ant = act;
//  }
//}
//
//void loop(){
//  if(counter != counter_ant){
//    Serial.println(counter);
//    counter_ant = counter;
//  }
//}
