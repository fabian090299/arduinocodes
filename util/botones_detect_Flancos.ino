
#define BTN_1 0  //boton 1
#define BTN_2 1  //boton 2
#define BTN_3 2
#define BTN_4 3
#define BTN_5 4

uint8_t button[5]={   //arreglo para configurar los pines de cada boton
  12,
  14,
  27,
  26,
  25
};

uint8_t button_state[5];

int flancoSubida(int btn){
  int new_value = digitalRead(button[btn]);
  delay(50);
  bool result = 0;

  if(button_state[btn] != new_value && new_value == 1){
    result = 1;
    button_state[btn] = new_value;
  }
  else{
    result = 0;
    button_state[btn] = new_value;
  }
  return result;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(button[BTN_1], INPUT);
  pinMode(button[BTN_2], INPUT);
  pinMode(button[BTN_3], INPUT);
  pinMode(button[BTN_4], INPUT);
  pinMode(button[BTN_5], INPUT);
  
  button_state[0]=LOW;
  button_state[1]=LOW;
  button_state[2]=LOW;
  button_state[3]=LOW;
  button_state[4]=LOW;
}

void loop() {
  if(flancoSubida(BTN_1)){
    Serial.println("Boton1");
  }

  if(flancoSubida(BTN_2)){
    Serial.println("Boton2");
  }

  if(flancoSubida(BTN_3)){
    Serial.println("Boton3");
  }

  if(flancoSubida(BTN_4)){
    Serial.println("Boton4");
  }

  if(flancoSubida(BTN_5)){
    Serial.println("Boton5");
  }
  delay(100);
}
