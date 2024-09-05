//----------Library-----------//
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>

//---------------------------------//
//       Deklarasi Variabel        //
//---------------------------------//
SoftwareSerial barcodeSerial(14, 12); // RX, TX 
WiFiClient client;
const char* ssid = "Infinite";
const char* password = "Gratias30";
const char* serverName = "http://192.168.104.20/Dropbox_System/includes/HTTP_Request.php"; 
String barcodeData;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  
  barcodeSerial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("Setup selesai.");
}

void loop() {
  //---------------------------------//
  //        Barcode Scanner          //
  //---------------------------------//
  while (barcodeSerial.available()) {
    delay(2);
    char c = barcodeSerial.read();
    barcodeData += c; 
  }

  if (barcodeData.length() > 0) {
    barcodeData.trim();
    Serial.println("Barcode scanned: " + barcodeData);

    // Logika spesial untuk barcode '14S22035'
    // Membuka Hanya pintu atas    
    if (barcodeData == "14S22035") {
      Serial1.println("Special barcode detected"); 
      Serial.println("Special barcode detected");
    } else {
      //---------------------------------//
      //     HTTP Request ke server      //
      //---------------------------------//
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(client, serverName);

        String postData = "barcode=" + barcodeData;
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.POST(postData);
        
        if (httpResponseCode > 0) {
          String payload = http.getString();
          Serial.println("Server response: " + payload);

          //---------------------------------//
          //    Komunikasi serial Arduino    //
          //---------------------------------//
          if (payload.indexOf("data ada") >= 0) {
            Serial1.println("Data terdaftar");
            Serial.println("Data terdaftar");
          } else if (payload.indexOf("data tidak ada") >= 0) {
            Serial1.println("Data tidak terdaftar");
            Serial.println("Data tidak terdaftar");
          }
        } else {
          Serial.print("Ada Kesalahan ");
          Serial.println(httpResponseCode);
          Serial1.println("Data terdaftar");
          Serial.println("Data terdaftar");
        }

        http.end();
      } else {
        Serial.println("WiFi Disconnected");
        WiFi.begin(ssid, password); // Attempt to reconnect
      }
    }

    // Reset variabel barcodeData setelah ditampilkan dan dikirim
    barcodeData = "";
  }
}