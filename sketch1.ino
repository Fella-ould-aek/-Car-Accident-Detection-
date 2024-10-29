#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

SoftwareSerial gsmSerial(2, 3); // RX, TX for GSM
SoftwareSerial gpsSerial(10, 11); // RX, TX for GPS
TinyGPSPlus gps;

// VARIABLES FOR GPS MODULE
float longitude = 0;
float latitude = 0;
String speed = "";


// ACCELEROMETER PINS
#define x A1
#define y A2
#define z A3

// VARIABLES FOR THE ACCELEROMETER
int xsample = 0;
int ysample = 0;
int zsample = 0;


// THRESHOLD FOR THE ACCELEROMETER
#define maxVal 700

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600); // Initialize GPS module
  gsmSerial.begin(9600); // Initialize GSM module

  Serial.println("Successfully initialized");
  Serial.println("GPS started");

  Serial.print("Initial Accelerometer readings: x=");
  Serial.print(xsample);
  Serial.print(", y=");
  Serial.print(ysample);
  Serial.print(", z=");
  Serial.println(zsample);
  delay(1000);
}

void loop() {
  // Check if GPS data is available
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  Serial.println("Checking GPS data...");
  if (gps.location.isValid()) {
    Serial.print("GPS: Latitude: ");
    Serial.println(gps.location.lat(), 6);  // Display latitude with 6 decimal places
    Serial.print("GPS: Longitude: ");
    Serial.println(gps.location.lng(), 6);  // Display longitude with 6 decimal places
    Serial.print("GPS: Speed MPH:");
    Serial.println(gps.speed.mph());
  } else {
    Serial.println("GPS: GPS location not valid");
  }

  int value1 = analogRead(x);
  int value2 = analogRead(y);
  int value3 = analogRead(z);
  Serial.print(value1);
  Serial.print(", y=");
  Serial.print(value2);
  Serial.print(", z=");
  Serial.println(value3);
  delay(1000);

  // Condition for accident detection
  if (value1 > maxVal || value2 > maxVal || value3 > maxVal) {
    // Calling
    Serial.println("GSM: Calling");
    Serial.println("GSM: Connecting to network");
    delay(2000);
    Serial.println("GSM: Should be connected to network by now");
    gsmSerial.print("ATD+21366979299;\r");
    Serial.println("GSM: Dialing");
    delay(10000); // Give it time to connect
    gsmSerial.print("ATH\r"); // And disconnect
    Serial.println("GSM: Disconnect");
    
    // Sending a message
    gsmSerial.print("Accident Detected; ");
    gsmSerial.print("Location; ");
    gsmSerial.print("Latitude - ");
    gsmSerial.print(gps.location.lat(), 6);
    gsmSerial.print(" Longitude - ");
    gsmSerial.print(gps.location.lng(), 6);
    gsmSerial.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
    gsmSerial.print(gps.location.lat(), 6);
    gsmSerial.print("+");
    gsmSerial.println(gps.location.lng(), 6);
    gsmSerial.write(26);

  }

  delay(1000);
}
