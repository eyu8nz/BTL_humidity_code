#include "AHTxx.h"

AHTxx aht10;

// This code initializes the sensor. It is run once.
void setup() {
     Serial.begin(9600);                        // Begin after 9600 ms
     delay(4800);
     Serial.println("Starting up...");          // Print on a new line. For checkpointing purposes
     while (aht10.begin() != true) {            // If the sensor isn't working...
     	   Serial.println("Sensor not running.");   // ...print this...
    	   delay(5000);                             // ...and delay by 5000 ms
     }
     Serial.println("AHT10 running");           // If the sensor is working, print this (for checkpointing purposes)...
     aht10.setCycleMode();                      // ...and set the cycle mode
}

// This is for letting the Python script know what measurements to write to the data file.
int counter = 0;

// This is the main code. It is run repeatedly.
void loop() {

     // This is printed to the serial monitor for the Python script.
     Serial.println(counter);

     // This is for determining how often we take measurements.
     float points_per_minute = 6.0;

     // For collecting an unspecified number of measurements...
     float humidity = aht10.readHumidity(true);          // Define humidity to be the measurement from the sensor
     float temperature = aht10.readTemperature(false);   // Define temperature to be the measurement from the sensor
     Serial.println(humidity);			         // Print the humidity value to the serial monitor     
     Serial.println(temperature);                        // Print the temperature value to the serial monitor
     delay(int(60.0 * (1/points_per_minute)) * 1000);    // Wait a certain number of ms before collecting the next measurements
     counter += 2;		   			 // Add 2 to the counter because two different measurements pass for each iteration.

     // For collecting measurements for a set period of time...
     /*
     int seconds = 0;
     int minutes = 10;
     int hours = 0;
     while (counter < ((seconds + (minutes * 60) + (hours * 3600)) * points_per_second)) {
     	   float humidity = aht10.readHumidity(true);            
	   float temperature = aht10.readTemperature(false);
	   Serial.println(humidity);                              
     	   Serial.println(temperature);                       
	   delay((1/points_per_second) * 1000);
	   counter++;                                        
     }
     Serial.println("Done!");
     */
}
