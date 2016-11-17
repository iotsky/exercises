/*
  Copyright 2016-2020 iotSky. All Rights Reserved.
  
  Light Them Up !
  Turns on an LED on for one second, then off for one second, continually as long as messages are arriving from iotSky.

 */
#include <aws_iot_mqtt.h>
#include "aws_iot_config.h"

void myCallback(char* src, unsigned int len, Message_status_t flag) {
  if(flag == STATUS_NORMAL) {
    Serial.println("In Callback...");
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  } else {
    Serial.println("Callback error...");
    Serial.println(flag);
  }
}

aws_iot_mqtt_client myClient;
char msg[32]; // read-write buffer

// the setup function runs once when you press reset or power the board
void setup() {
  int rc;

  // Choose a baud rate of 115200
  Serial.begin(115200);
  while(!Serial);
  
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  
  Serial.println("Starting subscription...");
  rc = myClient.setup(AWS_IOT_CLIENT_ID);
  if (rc != 0) {
    Serial.println("Setup client failed");
    Serial.println(rc);
  } else {
    Serial.println("Setup complete !");
  }
  
  rc = myClient.config(AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT, AWS_IOT_ROOT_CA_FILENAME, AWS_IOT_PRIVATE_KEY_FILENAME, AWS_IOT_CERTIFICATE_FILENAME);
  if (rc != 0) {
    Serial.println("Config client failed");
    Serial.println(rc);
  } else {
    Serial.println("Config complete !");
  }
  
  rc = myClient.connect();
  if (rc != 0) {
    Serial.println("Connect client failed");
    Serial.println(rc);
  } else {
    Serial.println("Connect complete !");
  }
  
  rc = myClient.subscribe(AWS_IOT_TOPIC, 1, myCallback);
  if (rc != 0) {
    Serial.println("Subscribe client failed...");
    Serial.println(rc);
  } else {
    Serial.println("Subscribe complete !");
  }
  
  delay(2000);
}

// the loop function runs over and over again forever
void loop() {
  int rc; 
  rc = myClient.yield();
  if (rc != 0) {
    Serial.println("Yield failed...");
    Serial.println(rc);
  }
  delay(200);
}

