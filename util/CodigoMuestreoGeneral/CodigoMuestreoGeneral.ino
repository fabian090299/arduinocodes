volatile unsigned long tiempoAct = 0;
volatile unsigned long tiempoAnt = 0;
volatile unsigned long delta = 0;
int T = 3000;          //periodo de muestreo en mS

void setup() {
  Serial.begin(9600);
}

void loop() {  
  tiempoAct = millis();   
  delta = (double) tiempoAct - tiempoAnt;

  if(delta >= T){
    Serial.println((float)millis()/1000);
    tiempoAnt = tiempoAct;
  }  
}