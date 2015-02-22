const int numReadings = 10;
int readings[numReadings];
int index = 0;
int total = 0;
int average =0;
int outputValue = 0;
// What pin is your Fanatec Clubsport pedals connected to the Arduino via
int sensorPin = A5;
// What lenght of time do you want information on the screen for - currently 9.5 seconds.
int interval = 9500;
unsigned long previousMillis=0; 

// Array of pins for the columns
int cPins[] = {A0, A1, A2, A3, A4};
// Number of pins in the column array
int cPinsNo = 5;

// Array of pins for the rows
int rPins[] = {9, 8, 7, 6};

// Number of pins in the row array
int rPinsNo = 4;

// Array for the last known switch states [cPinsNo][rPinsNo]
int colPrev[5][4] = {0};

// One Wire code - This is allowing 1 x DS18B20 probe to be connected to Pin 13 for the purpose of measuring a drivers body temp - The probe was fitted into the back of my Obutto Revolution.
//#include <OneWire.h>
//#include <DallasTemperature.h>
//#define ONE_WIRE_BUS 13
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);
// One Wire
// Required array information - edit seperatly.
#include "buttoncodes.h"
#include "buttonname.h"

// LCD activation code, standard code from any LCD sketch just make sure pin outs match.
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  lcd.begin(20, 4);
  //lcd.print("Driver Temp:");
  //lcd.setCursor(0, 1);
  lcd.print("Brake Booster:");
  // Sensor One Wire
  //sensors.begin();
  //lcd.setCursor(19, 0);
  //lcd.print("c");
  
  Serial.begin(9600);
  //C++ Code Test
  oldbuttons = 0;
	page = 0;
        changed = false;
        blinkrpm = false;
  //CPP Code End     

  for(int cPin = 0; cPin < cPinsNo; cPin++)
  {
    pinMode(cPins[cPin], OUTPUT);
    digitalWrite(cPins[cPin], HIGH);
  }
  //Set the Row Pin Mode
  Serial.println("Setting Row Pins...");

  for(int rPin = 0; rPin < rPinsNo; rPin++)
  {
    pinMode(rPins[rPin], INPUT);
    digitalWrite(rPins[rPin], HIGH);
  }
  Serial.println("Ready!");
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;    
} 
void loop()
{
 //CPP Test

  //sensors.requestTemperatures(); 
 //lcd.setCursor(14, 0);
 //lcd.print(sensors.getTempCByIndex(0));
  // blink without delay attempt
        unsigned long currentMillis = millis();

       // How much time has passed, accounting for rollover with subtraction!
       if ((unsigned long)(currentMillis - previousMillis) >= interval) {
       lcd.setCursor(0, 2);
       lcd.print("                    ");
       //sensors.requestTemperatures(); 
       previousMillis = currentMillis;
       }  
  // read the sensor value for A5 aka Clubsport Input
  outputValue = map(average, 0, 695, 0, 21);  
  // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(sensorPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         

  delay(50);        // delay in between reads for stability 
  lcd.setCursor (14, 1);
  lcd.print("    ");
  lcd.setCursor (14, 0);
  //lcd.print(outputValue /2 -50);
    lcd.print(outputValue);
  lcd.print("   ");
  //lcd.setCursor (17, 0);
  //lcd.print("%");
 
  // Loop through the columns
  for(int cPin = 0; cPin < cPinsNo; cPin++)
  {
    digitalWrite(cPins[cPin], LOW);

    // Loop through the rows
    for(int rPin = 0; rPin < rPinsNo; rPin++)
    {
      //Check if each switch is pressed
      if(digitalRead(rPins[rPin]) == LOW)
      {
        // Check to see if the state has changed since last time
        if(colPrev[cPin][rPin] == 0)
        {
          // Do action here, switch is on
          Serial.println("================================================================");
          Serial.print("Analog Pin # (Note:19 = A4, 20 = A3, 21 = A2, 22 = A1 & 23 = A0)");
          Serial.println();
          Serial.print(cPins[cPin]);
          Serial.println();
          Serial.print("Digital Pin # (Note: 9 = 9, 8 = 8, 7 = 7 and 6 = 6");
          Serial.println();
          Serial.print(rPins[rPin]);
          Keyboard.press(buttonCodes[cPin][rPin]);
          delay(50);
          Keyboard.release(buttonCodes[cPin][rPin]); 
          lcd.setCursor(0, 2);
          lcd.print("                    ");
          lcd.setCursor(0, 2);
          lcd.print(buttonName[cPin][rPin]);
          Serial.println();
          Serial.println("The LCD printed :");
          Serial.println(" ");
          Serial.println(buttonName[cPin][rPin]);
          Serial.println();
          Serial.print("Clubsport V2 Pedal is currently set to:");
          Serial.print(outputValue);
          Serial.print("%");
          Serial.println();
          Serial.println("================================================================");
         // Update last known state of this switch
          colPrev[cPin][rPin] = 1;
          }
        // Check to see if the state has changed since last time
        if(colPrev[cPin][rPin] == 1)
        {
          // Update last known state of this switch
          colPrev[cPin][rPin] = 0;
        }
      }
    }
    digitalWrite(cPins[cPin], HIGH);
  }

}


