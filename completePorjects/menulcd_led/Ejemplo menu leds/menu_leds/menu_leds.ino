#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);


//:::::::::::ENCODER::::::::::::::::::::::::::
#define outputA 6
#define outputB 7
#define sw 4

#define led1 8
#define led2 9
#define led3 10

int counter = -1; 
int aState;
int aLastState;  
String Scount;




//::::::::::MENU::::::::::::::::::::::
String menu1[] = {"Led1","Led2","Led3","Todos","Secuencias","Intensidad"};
int sizemenu1 = sizeof(menu1) / sizeof(menu1[0]);

String menu2[] = {"On","Off","Atras"};
int sizemenu2 = sizeof(menu2) / sizeof(menu2[0]);

String menu3[] = {"Izq -> Der","Der -> Izq","Atras"};
int sizemenu3 = sizeof(menu3) / sizeof(menu3[0]);

String linea1,linea2;
int seleccion = 0;
int pos = 0;
int level_menu = 0;
int opcion = 0;
bool btnpress = false;
byte brilloled3 = 0;
byte flecha[] = {B00000,B00100,B00110,B11111,B00110,B00100,B00000,B00000};

void setup() {
  Serial.begin(9600);
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode (sw,INPUT_PULLUP);
  digitalWrite(sw, HIGH);
  pinMode(led1,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, flecha);
  fn_credits();
  fn_menu(counter,menu1,sizemenu1);
 

}

void loop() {

  selectOption();


 //menu 1
  if(level_menu == 0){
 
    if(fn_encoder(sizemenu1) ){
      fn_menu(counter,menu1,sizemenu1);
    }


    
    if(btnpress){
      //led1
      if(counter == 0){
          counter = 0;
          fn_menu(counter,menu2,sizemenu2);
          level_menu = 1;  
      }

      //led2
      if(counter == 1){
          counter = 0;
          fn_menu(counter,menu2,sizemenu2);
          level_menu = 2;  
      }

      //led3
      if(counter == 2){
          counter = 0;
          fn_menu(counter,menu2,sizemenu2);
          level_menu = 3;  
      }


      //todos
      if(counter == 3){
          counter = 0;
          fn_menu(counter,menu2,sizemenu2);
          level_menu = 4;  
      }


      //secuencias
      if(counter == 4){
          counter = 0;
          fn_menu(counter,menu3,sizemenu3);
          level_menu = 5;  
      }

      //Intensidad
      if(counter == 5){
          counter = brilloled3; 
          level_menu = 6; 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Brillo Led3"); 
          lcd.setCursor(0, 1);
          lcd.print(brilloled3);
           
      }
     
      btnpress = false;
    }

  }







  if(level_menu == 1){

    Serial.println("level1");
    if(fn_encoder(sizemenu2)){
      fn_menu(counter,menu2,sizemenu2);
    }
    
    if(btnpress){

      if(counter == 0){
        digitalWrite(led1,HIGH);
      }

      if(counter == 1){
        digitalWrite(led1,LOW);
      }
      
      if(counter == 2){
          counter = 0;
          fn_menu(counter,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }







  if(level_menu == 2){

    if(fn_encoder(sizemenu2)){
      fn_menu(counter,menu2,sizemenu2);
    }
    
    if(btnpress){

      if(counter == 0){
        digitalWrite(led2,HIGH);
      }

      if(counter == 1){
        digitalWrite(led2,LOW);
      }
      
      if(counter == 2){
          counter = 1;
          fn_menu(counter,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }




  if(level_menu == 3){

    if(fn_encoder(sizemenu2)){
      fn_menu(counter,menu2,sizemenu2);
    }
    
    if(btnpress){

      if(counter == 0){
        digitalWrite(led3,HIGH);
        brilloled3 = 10;
      }

      if(counter == 1){
        digitalWrite(led3,LOW);
        brilloled3 = 0;
      }
      
      if(counter == 2){
          counter = 2;
          fn_menu(counter,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }





  if(level_menu == 4){

    if(fn_encoder(sizemenu2)){
      fn_menu(counter,menu2,sizemenu2);
    }
    
    if(btnpress){

      if(counter == 0){
        digitalWrite(led1,HIGH);
        digitalWrite(led2,HIGH);
        digitalWrite(led3,HIGH);
        brilloled3 = 10;
      }

      if(counter == 1){
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
        brilloled3 = 0;
      }
      
      if(counter == 2){
          counter = 3;
          fn_menu(counter,menu1,sizemenu1);
          level_menu = 0;   
      }
      
      btnpress = false;
    }
  }





  if(level_menu == 5){

    if(fn_encoder(sizemenu3)){
      fn_menu(counter,menu3,sizemenu3);
    }
    
    if(btnpress){

      if(counter == 0){
        digitalWrite(led1,HIGH);
        delay(500);
        digitalWrite(led1,LOW);
        digitalWrite(led2,HIGH);
        delay(500);
        digitalWrite(led2,LOW);
        digitalWrite(led3,HIGH);
        delay(500);
        digitalWrite(led3,LOW);
        brilloled3 = 0;
      }

      if(counter == 1){
        digitalWrite(led3,HIGH);
        delay(500);
        digitalWrite(led3,LOW);
        digitalWrite(led2,HIGH);
        delay(500);
        digitalWrite(led2,LOW);
        digitalWrite(led1,HIGH);
        delay(500);
        digitalWrite(led1,LOW);
        brilloled3 = 0;
      }
      

      if(counter == 2){
          counter = 4;
          fn_menu(counter,menu1,sizemenu1);
          level_menu = 0; 
      }
      
      
      btnpress = false;
    }
  }




  if (level_menu == 6){
  
   
    if(fn_encoder(11)){
      brilloled3 = counter;
       fn_intensidad();
    }

    if(btnpress){
      counter = 5;
      level_menu = 0;

      fn_menu(counter,menu1,sizemenu1);
      btnpress = false;
    }
  
  
    
  }






}












void fn_menu(int pos,String menus[],byte sizemenu){
  lcd.clear();
  linea1="";
  linea2="";
   
  if((pos % 2) == 0){

     lcd.setCursor(0, 0);
     lcd.write(byte(0));
     linea1 = menus[pos];
    
    if(pos+1 != sizemenu){
      linea2 = menus[pos+1];
    }
    
  }else{
    linea1 = menus[pos-1];
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    linea2 = menus[pos];
  }
  
     lcd.setCursor(1, 0);
     lcd.print(linea1);

     lcd.setCursor(1, 1);
     lcd.print(linea2); 
   
}


bool fn_encoder(byte sizemenu){ 
  bool retorno = false;
  aState = digitalRead(outputA); 
  if (aState != aLastState){     
    if (digitalRead(outputB) != aState) { 
      counter --;
    } else {
      counter ++;
    }

    if(counter <=0){
      counter = 0;
    }
    
    if(counter >= sizemenu-1 ){
      counter = sizemenu-1;
    }
    
    retorno = true;

  } 
  aLastState = aState;
  return retorno; 
}




void selectOption(){
  if(digitalRead(sw) == LOW){
    delay(500);
    btnpress = true;
  }
}




void fn_intensidad(){
  analogWrite(led3,map(brilloled3,0,10,0,255));
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(brilloled3);
}


void fn_credits(){
  lcd.setCursor(0, 0);
  lcd.print("La Buhardilla");
  
  lcd.setCursor(0, 1);
  lcd.print("Custom menu 1.0");
  delay(3000);
}
