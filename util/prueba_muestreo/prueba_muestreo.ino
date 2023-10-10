volatile unsigned long muestraAct = 0;
volatile unsigned long muestraAnt = 0;
volatile unsigned long delta = 0;
String stringIn = "";
bool finString = false;
bool stp = false;
int counter = 0;
int lapse = 4096;                         //lapso de tiempo de toma de muestras en mS  (2**n)
int T = 1;                                //periodo de muestreo en mS
int pinRead = A0;
int valor = 0;
int valor_map = 0;

void setup() {
  Serial.begin(115200);
  stringIn.reserve(64);
}

void loop() {

  if(finString == true){                            //Si se confirma como terminado el string realiza las sgtes funciones
    counter = 0;
    stringIn.trim();
    finString = false;
      if(stringIn.equals("start")){
        stp = false;
        while(!stp){
          muestraAct = millis();   
          delta = (double) muestraAct - muestraAnt;
    
          if(delta >= T && counter <= ((lapse/T)-1)){
            valor = analogRead(pinRead);
            valor_map = map(valor, 0, 1023, 0, 5000);
            Serial.print(counter);
            Serial.print("  ");
            Serial.println(valor_map);
            muestraAnt = muestraAct;
            counter++;
            if(counter >((lapse/T)-1)){
              stp = true;
            }
          }
        }   
      }
      else if(stringIn.equals("stop")){
        Serial.println(stp);
      }
    stringIn = "";       
  }
}

void serialEvent(){
  while(Serial.available()){
    char charIn=(char)Serial.read();   //Lee lo que se introduce y lo convierte a char
    stringIn+=charIn;            //Agrega el char que se leyo al string
    
    if(charIn == '\n'){                  //Si se aprieta enter lo toma como un salto de linea y determina que se completo el string 
      finString = true;    
    } 
  }
}
