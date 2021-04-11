#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

const char* ssid = "NETWORK_NAME"; // WiFi network name goes here
const char* password =  "NETWORK_PASS"; // WiFi password goes here

void setup() {
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("WiFi Connected");
  
  u8g2.begin();
  u8g2_set_default_settings();
  u8g2.drawStr(0, 0, "WiFi Connected");
  u8g2.sendBuffer();
  delay(1000);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    http.begin("https://api.kraken.com/0/public/Ticker?pair=adausd"); //Update crypto ticker here
    int httpCode = http.GET(); //Make the request
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        const char* crypto_price = doc["result"]["ADAUSD"]["c"][0]; //Update crypto ticker here
        Serial.println(crypto_price);
        
        u8g2.clearBuffer();
        u8g2.drawBox(0, 0, 127, 63);
        u8g2.sendBuffer();
        u8g2.setFont(u8g2_font_9x18B_mf); //Small font
        u8g2.setDrawColor(0);
        const char crypto_name[]   = "Cardano (ADA)"; //Currency name and ticker
        u8g2.drawStr(0, 13, crypto_name);
        u8g2.drawStr(0, 33, crypto_price);
        u8g2.sendBuffer();
        u8g2_set_default_settings();
        delay(10000); //Wait 10 seconds between price updates
      }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
}

void u8g2_set_default_settings() {
  u8g2.setFont(u8g2_font_6x12_mf); //Small font
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}
