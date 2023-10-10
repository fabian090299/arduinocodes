//String input;
char input;
int buffer[12];
int SlaveAddress = 0x01;
int timeout;
int i;
int q;
int coils[9];

//calculo de CRC16 
int CRC16(unsigned int dataLength, char check)
{
   unsigned int i,j, CheckSum;
   unsigned int  lowCRC, highCRC;
   CheckSum = 0xFFFF;

       for (j=0;j<dataLength;j++)
         {
          CheckSum=CheckSum^buffer[j];
            for(i=8;i>0;i--)
            {
                if((CheckSum)&0x0001==1)
                  CheckSum=(CheckSum>>1)^0xA001;
                  else CheckSum=CheckSum>>1;

            }
          }

    highCRC =(CheckSum>>8)&0x00FF;
    lowCRC =CheckSum&0x00FF;
    CheckSum=((CheckSum>>8)&0x00FF)|((CheckSum<<8)&0xFF00);

     if (check==1)  //check 1 es para verificar crc cuando se recibe 
    {
      if ( (buffer[dataLength+1] == highCRC) & (buffer[dataLength] == lowCRC ))
        return 1;
      else
        { q = 1;
          return 0;
        }
    }
    else  //check 0 es para generar crc cuando se trasmite
    {
      buffer[dataLength] = lowCRC;
      buffer[dataLength+1] = highCRC;
      return 1;
    }

}

///Fin CRC


////////////////////////////////////////////////////////////////////////////////
/* Lista de errores MODBUS
   01: Illegal Function
   02: Illegal Data Addres
   03: Illegal Data Value
   04: Slave Device Failure
*/

 //Formatea la rpta de excepcion
void write_exception_response(unsigned char error)
{
  unsigned char i;

  if (error>0)
  { buffer[1]+=0x80;
    buffer[2]=error;
    CRC16(3,0);
    for (i=0; i<5; i++)
    Serial.write(buffer[i]);
  }

}//Fin rpta excepcion


////////////////////////////////////////////////////////////////////////////////

/*read_coils()

byte 0: direccion esclavo
byte 1: codigo de funcion
byte 2: dirrecion de inicio (byte alto)
byte 3: dirrecion de inicio (byte bajo)
byte 4: cantidad de entradas (byte alto)
byte 5: cantidad de entradas (byte bajo)
byte 6: CRC (alto)
byte 7: CRC (bajo)


*/

//Funcion Modbus 0x02
void read_discrete_inputs(void)
{
    unsigned char i, n, k;
    unsigned int byte1 = 0x00;
    unsigned int byte2 = 0x00;
    unsigned int check, m;

    //Se verifica si la dirrecion de inicio o cantidad de entradas son correctas
    check = 9 - buffer[3];
    if(!(buffer[3]>8 || buffer[5]>check))
      {
          if(buffer[5] == 0x09)
          {
            for(i=0;i<8;i++)
             {
              byte1 = byte1 | coils[7-i];
               if(i<7)
               {byte1 = byte1<<1;}
             }

          }
          else
          {
           for(i=buffer[3];i<(buffer[3] + buffer[5]);i++)
             {
              //byte1 = byte1 | coils[(buffer[3] + buffer[5]  - 1) - i];

               coils[i] = coils[i]<<7;
               byte1 = byte1 | coils[i];

               //Usart_Write(coils[i]);
              if(i<(buffer[3] + buffer[5]) - 1)
                {byte1 = byte1>>1;}


             }
              byte1 = byte1>>(8-buffer[5]);

          }


          buffer[3] = byte1;
          
          if(buffer[5] == 0x09)
            {byte2 = byte2 | coils[8];
             buffer[2] = 2;
             buffer[4] = byte2;
             n = 7;
            }
          else
             {buffer[2] = 1;
              n = 6;
             }


    /*Respuesta MODBUS
    byte 0: direccion esclavo
    byte 1: codigo de funcion
    byte 2: conteo de bytes
    byte 3: coils byte 1
    byte 4: coils byte 2
    byte 5: CRC (alto)
    byte 6: CRC (bajo)
    */

    m = n-2;
    CRC16(m,0);

    //PORTD.F0 = 1;
    for (i=0; i<n; i++){
    Serial.write(buffer[i]);
    }

  }

    else
    {  //Se formatea la respuesta de excepción
       write_exception_response(2);
    }
  
}


///Verifica datos recibidos 
    int CheckInBuffer(void)
    {
      unsigned int j = 0;
      //digitalWrite(8,HIGH);
      while((!Serial.available()) && (j < timeout))
      { j++; }

      return Serial.available();
    }   //End Check buffer


//Lee tramas 
unsigned int ReadFrame(void)
      {
       int counter = 0;
       int stop = 0;
       //digitalWrite(8,HIGH);
       while (!stop)
       { if(!CheckInBuffer())
         {
         stop = 1;
         }
         else
          {

           if (counter<=12)
              {buffer[counter] = Serial.read();
               counter++;

              }
          }


       }

       //Para procesar las tramas
       if(buffer[0]==0x01){  
         
          // Se verifica el CRC
         if(CRC16(counter - 2,1)){ 
           //Se verifica si las funciones son soportadas por el Slave
            if(buffer[1] == 0x02)
              {
               read_discrete_inputs();
              }
            else if(buffer[1] == 0x0F){
               //write_multiple_coils();
              }
            else{
               write_exception_response(1);
               }
           }//Fin prueba CRC = OK

         else{
            write_exception_response(0);
            }//No CRC
       }//Fin verifica esclavo 
  }// end ReadFrame    
       



void setup(){
 
  pinMode(8, OUTPUT); // Declaramos que utilizaremos el pin 13 como salida
  Serial.begin(9600);
  timeout = 2000;
}
 
void loop(){
  digitalWrite(8,LOW);
      coils[0]= 0;   //Sensor X1
      coils[1]= 0;   //Sensor X2
      coils[2]= 0;   //Sensor X3
      coils[3]= 0;   //Sensor X4
      coils[4]= 1;   //Sensor Z1
      coils[5]= 1;   //Sensor Z2
      coils[6]= 1;   //Sensor Y1
      coils[7]= 1;   //Sensor Y2
      coils[8]= 1;   //Sensor Y3
  if (Serial.available()>0){
   
      ReadFrame();
  
    }
  
  
}
