#define clk 2
#define dt 3

bool stateDT = 0;
bool stateCLK = 0;

void setup() {
pinMode(clk, INPUT);
pinMode(dt, INPUT);
pinMode(13, OUTPUT);
digitalWrite(13, LOW);

Serial.begin(9600);

attachInterrupt(digitalPinToInterrupt(clk), event, FALLING);
}

void loop() {

}

void event(){
  stateDT = digitalRead(dt);
  if(stateDT == HIGH) Serial.println("True");
  if(stateDT == LOW) Serial.println("False");
  delay(20);
}