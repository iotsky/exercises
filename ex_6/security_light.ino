/*
  Copyright 2016-2020 iotSky. All Rights Reserved.

  Security Light !
  Turns a Philips Hue LED on/off when motion is detected.

  Exercise 6: Security Light
  https://iotsky.io/#/catalog/6

 */
#include <aws_iot_mqtt.h>
#include "aws_iot_config.h"

aws_iot_mqtt_client myClient;
int MOTION_DETECTOR_PIN = 2;

// the setup function runs once when you press reset or power the board
void setup() {
  int rc;

  // Choose a baud rate of 115200
  Serial.begin(115200);
  while(!Serial);

  pinMode(MOTION_DETECTOR_PIN, INPUT_PULLUP);

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

    int proximity = digitalRead(MOTION_DETECTOR_PIN);
    if (proximity == LOW) // If the sensor's output goes low, motion is detected
    {
        Serial.println("Motion detected!");
        while(1) {
          if((rc = myClient.publish(AWS_IOT_TOPIC, "toggle", strlen("toggle"), 1, false)) != 0) {
            Serial.println(F("Publish failed, retrying..."));
            Serial.println(rc);
            delay(100);
          } else {
            Serial.println("Publish success !");
            break;
          }
        }
    }

  int rc;

  rc = myClient.yield();
  if (rc != 0) {
    Serial.println("Yield failed...");
    Serial.println(rc);
  }

  delay(200);
}
