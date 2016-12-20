import sys
import paho.mqtt.client as mqtt
from phue import Bridge
import ssl

__doc__ = """
    This is a sample script to subscribe to messages from an MQTT broker at AWS. It then controls the intensity of a Philips Hue LED.
    Whenever it receives a message, it queries the state of the Philips Hue LED. It then sends a message to the Philips
    Bridge and sets the brightness of the LED.

    You have to register first at iotsky.io and create a new project.

    Then start the subscriber on one terminal and publish from another.

    For more details visit iotsky.io

    Ensure that you have the paho mqtt client python lib and the phue library installed. You can find it here:
     https://pypi.python.org/pypi/paho-mqtt/1.1
     https://github.com/studioimaginaire/phue

   Alternatively you can install via pip:
       pip install paho-mqtt
       pip install phue

"""

connflag = False

###########################################
# NOTE: Fill these with appropriate values
###########################################

# url to the aws cert is located in the README
AWS_CERT_PATH = '/home/iotsky/demo/rootaws_cert.crt'
IOTSKY_PROJECT_CERT_FILE = '/home/iotsky/demo/a_brighter_hue_cert.pem'
IOTSKY_PROJECT_PRIVATE_KEY_FILE = '/home/iotsky/demo/a_brighter_hue_private_key.pem'
# This will be displayed on the project details page on iotsky.io e.g.
# 9020198dd518778d384bebb123456789-my_project/*
IOTSKY_PROJECT_TOPIC = '9020198dd518778d384bebb123456789-a_brighter_hue/hue'
CLIENT_ID_SUB = 'PHILIPS_HUE_iotsky_sub'
PHILIPS_HUE_BRIDGE_IP = '192.168.1.109'
PHILIPS_HUE_BRIDGE_USERNAME = 'ov71111YNYCyx69R23EGf-abcdefghiljlmnopqr'

# Philips Hue LED details
HUE_BRIDGE = Bridge(ip=PHILIPS_HUE_BRIDGE_IP, username=PHILIPS_HUE_BRIDGE_USERNAME)
ACTIVE_LIGHTS = []


def get_hue_leds():
    HUE_BRIDGE.connect()
    hue_data = HUE_BRIDGE.get_api()
    light_keys = hue_data['lights'].keys()
    active_lights = []
    for light_key in light_keys:
        if hue_data['lights'][light_key]['state']['reachable']:
            active_lights.append(int(light_key))
    return active_lights


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    global connflag
    connflag = True
    print('Connected with result code {0}'.format(str(rc)))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(IOTSKY_PROJECT_TOPIC)


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    global ACTIVE_LIGHTS
    print('Message Received: {0}'.format(str(msg.payload)))
    # Hue brightness varies from 1 to 254
    # The potentiometer reading on analog input pin 0, varies from 0 to 1020
    brightness = int((int(msg.payload)/1020.0)*254)
    if brightness <= 0:
        brightness = 1
    if brightness >= 254:
        brightness = 254
    for light in ACTIVE_LIGHTS:
        print('Turning Hue intensity to {0}'.format(brightness))
        HUE_BRIDGE.set_light(light, 'bri', brightness)


def on_disconnect(client, userdata, rc):
    if rc != 0:
        print client, userdata
        print 'Unexpected Disconnection !'


def on_log(client, userdata, level, buf):
    print level, buf


def main():
    global ACTIVE_LIGHTS
    client = mqtt.Client(client_id=CLIENT_ID_SUB)
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect
    client.on_log = on_log

    client.tls_set(AWS_CERT_PATH, certfile=IOTSKY_PROJECT_CERT_FILE,
                   keyfile=IOTSKY_PROJECT_PRIVATE_KEY_FILE, tls_version=ssl.PROTOCOL_TLSv1_2,
                   ciphers='AES256-SHA256', cert_reqs=ssl.CERT_REQUIRED)
    client.connect("A1QA5YMMGWQ50B.iot.us-west-2.amazonaws.com", 8883, 60)

    ACTIVE_LIGHTS = get_hue_leds()
    if not ACTIVE_LIGHTS:
        print 'No Philips Hue LEDS are reachable'
        sys.exit(1)

    # Blocking call that processes network traffic, dispatches callbacks and
    # handles reconnecting.
    # Other loop*() functions are available that give a threaded interface and a
    # manual interface.
    client.loop_forever()


if __name__ == '__main__':
    main()


