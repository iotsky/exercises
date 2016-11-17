import json
import random
import ssl
from time import sleep
import paho.mqtt.client as mqtt


__doc__ = """
    This is a sample script to publish messages to an MQTT broker at AWS.
    You have to register first at iotsky.io and create a new project.

    Then start the subscriber on one terminal and publish from another.

    For more details visit iotsky.io

    Ensure that you have the paho mqtt client python lib installed. You can find it here:
     https://pypi.python.org/pypi/paho-mqtt/1.1
"""

conn_flag = False

# NOTE: Fill appropriate values here based on your project
AWS_CERT_PATH = '/tmp/rootaws_cert.crt'
IOTSKY_PROJECT_CERT_FILE = '/tmp/light_them_up_cert.pem'
IOTSKY_PROJECT_PRIVATE_KEY_FILE = '/tmp/light_them_up_private_key.pem'
# This will be displayed on the project page e.g.
IOTSKY_PROJECT_TOPIC = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-light_them_up/led'
CLIENT_ID_PUB = 'Blink_LED_IOTSKY_ClientID'


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    global conn_flag
    conn_flag = True
    print("Connected with result code {0}".format(str(rc)))


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print('{0} {1}'.format(msg.topic, str(msg.payload)))


def on_disconnect(client, userdata, rc):
    print 'Disconnected with status: {0}'.format(rc)


def create_client():
    client = mqtt.Client(client_id=CLIENT_ID_PUB)
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect

    client.tls_set(AWS_CERT_PATH, certfile=IOTSKY_PROJECT_CERT_FILE,
                   keyfile=IOTSKY_PROJECT_PRIVATE_KEY_FILE, tls_version=ssl.PROTOCOL_SSLv23,
                   ciphers='AES256-SHA256', cert_reqs=ssl.CERT_REQUIRED)
    client.connect("A1QA5YMMGWQ50B.iot.us-west-2.amazonaws.com", 8883, 60)
    return client


def main():
    global conn_flag
    client = create_client()
    client.loop_start()
    count = 0
    while True:
        sleep(0.5)
        if conn_flag:
            # NOTE: This should always be json data
            client.publish(IOTSKY_PROJECT_TOPIC, "blink", qos=1)
            print("msg sent: blink ")
            sleep(1)
            count += 1
            # exit after 10 messages so you don't blow up your iotsky rate limits !
            if count > 10:
                break
        else:
            print("waiting for connection...")

if __name__ == '__main__':
    main()

