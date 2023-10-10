unsigned int trama[4]; //trama de datos ejemplo (maximo 15 bytes)

int crc(unsigned int, char); //prototipo

void setup() {
  Serial.begin(9600);

  trama[0] = 0x00;
  trama[1] = 0x64;
  trama[2] = 0x00;
  trama[3] = 0x5B;

  if (crc(4-2, 1)==1) {
    for (int i=0; i<4; i++) {
      Serial.print(trama[i], HEX);
    }
  }
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