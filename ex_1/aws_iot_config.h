//===============================================================
#define AWS_IOT_MQTT_HOST "A1QA5YMMGWQ50B.iot.us-west-2.amazonaws.com"  // your endpoint
#define AWS_IOT_MQTT_PORT 8883                                          // your port, use 443 for MQTT over Websocket
#define AWS_IOT_CLIENT_ID   "Blink_LED_IOTSKY_ClientID_sub"             // your client ID
#define AWS_IOT_MY_THING_NAME "led"                                     // your thing name
#define AWS_IOT_ROOT_CA_FILENAME "/mnt/sda1/arduino/rootaws_cert.crt"   // your root-CA filename
#define AWS_IOT_CERTIFICATE_FILENAME "/mnt/sda1/arduino/ex_1/light_them_up_cert.pem"                 // your certificate filename
#define AWS_IOT_PRIVATE_KEY_FILENAME "/mnt/sda1/arduino/ex_1/light_them_up_private_key.pem"          // your private key filename
#define AWS_IOT_TOPIC "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-light_them_up/led" // your topic (replace this with the topic shown in your iotsky project page)
//===============================================================
