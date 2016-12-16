/*
  Copyright 2016-2020 iotSky. All Rights Reserved.

  Hue me on !
  Turns a Philips Hue LED on/off when a button, connected to a arduino yun, is pressed.

  Exercise 2: Hue me On
  https://iotsky.io/#/catalog/2

 */
#include <aws_iot_mqtt.h>
#include "aws_iot_config.h"

aws_iot_mqtt_client myClient;
int button_old_state = HIGH; // initial value
int BUTTON_PIN = 2;

// the setup function runs once when you press reset or power the board
void setup() {
  int rc;

  // Choose a baud rate of 115200
  Serial.begin(115200);
  while(!Serial);

  // initialize digital pin 2 as an input.
  pinMode(BUTTON_PIN, INPUT);

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
  if (button_old_state == LOW) {
    Serial.println("LOW");
  } else {
    Serial.println("HIGH");
  }
  
  int button_new_state = digitalRead(BUTTON_PIN);

  if (button_old_state != button_new_state && button_new_state == LOW) {
    Serial.println("Button pressed...");
    button_old_state = button_new_state;
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
  } else if (button_new_state == HIGH) {
    // takes care of the case when a button is pressed and released. We don't want to send a toggle message then.
    button_old_state = HIGH;
  }

  rc = myClient.yield();
  if (rc != 0) {
    Serial.println("Yield failed...");
    Serial.println(rc);
  }

  delay(200);
}
