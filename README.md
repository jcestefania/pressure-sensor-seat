# Pressure Sensor Seat - Smart Occupancy Detection 🪑📊

> 🏆 **Bachelor's Thesis (TFG) awarded with Distinction (Matrícula de Honor - 9.6/10)**.
> Universidad CUNEF - Engineering School.

## 📄 Full Documentation
You can read the full academic dissertation here:
👉 **[Download Project Memory (PDF)](./docs/TFG_Pressure_Sensor_System.pdf)**

---

This project is a smart occupancy detection system for seats using piezoresistive pressure sensors (MD30-60) and an ESP32 microcontroller. It is designed for real-time monitoring and wireless data transmission to the cloud using Google Sheets.

## Features

- Reads analog pressure values from 4 sensors.
- Sends timestamped data to a Google Sheets document via WiFi.
- Uses Google Cloud service account for secure authentication.
- Code written in C++ using the Arduino framework.

## System Architecture

The ESP32 reads data from 4 piezoresistive sensors connected as voltage dividers. The values are processed and sent every few seconds to a Google Sheets spreadsheet via the `ESP_Google_Sheet_Client` library.

## Hardware Components

- ESP32-S3 DevKitC-1
- 4 × MD30-60 piezoresistive pressure sensors
- 4 × 10kΩ resistors (for voltage divider)
- Dupont cables, breadboard, USB-C cable

## Software Setup

1. **Libraries Needed** (Install via Library Manager):
   - `WiFi.h`
   - `ESP_Google_Sheet_Client`
   - `GS_SDHelper`
   - `FirebaseJson`

2. **Google Cloud Setup**:
   - Create a Google Cloud project.
   - Generate service account credentials.
   - Share the target Google Sheet with the service account email.
   - Store credentials in a separate `secrets.h` file.

3. **`secrets.h` Structure**:
   ```cpp
   #define WIFI_SSID "YourWiFiSSID"
   #define WIFI_PASSWORD "YourWiFiPassword"
   #define PROJECT_ID "your-google-project-id"
   #define CLIENT_EMAIL "your-service-account-email"
   const char PRIVATE_KEY[] PROGMEM = R"EOF(
   -----BEGIN PRIVATE KEY-----
   YourPrivateKeyHere
   -----END PRIVATE KEY-----
   )EOF";

## Data Format
The Google Sheet stores data with the following structure:
```
| Timestamp (Unix) | Sensor 1 | Sensor 2 | Sensor 3 | Sensor 4 |
