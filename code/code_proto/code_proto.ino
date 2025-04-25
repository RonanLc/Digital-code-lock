
/* ### Parameters for the keyboard ### */

#include <Keypad.h>

#define ROWS    4 /* four rows */
#define COLS    4 /* four columns */

/* define the symbols on the buttons of the keypads */
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'0','F','E','D'}
};

byte rowPins[ROWS] = {4,  5,  6,  7}; /* connect to the row pinouts of the keypad */
byte colPins[COLS] = {8,  9, 10, 11}; /* connect to the column pinouts of the keypad */

/* initialize an instance of class NewKeypad */
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


/* ### Parameters for the LED strip ### */

#include <Adafruit_NeoPixel.h>

#define LED_PIN   3
#define LED_NUM   5

Adafruit_NeoPixel led(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);

int i = 0;


/* ### Parameters for the garage door actuator ### */

#define DOOR_PIN  2


/* ### main ### */

// ----------- CODE UTILISATEUR ------------
const int codeLength = 5;
char enteredCode[codeLength + 1]; // +1 pour le '\0'
byte codeIndex = 0;
unsigned long lastKeyPressTime = 0;
const unsigned long inputTimeout = 5000; // 5 secondes
char storedCode[codeLength + 1] = {'A','2','6','0','8','\0'};

void setup(){
  pinMode(DOOR_PIN, OUTPUT);

  Serial.begin(9600);
  led.begin();
  led.clear();

  led.setPixelColor(0, led.Color(0, 10, 0));
  led.show();
  delay(1000);
  for (int i = 0; i < 5; i++) {
    led.setPixelColor(i, led.Color(10, 10, 10));
  }
  led.show();
  delay(1);
}

void loop(){
  char key = keypad.getKey();

  if (key){
    Serial.println(key);

    if (codeIndex < codeLength) {
      led.setPixelColor(codeIndex, led.Color(0, 0, 10));
      led.show();
      delay(100);
      enteredCode[codeIndex++] = key;
      lastKeyPressTime = millis();
    }
  }

  // Vérifie si délai dépassé après dernière touche
  if (codeIndex > 0 && millis() - lastKeyPressTime > inputTimeout || codeIndex == codeLength) {
    enteredCode[codeIndex] = '\0';
    Serial.print("Code entré : ");
    Serial.println(enteredCode);

    /* char storedCode[codeLength + 1];
    for (int i = 0; i < codeLength; i++) {
      storedCode[i] = EEPROM.read(i);
    }
    storedCode[codeLength] = '\0'; */

    if (strcmp(enteredCode, storedCode) == 0) {
      Serial.println("Code correct !");
      
      digitalWrite(DOOR_PIN, 1);

      for (int i = 0; i < 5; i++) {
        led.setPixelColor(i, led.Color(0, 10, 0));
      }
      led.show();
      delay(1);

      delay(500);
      digitalWrite(DOOR_PIN, 0);

      delay(1500);
      for (int i = 0; i < 5; i++) {
        led.setPixelColor(i, led.Color(10, 10, 10));
      }
      led.show();
      delay(1);

    } else {
      Serial.println("Code incorrect.");
      
      for (int i = 0; i < 5; i++) {
        led.setPixelColor(i, led.Color(10, 0, 0));
      }
      led.show();
      delay(2000);
      for (int i = 0; i < 5; i++) {
        led.setPixelColor(i, led.Color(10, 10, 10));
      }
      led.show();
      delay(1);
    }

    codeIndex = 0;
  }

}
