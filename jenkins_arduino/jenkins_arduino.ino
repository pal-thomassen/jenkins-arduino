const int buttonPin = 2;
const int ledPin = 13;
const int goodPin = 12;
const int busyPin = 11;
const int failPin = 10;
const String buttonPressedMessage = "start_job\n";

String inputString = "";       
boolean stringComplete = false;
int buttonState = 0;

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 6, 5, 4, 3);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(goodPin, OUTPUT);
  pinMode(busyPin, OUTPUT);
  pinMode(failPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.print(buttonPressedMessage);
  }
  
  if (stringComplete) { 
   resetStatusLeds();   
   if (inputString.indexOf("good") >= 0) {
     digitalWrite(goodPin, HIGH);
     lcd.clear();
     lcd.print("All good!");
   } else if (inputString.indexOf("running") >= 0) {
     digitalWrite(busyPin, HIGH);
     lcd.clear();
     lcd.print("Bygger...");
   } else if (inputString.indexOf("fail") >= 0) {
     Serial.println("Setting fail output");
     digitalWrite(failPin, HIGH);
     lcd.clear();
     lcd.print("Bygg feilet...");
   }
    
   Serial.println(inputString);
   inputString = "";
   stringComplete = false;
  }
  
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

void resetStatusLeds() {
  digitalWrite(goodPin, LOW);
  digitalWrite(busyPin, LOW);
  digitalWrite(failPin, LOW);
}

