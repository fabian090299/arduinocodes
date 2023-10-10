#define sensor 9

int sensor_state;
uint8_t counter = 0;
bool state1 = false;

int event(int sen){
  int new_value = digitalRead(sen);
  delay(100);
  bool result = 0;

  if(sensor_state != new_value && (new_value == 0 || new_value == 1)){
    result = 1;
    sensor_state = new_value;
    
    if(state1 == false) state1 = true;
    else if(state1 == true) state1 = false;
  }
  else{
    result = 0;
    sensor_state = new_value;
  }
  return result;
}

void setup(){
  pinMode(sensor, INPUT);
  sensor_state = HIGH;
  Serial.begin(9600);
}

void loop(){
  if(event(sensor)){
    if(state1 == true) Serial.println("Activado 1");
    else Serial.println("Desactivado 1");
  }
}
