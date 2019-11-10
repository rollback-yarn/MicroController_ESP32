#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *SSID = "Hackathon19";
const char *WiFiPassword = "Digit@lhub";

int DEVICE_ID=1;
int  SIDEA = 0;
int  SIDEB = 0;

//LED
//int ledPin = 2;
int ledPinA = 25;
int ledPinB = 26;

void setup() {

 
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(SSID, WiFiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

 // pinMode(ledPin, OUTPUT);
  pinMode(ledPinA, OUTPUT);
  pinMode(ledPinB, OUTPUT);
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
  
    //digitalWrite(ledPin, HIGH);

    HTTPClient http;
 
    http.begin("http://yarnone.eu-central-1.elasticbeanstalk.com/status/d-05"); //Specify the URL with Last parameter as device ID
  //  http.addHeader("Content-Type", "text/plain"); //Specify content-type header
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        if( httpCode  == 200){
              Serial.println(payload);
              const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
              //DynamicJsonBuffer jsonBuffer(bufferSize);
              StaticJsonBuffer<300> jsonBuffer;
              JsonObject& root = jsonBuffer.parseObject(payload);

              if (!root.success()) {   //Check for errors in parsing
                Serial.println("Parsing failed");
                delay(5000);
                return;
             
              }
              
              // Parameters
              int id = root["id"]; // 1
              Serial.println(id);
              SIDEA= root["SIDEA"]; // "ON or OFF"
              SIDEB = root["SIDEB"]; // "ON or OFF"  
              
              Serial.print("SIDEA = ");
              Serial.println(SIDEA);                 
              }
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources

// Blinking LED

    if( SIDEA== 1)
        digitalWrite(ledPinA, HIGH);
    else
        digitalWrite(ledPinA, LOW);
    if( SIDEB== 1)
        digitalWrite(ledPinB, HIGH);
    else
        digitalWrite(ledPinB, LOW);

  }

  delay(500);
 
}
