
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Jhon Nunez - 650243534
//  Christian Dominguez - 677492330
//  
//  FINAL PROJECT - SMART LOCK
//  Description - 
//
//  Assumptions:
//
//
//
//
//
//  References:
//    Keyboard - https://playground.arduino.cc/Main/KeypadTutorial
//
//
//
//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Defines LED, Buzzer and LCD
#include <LiquidCrystal.h> 
#include <Keypad.h> //include keypad library - first you must install library (library link in the video description)
#include <Wire.h>

#define LED_G 13
//#define LED_R 12
#define LOCK 11
#define buzzer 9
const int PowerPin = 10;

const byte rows = 4;  //number of the keypad's rows
const byte cols = 3;  //number of the keypad's columns

char keyMap [rows][cols] = { //define the buttons of the keypad

  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
  
};
byte rowPins[rows] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[cols] = {2, 3, 4}; //connect to the column pinouts of the keypad

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);

//Initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
//LiquidCrystal lcd (A0, A1, A2, A3, A4, A5); // pins of the LCD. (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd (A0, A1, A2, A3, 12, 11); // pins of the LCD. (RS, EN, D4, D5, D6, D7)


char mystr[10]; //Initialized variable to store recieved data


void setup() {
  pinMode(LED_G, OUTPUT);     // Initialize green LED
  //pinMode(LED_R, OUTPUT);     // Initialize red LED
  pinMode(LOCK, OUTPUT);      // Initialize Lock 
  pinMode(buzzer, OUTPUT);    // Initialize Buzzer
  lcd.begin(16, 2);           // Initialize LCD   
  pinMode(PowerPin, OUTPUT);
  Serial.begin(9600);         // Serial 
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}

int codeStartX = 6;
int posX = codeStartX;
int ACCESS = 0;
String key_combination = "9999";
String input; 
String str;

void loop() {

  //Serial.readBytes(mystr,4); //Read the serial data and store in var
  
  if (Serial.available() > 0){
    Serial.println(Serial.read());
  }

  
  //Display default message
  lcd.setCursor(2,0);
  lcd.print("ENTER CODE:");
  
  char key = myKeypad.getKey(); //define which key is pressed with getKey

  if ( str.equals(key_combination) ){//button pressed
    lcd.clear();
    Serial.println(str);
    grantACCESS();
    lcd.clear();
  }

  
  if (key != NO_KEY){

    
    if (posX == codeStartX + 3){//only 4 inputs

      input += key;     //test
      Serial.println(input);    //test input string


      if (input.equals(key_combination)){//if input is equals to 
        ACCESS = 1;
      }
      
      
      //print last character
      lcd.setCursor(posX-1,1);
      lcd.print("*");
      lcd.setCursor(posX,1); 
      lcd.print(key);   

      if (ACCESS == 1){//if key combination is correct******
        lcd.clear();
        grantACCESS();
        lcd.clear();
        
      }
      else {//Try again
        lcd.clear();
        
        lcd.setCursor(3,0);
        lcd.print("TRY AGAIN!");
        posX = codeStartX; //reset position 


        input = ""; //clear input

        //Turn on red LED
        //digitalWrite(LED_R,HIGH);
        tone(buzzer, 1); //send low sound singnal 
        delay(1000);    //one second
        noTone(buzzer); //turn buzzer off
        //digitalWrite(LED_R,LOW);
        
        lcd.clear();  
      }   
    }
    else {
      
      input += key;     //test
      Serial.println(input);    //test input string
            
      if (posX > codeStartX){
        lcd.setCursor(posX-1,1);
        lcd.print("*");
      }
      
      lcd.setCursor(posX,1);     
      lcd.print(key);   
      posX++;
      
    }
 

    
  }


  
  
}

void grantACCESS(){
  
        lcd.clear();
        
        lcd.setCursor(0,0);
        lcd.print("ACCESS GRANTED!!");

        
        posX = codeStartX; //reset position 
        input = ""; //clear input

        digitalWrite(PowerPin, HIGH);
        
        //Turn on green LED
        digitalWrite(LED_G, HIGH); 
        tone(buzzer, 1000); //send 1KHz sound singnal 
        delay(2000);    //one second
        noTone(buzzer); //turn buzzer off
        digitalWrite(LED_G, LOW);

        digitalWrite(PowerPin, LOW);

        
        lcd.clear();   
        ACCESS = 0;  //reset ACCESS
        str = ""; //clear str
}

void receiveEvent(int howMany) {
  //str = "";
  lcd.clear();
  while (0 < Wire.available()) { // loop through all but the last
   
    char c = Wire.read(); // receive byte as a character
    str += c;  
  }
  lcd.clear();

}
