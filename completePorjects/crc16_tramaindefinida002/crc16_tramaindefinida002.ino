unsigned int trama[12]; //trama de datos ejemplo (maximo 15 bytes)
int timeout = 2000;   //maximo tiempo que puede pasar entre paquetes

int crc(unsigned int, char); //prototipo
int checkBuffer();
unsigned int ReadFrame();
void write_exception_response(unsigned char);

void setup() {
  Serial.begin(9600);
}

void loop() {
}

int crc(unsigned int dataLength, char check){
  unsigned int registroCRC;
  unsigned int initialRegister = 0xffff;
  short int lsb = 0;
  short int shift = 0;
  unsigned int polynom = 0xA001;
  unsigned int lowCRC = 0;
  unsigned int highCRC = 0;
  int right;

  //__________INICIO DE LA GENERACION DEL CRC_________________________________________________________________

  for (int i=0; i<dataLength; i++) {
    //__________INICIO DE LA GENERACION DEL CRC_________________________________________________________________

    registroCRC = initialRegister^trama[i];     //realiza una xor entre el initial register y la trama de datos y guarda en crc register

    lsb = registroCRC&0x0001;           //detecta el lsb del crc register 
    registroCRC = (registroCRC >> 1);   //realiza el primer corrimiento
    shift++;

    while(shift < 8){
      if(lsb == 0){
        lsb = registroCRC&0x0001;
        registroCRC = (registroCRC >> 1);
        shift++;
    }
      if(lsb == 1){
        registroCRC = registroCRC^polynom;
        lsb = registroCRC&0x0001;       
        registroCRC = (registroCRC >> 1); 
        shift++;
      }
    }
    initialRegister = registroCRC;
    shift = 0;
  }

  highCRC = (registroCRC >> 8) & 0x00ff;
  lowCRC = (registroCRC) & 0x00ff;

  if(check == 1){   //con check 1 se verifica
    if(trama[dataLength+1] == highCRC && trama[dataLength] == lowCRC) return 1;   //si crc recibido = a crc generado, retorna 1 (correcto)
    else return 0;      //sino, retorna 0 (incorrecto)
  }
  else{
    trama[dataLength] = lowCRC;
    trama[dataLength+1] = highCRC;
    return 1;
  }
}

int checkBuffer(){
  unsigned int j = 0;
  
  while((!Serial.available()) && (j < timeout)){  //verifica el tiempo entre paquetes
    j++;
  }
  return Serial.available();  //retorna lo que se encuentra en el buffer
}

unsigned int ReadFrame(){
  int counter = 0;
  int stop = 0;

  while (!stop){
    if(!checkBuffer()){   //si deja de recibir datos deja de leer
      stop = 1;
    }
    else{
      if (counter<=12){   //mientras check buffer de 1 lee el puerto y guarda los datos en un array dinamico
        trama[counter] = Serial.read();
        counter++;
      }
    }
  }
       //Para procesar las tramas
  if(trama[0]==0x01){  
    if(crc(counter-2, 1)){     // Se verifica el CRC 
      if(trama[1] == 0x02){//Se verifica si las funciones son soportadas por el Slave
        //funcion 0x02 de modbus, leer entradas digitales
        //read_discrete_inputs();
      }
      else if(trama[1] == 0x0F){
        //funcion de error
        //write_multiple_coils();
      }
      else{
        //funcion de error
        write_exception_response(1);
      }
    }      

    else{   //si no coincide el crc declara un error 
      write_exception_response(0);
    }
  }
}

void write_exception_response(unsigned char error){
  unsigned char i;

  if(error>0){
    trama[1]+=0x80;
    trama[2]=error;
    crc(3,0);
    for (i=0; i<5; i++)
    Serial.write(trama[i]);
  }
}