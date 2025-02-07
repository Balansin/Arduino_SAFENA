/*
Codigo NEO-6M
*/

#include <SoftwareSerial.h>  // Library for serial communication via software
#include <TinyGPS++.h>       // Library for parsing GPS data

// Define pins for GPS module connection
#define RXPin 3  // GPS TX -> Arduino RX
#define TXPin 4  // GPS RX -> Arduino TX
#define GPSBaud 9600  // Baud rate for GPS communication

// Create a software serial connection for the GPS module
SoftwareSerial gpsSerial(RXPin, TXPin);

// Create an instance of TinyGPS++ for GPS data processing
TinyGPSPlus gps;

void setup() {
    Serial.begin(115200); // Initialize serial communication with the computer
    gpsSerial.begin(GPSBaud); // Initialize serial communication with GPS module
    Serial.println("Waiting for GPS data...");
}

void loop() {
    // Read data from GPS module when available
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read()); // Parse received GPS data
        
        // If a new location update is available, display latitude and longitude
        if (gps.location.isUpdated()) {
            Serial.print("Latitude: ");
            Serial.print(gps.location.lat(), 6);
            Serial.print(" | Longitude: ");
            Serial.println(gps.location.lng(), 6);
        }

        // If a new date update is available, display the date
        if (gps.date.isUpdated()) {
            Serial.print("Date: ");
            Serial.print(gps.date.day());
            Serial.print("/");
            Serial.print(gps.date.month());
            Serial.print("/");
            Serial.println(gps.date.year());
        }

        // If a new time update is available, display the time in UTC
        if (gps.time.isUpdated()) {
            Serial.print("Time (UTC): ");
            Serial.print(gps.time.hour());
            Serial.print(":");
            Serial.print(gps.time.minute());
            Serial.print(":");
            Serial.println(gps.time.second());
        }

        Serial.println("-------------------------"); // Separator for better readability
        delay(1000); // Small delay to avoid excessive serial output
    }
}
