int lectura=A0;
float valor;
float volt;
float temp;

void setup() {
  pinMode(lectura, INPUT);
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  valor=analogRead(A0);
  volt=(valor/1023.0)*3300;
  temp=volt/10;
 Serial.print("TEMPERATURA: ");
  Serial.println(volt);
 Serial.println(" *C");
  delay(500);
}
