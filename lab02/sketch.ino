#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


//diody
#define HOTRGB 7
#define NORMALRGB 6
#define COLDRGB 5
#define HOT 4
#define NORMAL 3
#define COLD 2
//pomiar temperatury
float celsius = 0;
float hotTemp = 26;
float coldTemp = 15;
const float BETA = 3950; // powinien pasować do współczynnika Beta termistora

const String INFO_HOT = "Za goraco!";
const String INFO_NORMAL = "W sam raz";
const String INFO_COLD = "Za zimno!";

void setup() {
lcd.init();
lcd.backlight();
// Początkowy tekst na wyswieltaczu
lcd.setCursor(0, 0);
lcd.print("Pomiar temp.");
lcd.setCursor(2, 1);
lcd.print(" ");
//diody
pinMode(HOT, OUTPUT);
pinMode(NORMAL, OUTPUT);
pinMode(COLD, OUTPUT);
pinMode(HOTRGB, OUTPUT);
pinMode(NORMALRGB, OUTPUT);
pinMode(COLDRGB, OUTPUT);
Serial.begin(9600);
}
void loop() {
delay(1000);
// odczytuje stan czujnika temperatury i konwertuje wynik na stopnie Celsjusza
int analogValue = analogRead(A0);
// konwertuje miliwolty na stopnie Celsjusza
float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) -
273.15;

lcd.setCursor(0, 0);
lcd.print(String(celsius) + "st. Celc.");
lcd.setCursor(0, 1);


digitalWrite(COLDRGB, HIGH);
digitalWrite(NORMALRGB, HIGH);
digitalWrite(HOTRGB, HIGH);
if ( celsius < coldTemp )
{
digitalWrite(COLD, HIGH);
digitalWrite(COLDRGB, LOW);
lcd.print(INFO_COLD);
delay(1000);
digitalWrite(COLD, LOW);
digitalWrite(COLDRGB, HIGH);
}
else if ( celsius > coldTemp && celsius <= hotTemp )
{
digitalWrite(NORMAL, HIGH);
digitalWrite(NORMALRGB, LOW);
lcd.print(INFO_NORMAL);
delay(1000);
digitalWrite(NORMAL, LOW);
digitalWrite(NORMALRGB, HIGH);
}
else
{
// celsius > hotTemp
digitalWrite(HOT, HIGH);
digitalWrite(HOTRGB, LOW);
lcd.print(INFO_HOT);
delay(1000);
digitalWrite(HOT, LOW);
digitalWrite(HOTRGB, HIGH); 
}
lcd.clear();
}