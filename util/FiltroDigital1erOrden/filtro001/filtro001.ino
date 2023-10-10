//filtro digital, filtro discretizado 

//Parametros de ecuacion en tiempo discreto
double vok = 0;
double w0 = 0;
double h0 = 0;
double vikmenos1 = 0;
double vokmenos1 = 0;
double R1 = 1000;
double R2 = 1000;
double C = 0;

double f0 = 0.3;  //frecuencia de corte del filtro

// Parametros de muestreo
volatile unsigned long tiempoAct = 0;
volatile unsigned long tiempoAnt = 0;
volatile unsigned long delta = 0;
double T = 100000;        //periodo en microsegundos    //10 veces frecuencia maxima del sistema
double micro = 1000000;

//entradas y salidas
double input = 0;
double input_noise = 0;
double vi = 0;
double vo = 0;

// selector de modo
char modo = 'a'; //modo "a" lectura de sensor sin filtrar; modo "b" lectura filtrada
int ref = 3300; //voltaje de referencia analogico en mV
int noiselevel = 0; //nivel de ruido

void setup() {
  Serial.begin(115200);
  analogReference(EXTERNAL);
  randomSeed(analogRead(A0));

  //calculo de valor del capacitor
  C = 1/(2*3.14159*R2*f0);

  //calculo de los parametros del filtro
  w0 = 1/(R2*C);  //frecuencia del filtro
  h0 = -R2/R1;  //ganancia del filtro
}

void loop() {
  //muestreo
  tiempoAct = micros();   
  delta = (double) tiempoAct - tiempoAnt;
      
  if(delta >= T){
    //algoritmo del filtro digital
    input = analogRead(A0);
    input_noise = random(-noiselevel, noiselevel);
    input += input_noise;

    vi = (input*ref)/1023;    //transforma en tension el valor analogico medido

    switch(modo){
      case 'a':
        Serial.print((double)micros()/1000000);
        Serial.print("  ");
        Serial.println(vi);
      break;
      case 'b':
        vok = (T/micro)*w0*h0*vikmenos1 + (1-(T/micro)*w0)*vokmenos1;

        vo = vok*(-1);
        Serial.print((double)micros()/1000000);
        Serial.print("  ");
        Serial.println(vo);

        vikmenos1 = vi;
        vokmenos1 = vok;
      break;
      default:  Serial.println("No function");   
    }
    
    tiempoAnt = tiempoAct;
  }  
}