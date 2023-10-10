#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Definir los pines GPIO para los sensores de flotación
#define SENSOR1_PIN 12
#define SENSOR2_PIN 14
#define SENSOR3_PIN 27
#define SENSOR4_PIN 26
#define SENSOR5_PIN 25

// Definir las credenciales de la red WiFi
#define WIFI_SSID "Amplify"
#define WIFI_PASSWORD "AMPLIFY2018"

// Definir la dirección IP estática
IPAddress staticIP(192, 168, 100, 50);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 100, 1);

// Crear una instancia del servidor web
AsyncWebServer server(80);

void setup() {
  // Iniciar el puerto serie para la depuración
  Serial.begin(115200);

  // Configurar los pines GPIO como entrada
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(SENSOR4_PIN, INPUT);
  pinMode(SENSOR5_PIN, INPUT);

  // Desconectar de cualquier red WiFi anterior
  WiFi.disconnect(true);
  delay(1000);

  // Configurar la dirección IP estática
  WiFi.config(staticIP, gateway, subnet, dns);

  // Conectar a la red WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }

  Serial.println("Conectado a la red WiFi");

  // Configurar la ruta de la API REST
  server.on("/niveles-de-agua", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";

    // Leer el estado de los sensores y agregarlos al JSON
    if(!digitalRead(SENSOR5_PIN) && digitalRead(SENSOR4_PIN)){
      json += "\"variable1\": \"Error en la secuencia de sensores\",";
      json += "\"play\": 0";
    }
    else if(!digitalRead(SENSOR4_PIN) && digitalRead(SENSOR3_PIN)){
      json += "\"variable1\": \"Error en la secuencia de sensores\",";
      json += "\"play\": 0";
    }
    else if(!digitalRead(SENSOR3_PIN) && digitalRead(SENSOR2_PIN)){
      json += "\"variable1\": \"Error en la secuencia de sensores\",";
      json += "\"play\": 0";
    }
    else if(!digitalRead(SENSOR2_PIN) && digitalRead(SENSOR1_PIN)){
      json += "\"variable1\": \"Error en la secuencia de sensores\",";
      json += "\"play\": 0";
    }
    else if(!digitalRead(SENSOR5_PIN)){
      json += "\"variable5\": \"1 metro: posibilidad de arrastre para vehículos de pequeño y mediano porte\",";
      json += "\"play\": 5";
    }
    else if(!digitalRead(SENSOR4_PIN)){
      json += "\"variable4\": \"75cm: alerta para vehículos de mediano porte\",";
      json += "\"play\": 4";
    }
    else if(!digitalRead(SENSOR3_PIN)){
      json += "\"variable3\": \"45cm. alertas para motocicletas y vehículos de pequeño porte\",";
      json += "\"play\": 3";
    }
    else if(!digitalRead(SENSOR2_PIN)){
      json += "\"variable2\": \"30cm. alerta para peatones\",";
      json += "\"play\": 2";
    }
    else if(!digitalRead(SENSOR1_PIN)){
      json += "\"variable1\": \"15cm. primera alerta de posible inundación\",";
      json += "\"play\": 1";
    }
    else{
      json += "\"variable1\": \"sensores_no_activos\",";
      json += "\"play\": 0";
    }

    json += "}";

    // Enviar la respuesta
    request->send(200, "application/json", json);
  });

  // Iniciar el servidor web
  server.begin();
}

void loop() {
  // No es necesario
}
