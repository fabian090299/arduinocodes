#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x3f,16,2);    

/////////////////////////////////////////////////////variables y const PID//////////////////////////////////////////////////////////////////////////
#define sensor1 A0 
#define heater1 9 

int out = 0;    //salida 
float temp;       //Temperatura del Heater 1
float r1 = 0 ;    //SETPOINT
volatile float u = 0.0,u_1 = 0.0;  

float kp = 0.0,ti = 0.0, td = 0.0, ki = 0.0;    //Constantes PI


float q0,q1,q2;  
volatile float e = 0.0,e_1 = 0.0,e_2 = 0.0;

//////////////////////////////////////////////////////variables de tiempo de muestreo///////////////////////////////////8//////////////////////////////////

volatile unsigned long t = 0;
volatile unsigned long t_1 = 0;
volatile unsigned long delta = 0;
int T = 1000;                                //periodo de muestreo en mS
float Ts = 0;

////////////////////////////////////////////////////funcion de encoder//////////////////////////////////////////////////////////////////////////////////
#define clk 2
#define dt 5  

int seleccion = 0;

void event (){
    if (digitalRead(dt) == LOW){
      seleccion = 1;       
      Serial.print("False ");                       
    }
    else{
      seleccion = 2;
      Serial.print("True ");
    }
    delay(20);
return seleccion;
}

////////////////////////////////////////////////////funcion de boton//////////////////////////////////////////////////////////////////////////////////
#define button 8

int button_state;

int falling(int btn){
  int new_value = digitalRead(btn);
  delay(50);
  bool result = 0;

  if(button_state != new_value && new_value == 0){
    result = 1;
    button_state = new_value;
  }
  else{
    result = 0;
    button_state = new_value;
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define p0 0
#define p1 1 
#define p2 2
#define p3 3
#define p4 4
#define p5 5
#define p6 6
#define p7 7
#define p8 8

int estado = p0;   //estado actual-inicial

////////////////////////////////////////////////////valores/////////////////////////////////////////////////////////////////////////////////////////////

int tempSet = 0;
float kpSet = 0;
float kiSet = 0;

////////////////////////////////////////////////////VOID SETUP//////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  pinMode(heater1, OUTPUT);
  digitalWrite(heater1, LOW);
  pinMode(clk, INPUT);                                
  pinMode(dt, INPUT);

  analogReference(EXTERNAL);
  
  pinMode(button, INPUT_PULLUP);
  button_state = HIGH;

  attachInterrupt(digitalPinToInterrupt(clk), event, FALLING); //funcion para determinar interrupcion

  lcd.init();
  lcd.backlight();

  Ts = T/1000;

  printP0();
  estado = p0;
}

////////////////////////////////////////////////////inicio de funciones de impresiones de pantalla////////////////////////////////////////////////

void printP0(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("PI Temperature");
  lcd.setCursor(4,1);
  lcd.print("Control");
}

void printP1(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.setCursor(1,0);
  lcd.print("Temperature SP");
  lcd.setCursor(1,1);
  lcd.print("Kp Const.");
}

void printP2(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Temperature SP");
  lcd.setCursor(0,1);
  lcd.print(">");
  lcd.setCursor(1,1);
  lcd.print("Kp Const.");
}

void printP3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.setCursor(1,0);
  lcd.print("Ki Const.");
  lcd.setCursor(1,1);
  lcd.print("Start");
}

void printP4(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Ki Const.");
  lcd.setCursor(0,1);
  lcd.print(">");
  lcd.setCursor(1,1);
  lcd.print("Start");
}

void printP5(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Temperature");
  lcd.setCursor(0,1);
  lcd.print("Celsius: ");
  printTempSet();
}

void printTempSet(){
  lcd.setCursor(10,1);
  lcd.print(tempSet);
}

void printP6(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Kp");
  printKp();
}

void printKp(){
  lcd.setCursor(0,1);
  lcd.print(kpSet);  
}

void printP7(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Ki");
  printKi();
}

void printKi(){
  lcd.setCursor(0,1);
  lcd.print(kiSet);  
} 

void printP8(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Setpoint: ");
  lcd.setCursor(11, 0);
  lcd.print(tempSet);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  printTemp();
}

void printTemp(){
  lcd.setCursor(7, 1);
  lcd.print(temp);  
}

////////////////////////////////////////////////////VOID LOOP//////////////////////////////////////////////////////////////////////////////////

void loop() {
  switch(estado){
    case p0:            //HOME
      if(falling(button)){
        estado = p1;
        printP1();
        break;    //para que no siga en el loop  y parpadee la pantalla
      } 
    break;            //FIN HOME

    case p1:          //INICIO P1
      if(falling(button)){
        estado = p5;
        printP5();
        break;    //para que no siga en el loop  y parpadee la pantalla
      } 
      if(seleccion == 1){   //up
        estado = p4;
        printP4();
        seleccion = 3;
        break;
      }
      if(seleccion == 2){   //down
        estado = p2;
        printP2();
        seleccion = 3;
        break;
      }
    break;            //FIN P1      

    case p2:          //INICIO P2
      if(falling(button)){
        estado = p6;
        printP6();
        break;    //para que no siga en el loop  y parpadee la pantalla
      } 
      if(seleccion == 1){   //up
        estado = p1;
        printP1();
        seleccion = 3;
        break;
      }
      if(seleccion == 2){   //down
        estado = p3;
        printP3();
        seleccion = 3;
        break;
      }
    break;            //FIN P2

    case p3:          //INICIO P3
      if(falling(button)){
        estado = p7;
        printP7();
        break;    //para que no siga en el loop  y parpadee la pantalla
      } 
      if(seleccion == 1){   //up
        estado = p2;
        printP2();
        seleccion = 3;
        break;
      }
      if(seleccion == 2){   //down
        estado = p4;
        printP4();
        seleccion = 3;
        break;
      }
    break;            //FIN P3

    case p4:          //INICIO P4
      if(falling(button)){
        estado = p8;
        printP8();
        break;    //para que no siga en el loop  y parpadee la pantalla
      } 
      if(seleccion == 1){   //up
        estado = p3;
        printP3();
        seleccion = 3;
        break;
      }
      if(seleccion == 2){   //down
        estado = p1;
        printP1();
        seleccion = 3;
        break;
      }
    break;            //FIN P4

    case p5:
      if(falling(button)){
        estado = p1;
        printP1();
        break;
      }
      if(seleccion == 2){   //up
        tempSet++;
        if(tempSet>=100)tempSet = 100;
        seleccion = 3;
        break;
      }
      if(seleccion == 1){   //down
        tempSet--;
        if(tempSet<=0)tempSet = 0;
        seleccion = 3;
        break;
      } 
      printP5();
    break;

    case p6:        //INICIO P6
      if(falling(button)){
        estado = p2;
        printP2();
        break;
      }
      if(seleccion == 2){   //up
        kpSet = kpSet + 0.1;
        if(kpSet>=100)kpSet = 100;
        seleccion = 3;
        break;
      }
      if(seleccion == 1){   //down
        kpSet = kpSet - 0.1;
        if(kpSet<=0)kpSet = 0;
        seleccion = 3;
        break;
      } 
      printP6();
    break;          //FIN P6

    case p7:        //INICIO P7
      if(falling(button)){
        estado = p3;
        printP3();
        break;
      }
      if(seleccion == 2){   //up
        kiSet = kiSet + 0.1;
        if(kiSet>=100)kiSet = 100;
        seleccion = 3;
        break;
      }
      if(seleccion == 1){   //down
        kiSet = kiSet - 0.1;
        if(kiSet<=0)kiSet = 0;
        seleccion = 3;
        break;
      } 
      printP7();
    break;          //FIN P7

    case p8:        //INICIO P8
      if(falling(button)){
        estado = p0;
        printP0();
        kpSet = 0;
        kiSet = 0;
        tempSet = 0;
        analogWrite(heater1, 0);
        break;
      }
      
      t = millis();   
      delta = (double) t - t_1;

      if(delta >= T){
        temp= 3.3*100.0*analogRead(sensor1)/1024.0; 

        kp = kpSet;
        ki = kiSet;
        ti = 1/ki;
        r1 = tempSet;
         
        q0=kp*(1+Ts/(2.0*ti)+td/Ts);
        q1=-kp*(1-Ts/(2.0*ti)+(2.0*td)/Ts);
        q2=(kp*td)/Ts;;
        
        e=(r1-temp);
        u = u_1 + q0*e + q1*e_1 + q2*e_2;
        
        if (u >= 100.0)u = 100.0;  
        if (u <= 0.0 || r1==0)u = 0.0;    
        
        out = map(u, 0,100, 0,255);
        analogWrite(heater1, out);
    
    //    Serial.print("Temperatura: ");
    //    Serial.println(T1);
    //    Serial.print("Out: ");
    //    Serial.println(u);

      //  Serial.print((float)millis()/1000);
    //    Serial.print("  ");
        Serial.println(temp);
    //    Serial.print("  ");
    //     Serial.println(u);
    
        e_2=e_1;
        e_1=e;
        u_1=u;
        
        t_1 = t;
        printP8();
      }
    break;          //FIN P8
  }
}
