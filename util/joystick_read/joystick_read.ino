// ==================================================================================================================================
//===================================== ALGORITMO LECTURA DE JOYSTICK ===============================================================
// ==================================================================================================================================

uint8_t axisY = 0;
uint8_t axisX = 1;
uint8_t sweepY = 0;
uint8_t sweepX = 0;

bool eventY(uint8_t readY){
  static uint16_t ant = 0;  // Declarar 'ant' como una variable estática para que no se setee a 0 en cada llamada a funcion event

  uint16_t act = analogRead(readY);
  delay(50);

  if ((ant != act) && (act >= 1000)) {
    ant = act;
    sweepY = 1;
    return true;
  } 
  else if((ant != act) && (act <= 100)){
    ant = act;
    sweepY = 2;
    return true;
  }
  else {
    ant = act;
    return false;
  }
}

bool eventX(uint8_t readX){
  static uint16_t ant = 0;  // Declarar 'ant' como una variable estática para que no se setee a 0 en cada llamada a funcion event

  uint16_t act = analogRead(readX);
  delay(50);

  if ((ant != act) && (act >= 1000)) {
    ant = act;
    sweepX = 4;
    return true;
  } 
  else if((ant != act) && (act <= 100)){
    ant = act;
    sweepX = 3;
    return true;
  }
  else {
    ant = act;
    return false;
  }
}
// ================================================================================================================================
// ==================================================================================================================================
// ==================================================================================================================================

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (eventY(axisY)) {
    Serial.print("Evento: ");
    Serial.println(sweepY);
  }
  if(eventX(axisX)){
    Serial.print("Evento: ");
    Serial.println(sweepX);
  }
}