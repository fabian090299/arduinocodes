#include "U8glib.h"

// MOSI 51
// SCK 52
// CS 53

U8GLIB_ST7920_128X64 u8g(52, 51, 53, U8G_PIN_NONE);

String palabra = "x";

void setup() {
  u8g.setFont(u8g_font_unifont);  // Configura la fuente
  u8g.setContrast(255);           // Ajusta el contraste si es necesario
}

void loop() {
  // Muestra "Hola" en la pantalla
  u8g.firstPage();
  do {
    u8g.drawStr(0, 10, ">Set start (hs)");
    u8g.drawStr(0, 22, ">Set start (min)");
    u8g.drawStr(0, 34, ">Set end (hs)");
    u8g.drawStr(0, 46, ">Set end (min)");
    u8g.drawStr(0, 58, palabra.c_str());  // Convierte la cadena en un c-string
  } while (u8g.nextPage());

  delay(1000);  // Espera 1 segundo

  // Limpia el búfer de pantalla
  u8g.firstPage();
  do {
    // No se dibuja nada en este bloque, el búfer se considera limpio
  } while (u8g.nextPage());
  delay(10);  // Espera 1 segundo

  // Muestra "¿Qué tal?" en la pantalla
  u8g.firstPage();
  do {
    u8g.drawStr(0, 10, ">Linea 1");
    u8g.drawStr(0, 22, ">Linea 2");
    u8g.drawStr(0, 34, ">Linea 3");
    u8g.drawStr(0, 46, ">Linea 4");
    u8g.drawStr(0, 58, ">Linea 5");
  } while (u8g.nextPage());

  delay(1000);  // Espera 1 segundo
}
