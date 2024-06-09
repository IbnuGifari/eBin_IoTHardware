/*********
  Rui Santos

  modified by: Ibnu Khairy Algifari

  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-hc-sr04-ultrasonic-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
#include <WiFi.h>
#include <HTTPClient.h>


// Replace with your network credentials
const char* ssid     = "add your WiFi name";
const char* password = "add your WiFi password";

const int trigPin = 12;
const int echoPin = 14;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://ebin-api.onrender.com/trash-stations/:id/capacity";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 

// Uncomment whatever type you're using!

void setup() {
  Serial.begin(115200); // Starts the serial communication
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient https;
    
    // Your Domain name with URL path or IP address with path
    https.begin(serverName);
    
    // Specify content-type header
    // https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  float capacity = 100 - (distanceCm/30*100);

  if(capacity < 0){
    capacity = 0;
  }
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  Serial.print("Capacity (%): ");
  Serial.println(capacity);

  // Prepare your HTTP POST request data
  //  String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
  //                         + "&location=" + sensorLocation + "&value1=" + String(dht.readTemperature())
  //                         + "&value2=" + String(dht.readHumidity()) + "&value3=" + String(dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity(), false)) + "";
  //   Serial.print("httpRequestData: ");
  //   Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    // String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=DHT11&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    // int httpResponseCode = https.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //https.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = https.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    https.addHeader("Content-Type", "application/json");
    int httpResponseCode = https.PUT("{\"capacity\":"+String(capacity)+"}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    https.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(90000);  
}