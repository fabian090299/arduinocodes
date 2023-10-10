/*
PROYECTO: Sistema de Riego Automatizado

AUTOR:

DESCRIPCION: Sistema de riego automatico, utilizando un display LCD 16x2 i2c, un modulo joystick, modulo relé para accionamiento,
modulo RTC real time clock, arduino MEGA 2560.
Se implementa un menu mediante una maquina de estados finitos (FSM), sin ninguna libreria adicional de menu para tener mayor control
del codigo implementado. 

=================================================================================================================
Contenido del menu:

El desplazamiento entre lineas se realiza con el movimiento del joystick en el eje Y.

Menu (pagina1): 
  >Fecha y Hora: Haciendo click se ingresa para verificar la hora y fecha actuales (click para volver a la pantalla principal).
  >Prog. Riego: Haciendo click se ingresa a un submenu (submenu 1) para programacion de riego. 

Menu (pagina2):
  >Riego manual: Haciendo click se ingresa a un submenu (submenu2) para activar o desactivar el riego manual.

Submenu 1: se accede haciendo click en ">Prog. Riego", el contenido es:
  >Set In (hs): donde se setea la hora en la que se desea iniciar el riego automatico.
  >Set In (min): donde se especifica el minuto cuando comienza el riego.
    (desplazar joystick hacia abajo)
  >Set Fin (hs): donde se setea la hora en la que se desea culminar el riego automatico.
  >Set Fin (min): donde se especifica el minuto cuando termina el riego.
    (desplazar el joystick hacia abajo)
  >Start: comenzar la espera para el riego automatico.
  >Stop: detener de manera forzada el riego automatico una vez que ya haya comenzado.
    (desplazar el joystick hacia abajo)
  >Reset: resetear los valores de horas y minutos a 0.
  >Atras: lleva a la pantalla principal.

Submenu 2: se accede haciendo click en ">Riego manual", el contenido es:
  >Start manual: haciendo click se activa el riego en ese instante.
  >Stop manual: haciendo click se desactiva el riego en ese instante.
    (desplazar el joystick hacia abajo)
  >Atras: haciendo click regresa a la pantalla principal (menu pagina 1).

=================================================================================================================
CONEXION
-LCD i2c a bus i2c de arduino (pines SCL y SDA) en MEGA: 20, 21, en UNO: A4 y A5.
-RTC (a bus i2c igual que LCD, resto de pines no conectados)
-JOYSTICK: VCC 5V, GND, SWITCH: 7, VRY: A1, VRX: no conectado.
-RELE: VCC 5V, GND, CONTROL: 8.
=================================================================================================================
Se puede utilizar displays LCD 20x4 o cualquiera que tenga puerto i2c para mas practicidad (conexion identica).
*/

#include <RTClib.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x3f,16,2);  
RTC_DS3231 rtc;

uint8_t sweepY = 0;
uint8_t axisY = 0;

uint16_t act = 0;
uint16_t ant = 0;

bool releIn = true;
bool releFin = true;

uint8_t hora = 0;
uint8_t minutos = 0;
uint8_t dia = 0;
uint8_t mes = 0;
uint16_t an = 0;

//================================================================================================================================
#define button 7

int button_state;

int falling(int btn) {
  int new_value = digitalRead(btn);
  delay(50);
  bool result = 0;

  if (button_state != new_value && new_value == 0) {
    result = 1;
    button_state = new_value;
  } else {
    result = 0;
    button_state = new_value;
  }
  return result;
}

//================================================================================================================================
#define s0 0
#define s1 1 
#define s2 2
#define s3 3
#define s4 4
#define s5 5
#define s6 6
#define s7 7
#define s8 8
#define s9 9
#define s10 10
#define s11 11  
#define s12 12  //fecha y hora 
#define s13 13  //prog riego
#define s14 14  //riego manual
#define s15 15  //mostrar fecha y hora
#define s16 16  //start manual
#define s17 17  //stop manual 
#define s18 18 //atras
#define s19 19 //atras p/ pantalla principal



int estado = s0;   //estado actual-inicial
//================================================================================================================================
uint8_t horaIn = 0, minIn = 0, horaFin = 0, minFin = 0;

//================================================================================================================================
#define rele 8
//================================================================================================================================

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(rele, OUTPUT);
  button_state = HIGH;
  digitalWrite(rele, HIGH);

  if(!rtc.begin())Serial.println("RTC no encontrado");

  lcd.init();
  lcd.backlight(); // Activa el backlight

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sistema de Riego");
  lcd.setCursor(3,1);
  lcd.print("Automatico");
  delay(2000);  

  printS12();
  estado = s12;   //comienza en el estado 12, pagina principal
}

void printS0(){           //set inicio hora
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Set Start (hs)");
  lcd.setCursor(0,1);
  lcd.print(" Set Start (min)");
}

void printS1(){         //set inicio min
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Set Start (hs)");
  lcd.setCursor(0,1);
  lcd.print(">Set Start (min)");
}

void printS2(){         //set fin hora
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Set Fin (hs)");
  lcd.setCursor(0,1);
  lcd.print(" Set Fin (min)");
}

void printS3(){         //set fin min
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Set Fin (hs)");
  lcd.setCursor(0,1);
  lcd.print(">Set Fin (min)");
}

void printS4(){         //start
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Start");
  lcd.setCursor(0,1);
  lcd.print(" Stop");
}

void printS5(){         //stop
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Start");
  lcd.setCursor(0,1);
  lcd.print(">Stop");
}

void printS6(){     // reset
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Reset");
  lcd.setCursor(0,1);
  lcd.print(" Atras");
}

void printS7(){       //contador de hora de inicio
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Inicio en:");
  lcd.setCursor(1,1);
  lcd.print(horaIn);
  lcd.setCursor(4,1);
  lcd.print("hs");
}

void printS8(){     //contador de min de inicio
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Inicio en:");
  lcd.setCursor(1,1);
  lcd.print(minIn);
  lcd.setCursor(4,1);
  lcd.print("min");
}

void printS9(){   //contador de hora de fin
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Fin en: (hs)");
  lcd.setCursor(1,1);
  lcd.print(horaFin);
  lcd.setCursor(4,1);
  lcd.print("hs");
}

void printS10(){    //contador de min de fin
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Fin en: (min)");
  lcd.setCursor(1,1);
  lcd.print(minFin);
  lcd.setCursor(4,1);
  lcd.print("min");
}

void printS11(){    //pantalla de espera a riego
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Riego Programado");
  lcd.setCursor(0,1);
  lcd.print(horaIn);
  lcd.setCursor(2,1);
  lcd.print(":");
  lcd.setCursor(3,1);
  lcd.print(minIn);

  lcd.setCursor(6,1);
  lcd.print(" A: ");
  lcd.setCursor(10,1);
  lcd.print(horaFin);
  lcd.setCursor(12,1);
  lcd.print(":");
  lcd.setCursor(13,1);
  lcd.print(minFin);
}

void printS12(){    // fecha y hora
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Fecha y Hora");
  lcd.setCursor(0,1);
  lcd.print(" Prog. Riego");
}

void printS13(){    // prog riego
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Fecha y Hora");
  lcd.setCursor(0,1);
  lcd.print(">Prog. Riego");
}

void printS14(){    // riego manual
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Riego Manual");
}

void printS15(){    // mostrar fecha y hora
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(dia);
  lcd.setCursor(2,0);
  lcd.print("/");
  lcd.setCursor(3,0);
  lcd.print(mes);
  lcd.setCursor(5,0);
  lcd.print("/");
  lcd.setCursor(6,0);
  lcd.print(an);

  lcd.setCursor(0,1);
  lcd.print(hora);
  lcd.setCursor(2,1);
  lcd.print(":");
  lcd.setCursor(3,1);
  lcd.print(minutos);
  lcd.setCursor(6,1);
  lcd.print("hs");
}

void printS16(){    // start manual
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Start manual");
  lcd.setCursor(0,1);
  lcd.print(" Stop manual");
}

void printS17(){    // Stop manual
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Start manual");
  lcd.setCursor(0,1);
  lcd.print(">Stop manual");
}

void printS18(){    // atras
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Reset");
  lcd.setCursor(0,1);
  lcd.print(">Atras");
}

void printS19(){    // atras
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">Atras");
}


void loop() {
  //======================================================================================================
  //=================================LECTURA DE JOYSTICK============================================
  //======================================================================================================

  act = analogRead(axisY);
  delay(100);

  if ((ant != act) && (act >= 1020)) {
    ant = act;
    sweepY = 1;
    Serial.println("arriba");
  } 
  else if((ant != act) && (act <= 10)){
    Serial.println("abajo");
    ant = act;
    sweepY = 2;
  }
  else {
    ant = act;
    sweepY = 0;
  }
  //======================================================================================================
  //======================================================================================================
  //======================================================================================================

  DateTime time = rtc.now();
  dia = time.day();
  mes = time.month();
  an = time.year();
  hora = time.hour();
  minutos = time.minute();

  switch(estado){
    case s0:
      if(sweepY == 1){
        estado = s18;
        printS18();
        break;
      }
      if(sweepY == 2){
        estado = s1;
        printS1();
        break;
      }
      if(falling(button)){
        estado = s7;
        printS7();
        break;
      }
    break;

    case s1:
      if(sweepY == 1){
        estado = s0;
        printS0();
        break;
      }
      if(sweepY == 2){
        estado = s2;
        printS2();
        break;
      }
      if(falling(button)){
        estado = s8;
        printS8();
        break;
      }
    break;

    case s2:
      if(sweepY == 1){
        estado = s1;
        printS1();
        break;
      }
      if(sweepY == 2){
        estado = s3;
        printS3();
        break;
      }
      if(falling(button)){
        estado = s9;
        printS9();
        break;
      }
    break;

    case s3:
      if(sweepY == 1){
        estado = s2;
        printS2();
        break;
      }
      if(sweepY == 2){
        estado = s4;
        printS4();
        break;
      }
      if(falling(button)){
        estado = s10;
        printS10();
        break;
      }
    break;

    case s4:
      if(sweepY == 1){
        estado = s3;
        printS3();
        break;
      }
      if(sweepY == 2){
        estado = s5;
        printS5();
        break;
      }
      if(falling(button)){
        if((horaIn==horaFin) && (minIn==minFin)){
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("Set Invalido");
          delay(2000);
          estado = s4;
          printS4();
        }
        else{
          estado = s11;
          digitalWrite(rele, HIGH);
          printS11();
          break;
        }
      }
    break;

    case s5:
      if(sweepY == 1){
        estado = s4;
        printS4();
        break;
      }
      if(sweepY == 2){
        estado = s6;
        printS6();
        break;
      }
      if(falling(button)){
        digitalWrite(rele, HIGH);
        estado = s5;
        printS5();
        break;
      }
      
    break;

    case s6:
      if(sweepY == 1){
        estado = s5;
        printS5();
        break;
      }
      if(sweepY == 2){
        estado = s18;
        printS18();
        break;
      }
      if(falling(button)){
        digitalWrite(rele, HIGH);//logica inversa
        horaIn=0;
        minIn=0;
        horaFin=0;
        minFin=0;
        estado = s0;
        printS0();
        break;
      }
    break;

    case s7:
      if(falling(button)){
        estado = s0;
        printS0();
        break;
      }
      if(sweepY == 1){
        horaIn++;
        if(horaIn>=23) horaIn=23;
        estado = s7;
        printS7();
        break;
      }
      if(sweepY == 2){
        horaIn--;
        if(horaIn<=0) horaIn=0;
        estado = s7;
        printS7();
        break;
      }
    break;

    case s8:
      if(falling(button)){
        estado = s1;
        printS1();
        break;
      }
      if(sweepY == 1){
        minIn=minIn+1;
        if(minIn>=59) minIn=59;
        estado = s8;
        printS8();
        break;
      }
      if(sweepY == 2){
        minIn=minIn-1;
        if(minIn==0) minIn=0;
        estado = s8;
        printS8();
        break;
      }
    break;

    case s9:
      if(falling(button)){
        estado = s2;
        printS2();
        break;
      }
      if(sweepY == 1){
        horaFin++;
        if(horaFin>=23) horaFin=23;
        estado = s9;
        printS9();
        break;
      }
      if(sweepY == 2){
        horaFin--;
        if(horaFin<=0) horaFin=0;
        estado = s9;
        printS9();
        break;
      }
    break;

    case s10:
      if(falling(button)){
        estado = s3;
        printS3();
        break;
      }
      if(sweepY == 1){
        minFin++;
        if(minFin>=59) minFin=59;
        estado = s10;
        printS10();
        break;
      }
      if(sweepY == 2){
        minFin--;
        if(minFin<=0) minFin=0;
        estado = s10;
        printS10();
        break;
      }
    break;

    case s11:
      if(falling(button)){
        estado = s4;
        printS4();
        break;
      }
      if(time.hour() == horaIn && time.minute() == minIn){
        if(releIn){
          digitalWrite(rele, LOW);
          Serial.println("rele ON");
          releIn = false;   
        }
      }
      if(time.hour() == horaFin && time.minute() == minFin){
        if(releFin){
          digitalWrite(rele, HIGH);
          Serial.println("rele OFF");
          releFin = false;   
        }
      }
      if(time.hour() == 1 && time.minute() == 0){//a las 1 de la mañana se resetea el estado de lso reles
        releIn=true;
        releFin=true;
      }
    break;

    case s12:
      if(falling(button)){
        estado = s15;
        printS15();
        break;
      }
      if(sweepY == 1){
        estado = s14;
        printS14();
        break;
      }
      if(sweepY == 2){
        estado = s13;
        printS13();
        break;
      }
    break;

    case s13:
      if(falling(button)){
        estado = s0;
        printS0();
        break;
      }
      if(sweepY == 1){
        estado = s12;
        printS12();
        break;
      }
      if(sweepY == 2){
        estado = s14;
        printS14();
        break;
      }
    break;

    case s14:
      if(falling(button)){
        estado = s16;
        printS16();
        break;
      }
      if(sweepY == 1){
        estado = s13;
        printS13();
        break;
      }
      if(sweepY == 2){
        estado = s12;
        printS12();
        break;
      }
    break;

    case s15:
      printS15();
      delay(1);

      if(falling(button)){
        estado = s12;
        printS12();
        break;
      }
    break;

    case s16:
      if(falling(button)){
        estado = s16;
        digitalWrite(rele, LOW);
        printS16();
        break;
      }
      if(sweepY == 1){
        estado = s19;
        printS19();
        break;
      }
      if(sweepY == 2){
        estado = s17;
        printS17();
        break;
      }
    break;

    case 17:
      if(falling(button)){
        estado = s17;
        digitalWrite(rele, HIGH);
        printS17();
        break;
      }
      if(sweepY == 1){
        estado = s16;
        printS16();
        break;
      }
      if(sweepY == 2){
        estado = s19;
        printS19();
        break;
      }
    break;

    case 18:
      if(falling(button)){
        estado = s12;
        printS12();
        break;
      }
      if(sweepY == 1){
        estado = s6;
        printS6();
        break;
      }
      if(sweepY == 2){
        estado = s0;
        printS0();
        break;
      }
    break;

    case 19:
      if(falling(button)){
        estado = s14;
        printS14();
        break;
      }
      if(sweepY == 1){
        estado = s17;
        printS17();
        break;
      }
      if(sweepY == 2){
        estado = s16;
        printS16();
        break;
      }
    break;
  }
}