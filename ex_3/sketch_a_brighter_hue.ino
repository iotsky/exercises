/*
  Copyright 2016-2020 iotSky. All Rights Reserved.
  
  A Brighter Hue !
  Alter the intensity of a Philips Hue LED via a potentiometer attached to a Arduino

  Exercise 3: A Brighter Hue
  https://iotsky.io/#/catalog/3

 */
#include <aws_iot_mqtt.h>
#include "aws_iot_config.h"

aws_iot_mqtt_client myClient;
int POTENTIOMETER_PIN = 0;
int old_voltage_value = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  int rc;

  // Choose a baud rate of 115200
  Serial.begin(115200);
  while(!Serial);
  
  // initialize analog pin 0 as an input. (Actually this is redundant as pin 0 is 'analog in')
  pinMode(POTENTIOMETER_PIN, INPUT);

  while(1) {
    rc = myClient.setup(AWS_IOT_CLIENT_ID);
    if (rc != 0) {
      Serial.println("Setup client failed, retrying...");
      Serial.println(rc);
      delay(100);
    } else {
      Serial.println("Setup complete !");
      break;
    }
  }

  while(1) {
    rc = myClient.config(AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT, AWS_IOT_ROOT_CA_FILENAME, AWS_IOT_PRIVATE_KEY_FILENAME, AWS_IOT_CERTIFICATE_FILENAME);
    if (rc != 0) {
      Serial.println("Config client failed, retrying...");
      Serial.println(rc);
      delay(100);
    } else {
      Serial.println("Config complete !");
      break;
    }
  }

  while(1) {
    rc = myClient.connect();
    if (rc != 0) {
      Serial.println("Connect client failed, retrying...");
      Serial.println(rc);
      delay(100);
    } else {
      Serial.println("Connect complete !");
      break;
    }
  
    delay(1000);
 }
    
}

// the loop function runs over and over again forever
void loop() {
  int rc; 
  
  int new_voltage_value = analogRead(POTENTIOMETER_PIN);
  // round down to closest multiple of 10
  new_voltage_value = new_voltage_value - (new_voltage_value % 10);

  if (old_voltage_value != new_voltage_value) {
    Serial.println("Voltage changed...");
    Serial.println(new_voltage_value);
    old_voltage_value = new_voltage_value;
    
    while(1) {
      String val = String(new_voltage_value);
      if((rc = myClient.publish(AWS_IOT_TOPIC, val.c_str(), val.length(), 1, false)) != 0) {
        Serial.println(F("Publish failed, retrying..."));
        Serial.println(rc);
        delay(100);
      } else {
        Serial.println("Publish voltage success !");
        break;
      }
    }
  } 

  rc = myClient.yield();
  if (rc != 0) {
    Serial.println("Yield failed...");
    Serial.println(rc);
  }
  
  delay(200);
}

