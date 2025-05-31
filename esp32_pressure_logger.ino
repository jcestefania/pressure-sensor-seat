#include <WiFi.h>  // Librería para conectividad WiFi
#include <ESP_Google_Sheet_Client.h>  // Librería para conexión con Google Sheets
#include <GS_SDHelper.h>  // Librería de soporte para hojas de cálculo de Google
#include "secrets.h"  // Archivo que contiene las credenciales privadas y sensibles

// ID de la hoja de cálculo de Google donde se almacenan los datos
const char spreadsheetId[] = "12JwJ-bJrUJqnzQ1dzk9175PE_ACASiQFHZUj5EFcfo4";

// Servidor NTP para obtener la hora actual (necesaria para el timestamp)
const char* ntpServer = "pool.ntp.org";

// Definición de los pines del ESP32 conectados a los sensores de presión
const int sensorPins[4] = {34, 35, 32, 33};

// Variables para gestionar el intervalo de envío de datos
unsigned long lastTime = 0;
unsigned long timerDelay = 2000; // Intervalo de 2 segundos entre envíos
unsigned long epochTime; // Tiempo UNIX actual

// Función callback para mostrar información del estado del token de autenticación
void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error)
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  else
    GSheet.printf("Token status: %s\n", GSheet.getTokenStatus(info).c_str());
}

// Función que obtiene el tiempo actual desde el servidor NTP
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return 0;
  time(&now);
  return now;
}

// Configuración inicial del sistema
void setup() {
  Serial.begin(115200);  // Inicializa la comunicación por consola
  configTime(0, 0, ntpServer);  // Sincroniza la hora mediante NTP
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Inicia la conexión WiFi

  // Espera hasta que el ESP32 se conecte al WiFi
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Configura el cliente de Google Sheets
  GSheet.setTokenCallback(tokenStatusCallback);
  GSheet.setPrerefreshSeconds(600);  // Refresca el token cada 10 minutos
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
}

// Bucle principal
void loop() {
  // Comprueba si el cliente está listo y ha pasado el intervalo deseado
  bool ready = GSheet.ready();
  if (ready && millis() - lastTime > timerDelay) {
    lastTime = millis();

    // Array para almacenar los valores leídos de los sensores
    int sensorValues[4];
    for (int i = 0; i < 4; i++) {
      sensorValues[i] = analogRead(sensorPins[i]);  // Lectura analógica
    }

    // Obtiene la marca temporal actual
    epochTime = getTime();

    // Crea el objeto JSON con los datos a enviar
    FirebaseJson data;
    data.add("majorDimension", "COLUMNS");
    data.set("values/[0]/[0]", epochTime);  // Primera columna: tiempo
    for (int i = 0; i < 4; i++) {
      data.set("values/[" + String(i + 1) + "]/[0]", sensorValues[i]);  // Resto: sensores
    }

    // Envío de datos a Google Sheets
    FirebaseJson response;
    bool success = GSheet.values.append(&response, spreadsheetId, "Datos!A2", &data);

    // Mostrar por consola si la operación fue exitosa o no
    if (success)
      response.toString(Serial, true);
    else {
      Serial.println("Error en append:");
      response.toString(Serial, true);
      Serial.println(GSheet.errorReason());
    }
  }
}
