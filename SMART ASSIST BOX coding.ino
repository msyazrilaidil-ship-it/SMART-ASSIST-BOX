#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "DFRobotDFPlayerMini.h"


// ---------------- I2C ----------------
#define SDA_PIN 21
#define SCL_PIN 22


// ---------------- BUTTON ----------------
#define BUTTON1_PIN 32
#define BUTTON2_PIN 33
#define BUTTON3_PIN 25
#define BUTTON4_PIN 26
#define BUTTON5_PIN 27


// ---------------- WIFI ----------------
const char* ssid = "Infinix HOT 40i";
const char* password = "boli3137";


// ---------------- TELEGRAM ----------------
String botToken = "8977356410:AAE6RJte57YNxybYtmO37qj0-_z1L126vU8";
String chatID = "1826486432";


// ---------------- THINGSPEAK ----------------
const char* thingSpeakAPIKey = "7MXCLP2D85FIYCZO";


// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);


// ---------------- DFPLAYER ----------------
#define PIN_MP3_TX 14   // ESP32 TX → DFPlayer RX
#define PIN_MP3_RX 13   // ESP32 RX ← DFPlayer TX

#define MP3_BUSY_PIN 23  // DFPlayer BUSY → ESP32 GPIO 23

HardwareSerial mp3Serial(2);

DFRobotDFPlayerMini player;

int currentTrack = 1;


// ---------------- BUTTON VARIABLES ----------------
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;
bool lastButtonState4 = HIGH;
bool lastButtonState5 = HIGH;

bool buttonState1 = HIGH;
bool buttonState2 = HIGH;
bool buttonState3 = HIGH;
bool buttonState4 = HIGH;
bool buttonState5 = HIGH;


// --------------------------------------------------
// WiFi Status
// --------------------------------------------------
void showWiFiStatus() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Wifi Connected");

  lcd.setCursor(0, 1);
  lcd.print("System Ready");

  delay(1000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("System Ready");

  lcd.setCursor(0, 1);
  lcd.print("Pilih Button");
}


// --------------------------------------------------
// Telegram
// --------------------------------------------------
void sendTelegram(String message) {

  if (WiFi.status() == WL_CONNECTED) {

    WiFiClientSecure client;
    client.setInsecure();

    message.replace(" ", "%20");
    message.replace("\n", "%0A");

    String url =
      "https://api.telegram.org/bot" +
      botToken +
      "/sendMessage?chat_id=" +
      chatID +
      "&text=" +
      message;

    HTTPClient https;

    if (https.begin(client, url)) {

      int httpCode = https.GET();

      Serial.print("Telegram : ");
      Serial.println(httpCode);

      https.end();
    }
  }
}


// --------------------------------------------------
// ThingSpeak
// --------------------------------------------------
void sendThingSpeak(int fieldNumber) {

  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    String url =
      "http://api.thingspeak.com/update?api_key=";

    url += thingSpeakAPIKey;
    url += "&field";
    url += fieldNumber;
    url += "=1";

    http.begin(client, url);

    int httpCode = http.GET();

    Serial.print("ThingSpeak Field ");
    Serial.print(fieldNumber);
    Serial.print(" : ");
    Serial.println(httpCode);

    http.end();
  }
}


// --------------------------------------------------
// Play Track
// --------------------------------------------------
void playTrack(int trackNumber) {

  player.play(trackNumber);

  currentTrack = trackNumber;

  Serial.print("Playing Track ");
  Serial.println(trackNumber);
}


// --------------------------------------------------
// Setup
// --------------------------------------------------
void setup() {

  Serial.begin(115200);


  // I2C LCD
  Wire.begin(SDA_PIN, SCL_PIN);

  lcd.begin();
  lcd.backlight();


  // Button input
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(BUTTON4_PIN, INPUT_PULLUP);
  pinMode(BUTTON5_PIN, INPUT_PULLUP);


  // ---------------- DFPLAYER ----------------

  pinMode(MP3_BUSY_PIN, INPUT);

  mp3Serial.begin(
    9600,
    SERIAL_8N1,
    PIN_MP3_RX,
    PIN_MP3_TX
  );


  if (player.begin(mp3Serial)) {

    Serial.println("DFPlayer OK");

    player.volume(30);

  } else {

    Serial.println("Connecting to DFPlayer Mini failed!");
  }


  // LCD WiFi Connecting
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Connecting");

  lcd.setCursor(0, 1);
  lcd.print("WiFi...");


  // Start WiFi
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }


  Serial.println();
  Serial.println("WiFi Connected");


  showWiFiStatus();
}


// --------------------------------------------------
// Loop
// --------------------------------------------------
void loop() {


  // Read buttons
  buttonState1 = digitalRead(BUTTON1_PIN);
  buttonState2 = digitalRead(BUTTON2_PIN);
  buttonState3 = digitalRead(BUTTON3_PIN);
  buttonState4 = digitalRead(BUTTON4_PIN);
  buttonState5 = digitalRead(BUTTON5_PIN);


  // ================= BUTTON 1 =================
  if (lastButtonState1 == LOW && buttonState1 == HIGH) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 1");

    lcd.setCursor(0, 1);
    lcd.print("Saya Nak Makan");


    // Play Track 1
    playTrack(1);


    // Telegram
    sendTelegram("Button 1 ditekan\nSaya Nak Makan");


    // ThingSpeak Field 1
    sendThingSpeak(1);
  }


  // ================= BUTTON 2 =================
  if (lastButtonState2 == LOW && buttonState2 == HIGH) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 2");

    lcd.setCursor(0, 1);
    lcd.print("Saya Nak Mandi");


    // Play Track 2
    playTrack(2);


    // Telegram
    sendTelegram("Button 2 ditekan\nSaya Nak Mandi");


    // ThingSpeak Field 2
    sendThingSpeak(2);
  }


  // ================= BUTTON 3 =================
  if (lastButtonState3 == LOW && buttonState3 == HIGH) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 3");

    lcd.setCursor(0, 1);
    lcd.print("Saya Nak Tidur");


    // Play Track 3
    playTrack(3);


    // Telegram
    sendTelegram("Button 3 ditekan\nSaya Nak Tidur");


    // ThingSpeak Field 3
    sendThingSpeak(3);
  }


  // ================= BUTTON 4 =================
  if (lastButtonState4 == LOW && buttonState4 == HIGH) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 4");

    lcd.setCursor(0, 1);
    lcd.print("Saya Ke Tandas");


    // Play Track 4
    playTrack(4);


    // Telegram
    sendTelegram("Button 4 ditekan\nSaya Nak Pergi Tandas");


    // ThingSpeak Field 4
    sendThingSpeak(4);
  }


  // ================= BUTTON 5 =================
  if (lastButtonState5 == LOW && buttonState5 == HIGH) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 5");

    lcd.setCursor(0, 1);
    lcd.print("SoS (emergency)");


    // Play Track 5
    playTrack(5);


    // Telegram
    sendTelegram("Button 5 ditekan\nSoS (emergency)");


    // ThingSpeak Field 5
    sendThingSpeak(5);
  }


  // Save button states
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  lastButtonState4 = buttonState4;
  lastButtonState5 = buttonState5;


  delay(20);
}
