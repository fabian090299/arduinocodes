String string_entrada= "";  
bool fin_string= false;

void setup() {
  Serial.begin(9600);
  string_entrada.reserve(64);               //Reserva un espacio de hasta 64bytes
}

void loop() { 
  if(fin_string){                            //Si se confirma como terminado el string realiza las sgtes funciones
    string_entrada.trim();                   // trim() impide que se imprima el valor en ascii del espacio y del salto de linea
    fin_string=false;                        //Vuelve a setear como false el fin string
    Serial.println(string_entrada);            //imprime lo que se ha escrito en el string_entrada
    string_entrada= "";                      //Vacia el buffer de entrada 
  }
}

void serialEvent(){
  while(Serial.available()){
    char char_entrada=(char)Serial.read();   //Lee lo que se introduce y lo convierte a char
    string_entrada+=char_entrada;            //Agrega el char que se leyo al string
    
    if(char_entrada=='\n'){                  //Si se aprieta enter lo toma como un salto de linea y determina que se completo el string 
      fin_string=true;    
    } 
  }
}
