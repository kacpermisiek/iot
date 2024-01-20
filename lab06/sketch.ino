#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

#define OLED_ADDR 0x3C //0x3D
#define OLED_SDA A5
#define OLED_SCL A4
#define JOY_X A1
#define JOY_Y A0
#define JOY_PIN 2
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SNAKE_SIZE 4
#define MAX_SNAKE_SIZE 100
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SPEAKER_PIN 8

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_ADDR);
// Zmienne dotyczące węża
int snakeX[MAX_SNAKE_SIZE];
int snakeY[MAX_SNAKE_SIZE];
int snakeSize = SNAKE_SIZE;
int snakeDir = 1;
int foodX=1, foodY=1;
bool gameOver = false;
bool gamePaused = false;

RTC_DS1307 rtc;

int points = 0;
// Zainicjuj węża
void initSnake() {
    int startX = SCREEN_WIDTH / 2;
    int startY = SCREEN_HEIGHT / 2;
    for (int i = 0; i < snakeSize; i++) {
    snakeX[i] = startX - i;
    snakeY[i] = startY;
    }
    foodX = random(0,SCREEN_WIDTH);
    foodY = random(0,SCREEN_HEIGHT);
}
// aktualizuj pozycje węża
void updateSnake() {
    int newHeadX = snakeX[0];
    int newHeadY = snakeY[0];
    display.println("Update snake");
    switch (snakeDir) {
        case 0: // w gore
        newHeadY--;
        break;
        case 1: // w prawo
        newHeadX++;
        break;
        case 2: // na dol
        newHeadY++;
        break;
        case 3: // w lewo
        newHeadX--;
        break;
    }
    // sprawdz kolizje ze scianami
    if (newHeadX < 0 || newHeadX >= SCREEN_WIDTH || newHeadY < 0 || newHeadY >= 
        SCREEN_HEIGHT) {
        gameOver = true;
        return;
    }
    // sprawdz kolizje węża z samym sobą
    for (int i = 1; i < snakeSize; i++) {
        if (newHeadX == snakeX[i] && newHeadY == snakeY[i]) {
        gameOver = true;
        return;
        }
    }
    // sprawdź czy jest kolizja z jedzeniem
    if (newHeadX == foodX && newHeadY == foodY) {
        // Increase snake size
        snakeSize++;
        points++;
        playSound();
        if (snakeSize > MAX_SNAKE_SIZE) {
        snakeSize = MAX_SNAKE_SIZE;
        }
        // wygeneruj nowa pozycje pokarmu
        foodX = random(0, SCREEN_WIDTH);
        foodY = random(0, SCREEN_HEIGHT);
    } else {
        // rusz wezem
        for (int i = snakeSize - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
    }
    snakeX[0] = newHeadX;
    snakeY[0] = newHeadY;
}

void playSound() {
    tone(SPEAKER_PIN, HIGH);
    delay(300);
    noTone(SPEAKER_PIN);
}

void displayDate() {
  DateTime now = rtc.now();

  display.setTextSize(1);
  display.setCursor(5, 40);
  display.print(now.day(), DEC);
  display.print(".");
  display.print(now.month(), DEC);
  display.print(".");
  display.print(now.year(), DEC);

  display.setCursor(5, 50);

  display.print(now.hour(), DEC);
  display.print(":");
  display.print(now.minute(), DEC);
}

// Narysuj węża i pokarm na ekranie OLED
void drawGame() {
    display.clearDisplay();
    

    // Narysuj weza
    for (int i = 0; i < snakeSize; i++) {
        display.fillRect(snakeX[i] , snakeY[i] , 4, 4, WHITE);
    }
    // Narysuj pokarm
    display.fillRect(foodX , foodY, 4, 4, WHITE);
    drawScore(); 
    display.display();
}

void drawScore() {
    display.setCursor(5, 5);
    display.print(String(points));
}

void validateGamePause() {
    if (digitalRead(JOY_PIN) != LOW) { 
      gamePaused = !gamePaused; 
      }
}

void setup() {
    //zainicjowanie generatora liczb losowych
    rtc.begin();
    randomSeed(A0); 
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    displayDate();
    display.setCursor(20, 10);
    display.println("Snake Game");
    display.display();
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(JOY_X, INPUT);
    pinMode(JOY_Y, INPUT);
    pinMode(JOY_PIN, INPUT_PULLUP);
    delay(2000);
    initSnake();
}
void loop() {
    validateGamePause();
    if (!gamePaused){
        updateSnake();
        drawGame();
        // Odczytaj wskazania joystika
        int joyX = analogRead(JOY_X);
        int joyY = analogRead(JOY_Y);
        // sprawdź kierunek wychyłu
        if (joyX < 100) {
        snakeDir = 1; // Left
        } else if (joyX > 900) {
        snakeDir = 3; // Right
        } else if (joyY < 100) {
        snakeDir = 2; // Up
        } else if (joyY > 900) {
        snakeDir = 0; // Down
        }
    }
    
    delay(20); //dostosowanie prędkości gry
    if (gameOver) {
    playSound();
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(10, SCREEN_HEIGHT / 2 - 10);
    display.println("Game Over");
    drawScore();
    displayDate();
    display.display();
    while (true) {
    // pętla nieskończona
    }
    }
}