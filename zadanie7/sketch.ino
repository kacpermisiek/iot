/*
Autor: Kacper Misiek
*/


#define LED_RED1 2
#define LED_YELLOW1 1
#define LED_GREEN1 0
#define LED_RED2 12
#define LED_YELLOW2 11
#define LED_GREEN2 10


#define BUTTON_1 3
#define BUTTON_2 13

#define YELLOW_LIGHT_DELAY 500
#define SWITCH_LIGHT_DELAY 100



void setup() {
  pinMode(LED_RED1, OUTPUT);
  pinMode(LED_YELLOW1, OUTPUT);
  pinMode(LED_GREEN1, OUTPUT);
  pinMode(LED_RED2, OUTPUT);
  pinMode(LED_YELLOW2, OUTPUT);
  pinMode(LED_GREEN2, OUTPUT);

  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);

  digitalWrite(LED_GREEN1, HIGH);
  digitalWrite(LED_RED2, HIGH);
  

}

void loop() {
    delay(100);
    if (digitalRead(BUTTON_2) == HIGH){
      delay(SWITCH_LIGHT_DELAY);
      digitalWrite(LED_GREEN1, LOW);
      digitalWrite(LED_RED2, LOW);

      digitalWrite(LED_YELLOW1, HIGH);
      digitalWrite(LED_YELLOW2, HIGH);

      delay(YELLOW_LIGHT_DELAY);

      digitalWrite(LED_YELLOW1, LOW);
      digitalWrite(LED_YELLOW2, LOW);

      digitalWrite(LED_RED1, HIGH);
      digitalWrite(LED_GREEN2, HIGH);
    }
  
    if (digitalRead(BUTTON_1) == HIGH){
      delay(SWITCH_LIGHT_DELAY);

      digitalWrite(LED_GREEN2, LOW);
      digitalWrite(LED_RED1, LOW);

      digitalWrite(LED_YELLOW2, HIGH);
      digitalWrite(LED_YELLOW1, HIGH);

      delay(YELLOW_LIGHT_DELAY);

      digitalWrite(LED_YELLOW2, LOW);
      digitalWrite(LED_YELLOW1, LOW);

      digitalWrite(LED_RED2, HIGH);
      digitalWrite(LED_GREEN1, HIGH);

    }

}
