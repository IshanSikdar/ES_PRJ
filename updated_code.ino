#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>

// DS18B20 Sensor Setup
const int oneWireBus = 4; // GPIO pin where the DS18B20 is connected
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Joystick and ESC Setup
#define X_AXIS_PIN 33 // Joystick X-axis pin
#define ESC_PIN 16    // ESC control pin

Servo esc;

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);

  // Initialize DS18B20 sensor
  sensors.begin();

  // Attach ESC and calibrate
  esc.attach(ESC_PIN, 1000, 2000);
  Serial.println("Calibrating ESC...");

}

void loop()
{
  // Temperature Reading and Display
  sensors.requestTemperatures(); // Request temperature readings
  float temperatureC = sensors.getTempCByIndex(0);

  esc.write(0);
  Serial.println("Select (1) Automatic  /  (2) Maunal.");
   if (Serial.available() > 0)
    {
    char Method = Serial.read();
    Serial.print("Received: ");
    Serial.println(Method);
   }
  switch (Method)
  {
    case 1 : 
      
    {
      Serial.println("enter the Motor speed from 0-180 .");
      int speed = Serial.read();

      while ( speed <= 180)
      {
        for (int i=0; i<10 ;i++)
        {

        
          if (temperatureC >= 35)
          {
            Serial.println("Warning temperature too high");
            Serial.println("Motor Breakdown ");
            esc.write(0);
          }
          else
          {

            esc.write(speed);
            Serial.println(" Motor speed = ");
            Serial.print(speed);
            Serial.print("   ");
            Serial.println(" Temperature = ");
            Serial.print(temperatureC);
            Serial.print("   ");
          
            Serial.print(temperatureC);
            Serial.print(",");
            Serial.println(speed);

            delay(1000);
                 
          }
        }
        Serial.println("Do you want to continue Y/N  ");
        char Y,N;
        char response = Serial.read();
        if ( response == Y)
        {
          Serial.println("enter the Motor speed from 0-180 .");
          speed = Serial.read();
        
        }
        else if ( response == N)
        {
          Serial.println("exiting the Program.");
          break;
    
        
        }
        else 
        {
          Serial.println("Invalid input .");
          Serial.println("exiting the Program.");
          break;
    
        
        }
        
      }
      while ( speed <0)
      {
        Serial.println(" Invalid input please enter valid speed within the range.");
      }
      while ( speed > 180)
      {
        Serial.println(" Invalid input please enter valid speed within the range.");
      }
    }
    case 2 :
    {
        if (temperatureC >= 35)
        {
          Serial.println("Warning temperature too high");
          Serial.println("Motor Breakdown ");
          esc.write(0);
        }
        else
        {                
          // Joystick-Based Motor Control
          int joystickValue = analogRead(X_AXIS_PIN);       // Read joystick value (0 to 4095)
          joystickValue = constrain(joystickValue, 2200, 4095); // Constrain joystick to valid range

          int motorSpeed = map(joystickValue, 2200, 4095, 0,180); // Corrected range for ESC (0-180)
          esc.write(motorSpeed); // Send mapped value to ESC

          Serial.print("Joystick Value: ");
          Serial.print(joystickValue);
          Serial.print(" | Temperature: ");
          Serial.print(temperatureC);
          Serial.print(" | Motor Speed: ");
          Serial.println(motorSpeed);

          // Print data for Serial Plotter
          Serial.print(temperatureC);
          Serial.print(",");
          Serial.println(motorSpeed);

          delay(50);
        } // Reduced delay for faster response
    }
 }
}

