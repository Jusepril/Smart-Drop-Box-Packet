//----------- Library -------------//
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

//---------------------------------//
//        Deklarasi Variabel      //
//-------------------------------//
SoftwareSerial wemosSerial(10, 11); // RX, TX
const int relayPin1 = 12;
const int relayPin2 = 13;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);  
  wemosSerial.begin(9600);  
  
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Setup  selesai");
  lcd.setCursor(0, 1);
  lcd.print(" Dropbox System");
  Serial.println("Setup selesai, siap menerima data dari Wemos.");
}

void loop() {
  //---------------------------------//
  //   Logika saat barcode di scan   //
  //---------------------------------//
  if (wemosSerial.available() > 0) {
    Serial.println("Data tersedia dari Wemos."); // Tambahan debug
    String data = wemosSerial.readStringUntil('\n');
    data.trim();
    Serial.println("Data diterima dari Wemos: " + data);
    
    if (data == "Data terdaftar") {
      Serial.println("Nunga pas be hasian");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Resi Terdaftar ");
      lcd.setCursor(0, 1);
      lcd.print("Masukkan Paket");
      // Nyalakan relay
      //digitalWrite(relayPin1, HIGH);
      digitalWrite(relayPin2, HIGH);
      delay(5000);
      digitalWrite(relayPin2, LOW);
    } else if (data == "Data tidak terdaftar") {
      Serial.println("Resi Tidak Terdaftar");
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Resi Tidak ");
      lcd.setCursor(3, 1);
      lcd.print("Terdaftar");
      // Matikan relay jika data tidak sesuai
      digitalWrite(relayPin1, LOW);
      digitalWrite(relayPin2, LOW);
    }
    //---------------------------------//
    //        Membuka Pintu Atas       //
    //---------------------------------//
    else if (data == "Special barcode detected") {
      Serial.println("Barcode Spesial Terdeteksi");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Barcode Spesial");
      lcd.setCursor(0, 1);
      lcd.print("Terdeteksi");
      // Nyalakan relay
      digitalWrite(relayPin1, HIGH);
      delay(5000);
      digitalWrite(relayPin1, LOW);
    } else {
      // Jika ada pesan lain yang diterima
      Serial.println(data);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Paket Sudah ");
      lcd.setCursor(3, 1);
      lcd.print("Didalam");
      delay(5000);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Setup  selesai");
      lcd.setCursor(0, 1);
      lcd.print(" Dropbox System");
    }
  }
}