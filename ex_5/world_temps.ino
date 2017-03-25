/*
  Copyright 2016-2020 iotSky. All Rights Reserved.

  World Temperatures !
  Transmits your current room temperature to iotsky, where it is displayed on a world map.

  Exercise 5: World Temperatures
  https://iotsky.io/#/catalog/5

 */
#include <aws_iot_mqtt.h>
#include "aws_iot_config.h"

aws_iot_mqtt_client myClient;
int button_old_state = HIGH; // initial value
int TEMP_PIN = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  int rc;

  // Choose a baud rate of 115200
  Serial.begin(115200);
  while(!Serial);

  // initialize digital pin 2 as an input.
  pinMode(TEMP_PIN, INPUT);

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
  float voltage = analogRead(TEMP_PIN) * 0.004882814;
  String payload;
  
  Serial.println(voltage);
  float degrees_in_celsius = (voltage - 0.5) * 100.0;
  float degrees_in_fahrenheit = degrees_in_celsius * (9.0/5.0) + 32.0;

  while(1) {
      /* construct the json payload */
      payload = String("{\"latitude\": ") + 
                String(MY_LATITUDE) + 
                String(", \"fahrenheit\": ") + 
                String(degrees_in_fahrenheit) + 
                String(", \"celsius\": ") + 
                String(degrees_in_celsius) + 
                String(", \"app_name\": \"world_temp\", \"longitude\": ") + 
                String(MY_LONGITUDE) + 
                String(", \"name\": \"") + APP_NAME + String("\"")
                String("}");
      
      Serial.println("Sending:");
      Serial.println(payload);
      
      if((rc = myClient.publish(AWS_IOT_TOPIC, payload.c_str(), payload.length(), 1, false)) != 0) {
        Serial.println(F("Publish failed, retrying..."));
        Serial.println(rc);
        delay(300);
      } else {
        Serial.println("Publish success !");
        break;
      }
  }

  rc = myClient.yield();
  if (rc != 0) {
    Serial.println("Yield failed...");
    Serial.println(rc);
  }

  delay(2000);
}

