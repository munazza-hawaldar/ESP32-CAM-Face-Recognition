/*
  ESP32-CAM camera server (AI-Thinker board, OV2640 camera)

  Initialises the camera, connects to Wi-Fi, starts the camera web server
  and prints the IP address on the Serial Monitor (115200 baud).

  Transcribed from the project report (Step 2: Source code for ESP32-CAM).

  Needs two more files in the same sketch folder, taken from the ESP32
  "CameraWebServer" example in the Arduino IDE:
    - camera_pins.h   (pin map for the AI-Thinker board)
    - app_httpd.cpp   (defines startCameraServer())

  Uploading: short IO0 to GND, press upload (press RESET if you see dots and
  dashes), then remove IO0 from GND and press RESET again.
*/

#include "esp_camera.h"
#include <WiFi.h>

// Camera Model - AI Thinker
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// WiFi Credentials - replace with your own, do not commit real ones
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

void startCameraServer();

void setup() {
  Serial.begin(115200);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Lower the resolution to reduce lag
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;  // 320x240 (fastest for web)
    config.jpeg_quality = 15;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed!");
    return;
  }

  // WiFi Connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  startCameraServer();
  Serial.println("\nWiFi Connected!");
  Serial.print("Use this IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(10000);
}
