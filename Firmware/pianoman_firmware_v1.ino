#include "USB.h"
#include "USBHIDKeyboard.h"
#include <Keypad.h>

USBHIDKeyboard Keyboard;

// --- Rotary Encoder Pins ---
const int ENCODER_A = D8;  // Pin connected to Encoder Phase A
const int ENCODER_B = D9;  // Pin connected to Encoder Phase B
const int ENCODER_SW = D10; // Pin connected to Encoder Push Switch

// Variables for tracking rotary encoder state
volatile int encoderValue = 0;
int lastEncoderAState;

// --- Keypad Setup (4 Rows x 3 Columns) ---
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns

// Define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Connect to the row pinouts of the keypad (matching your schematic D3 to D6)
byte rowPins[ROWS] = {D3, D4, D5, D6}; 

// Connect to the column pinouts of the keypad (matching your schematic D0 to D2)
byte colPins[COLS] = {D0, D1, D2}; 

// Initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  // Start Serial communication for debugging
  Serial.begin(115200);
  delay(1000);
  Serial.println("Macropad Initialized!");

  // Configure Rotary Encoder Pins
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  // Read the initial state of Encoder A
  lastEncoderAState = digitalRead(ENCODER_A);
}

void loop() {
  // 1. Check for Key Matrix Presses
  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.print("Key Pressed: ");
    Serial.println(customKey);
    // Add your custom actions/shortcuts here (e.g., SendKeyboard library commands)
  }

  // 2. Check Rotary Encoder Rotation
  int currentEncoderAState = digitalRead(ENCODER_A);
  if (currentEncoderAState != lastEncoderAState) {
    // If the output B state is different from the output A state, it's rotating counter-clockwise
    if (digitalRead(ENCODER_B) != currentEncoderAState) {
      encoderValue++;
      Serial.print("Encoder Turned CW: ");
      Serial.println(encoderValue);
    } else {
      encoderValue--;
      Serial.print("Encoder Turned CCW: ");
      Serial.println(encoderValue);
    }
  }
  lastEncoderAState = currentEncoderAState;

  // 3. Check Rotary Encoder Push Button
  if (digitalRead(ENCODER_SW) == LOW) {
    Serial.println("Encoder Button Pressed!");
    delay(200); // Simple debounce delay
  }
}