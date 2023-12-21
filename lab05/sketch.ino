#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"




#define DHTPIN 2 // Czujnik DHT podpięty do pinu numer 2 
#define PHOTORESISTOR_PIN A0
#define DHTTYPE DHT22 // DHT 22 (AM2302)
#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2
#define RGB_RED 11
#define RGB_GREEN 10
#define RGB_BLUE 9


LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
RTC_DS1307 rtc;

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

bool warning;
bool error;
const float GAMMA = 0.7;
const float RL10 = 50;
float photoresistance;



void setup() {
    Serial.begin(9600);
    dht.begin();
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);
    delayMS = sensor.min_delay / 1000;
    pinMode(DHTPIN, INPUT);
    pinMode(RGB_RED, OUTPUT);
    pinMode(RGB_GREEN, OUTPUT);
    pinMode(RGB_BLUE, OUTPUT);

    lcd.init();
    lcd.backlight();

    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
}
void loop() {
    delay(delayMS);
    warning = false;
    error = false;

    sensors_event_t event;
    dht.temperature().getEvent(&event);

    if (isnan(event.temperature)) {
        Serial.println(F("Błąd w odczycie temperatury!"));
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("t:");
      lcd.print(event.temperature);

      if (event.temperature >= 25 || event.temperature <= 15){warning = true; Serial.println("TEMPERATURE WARNING");}
      if (event.temperature >= 30 || event.temperature <= 10){error = true; Serial.println("TEMPERATURE ERROR");}
    }

    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
    Serial.println(F("Błąd w odczycie wilgotności!"));
    }
    else {
      lcd.setCursor(8, 0);
      lcd.print("h:");
      lcd.print(event.relative_humidity);

      if (event.relative_humidity >= 70 || event.relative_humidity <= 30){warning = true; Serial.println("HUMIDITY WARNING");}
      if (event.relative_humidity >= 80 || event.relative_humidity <= 20){error = true; Serial.println("HUMIDITY ERROR");}
    }
    lcd.setCursor(0, 1);
    photoresistance = analogRead(PHOTORESISTOR_PIN);
    float voltage = photoresistance / 1024. * 5;
    float resistance = 2000 * voltage / (1 - voltage /5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
    lcd.print("l:");
    lcd.print(lux);
    if (lux >= 1000 || lux <= 500){warning = true; Serial.println("LIGHT WARNING");}
    if (lux >= 1500 || lux <= 300){error = true; Serial.println("LIGHT ERROR");}

    DateTime now = rtc.now();
    lcd.setCursor(11, 1);
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    lcd.print(now.minute(), DEC);

    if (error){
      analogWrite(RGB_RED, 1023);
      analogWrite(RGB_GREEN, LOW);
      analogWrite(RGB_BLUE, LOW);
    }
    else if(warning){
      analogWrite(RGB_RED, 1023);
      analogWrite(RGB_GREEN, 1023);
      analogWrite(RGB_BLUE, LOW);
    }
    else{
      analogWrite(RGB_RED, LOW);
      analogWrite(RGB_GREEN, 1023);
      analogWrite(RGB_BLUE, LOW);
    }
}