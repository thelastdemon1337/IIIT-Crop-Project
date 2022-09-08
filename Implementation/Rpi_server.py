import paho.mqtt.client as mqtt
import os
import random
# from pydub import AudioSegment
# from pydub.playback import play
import RPi.GPIO as GPIO
import time
from gpiozero import Buzzer
from time import sleep

buzzer = Buzzer(27)

# LED FUNCTIONALITY

LED_PIN = 17
servoPIN = 21

GPIO.setup(servoPIN, GPIO.OUT)


p = GPIO.PWM(servoPIN, 50) # GPIO 17 for PWM with 50Hz
p.start(2.5) # Initialization


MQTT_ADDRESS = '192.168.137.243'
MQTT_USER = 'testUser'
MQTT_PASSWORD = 'test'
MQTT_TOPIC1 = 'animal/alert_north'
MQTT_TOPIC2 = 'animal/alert_south'

def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC1)
    client.subscribe(MQTT_TOPIC2)


def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    print(msg.topic + ' ' + str(msg.payload))
    
    try:    
        if str(msg.payload):
    #         path = '/home/pi/Desktop/audio'
    #         audios = os.listdir(path)
    #         d = random.choice(audios)
    #         play(AudioSegment.from_mp3(path + '/' + d))
            GPIO.setmode(GPIO.BCM)
            GPIO.setup(LED_PIN, GPIO.OUT)
            
            
                
            if(msg.topic == 'animal/alert_north'):
                print("Aiming Target")
                p.ChangeDutyCycle(7.5)
                i = 0
                while(i < 10):
                    print("Shooting")
                    GPIO.output(LED_PIN,GPIO.HIGH)
                    time.sleep(0.5)
                    buzzer.on()
                    GPIO.output(LED_PIN,GPIO.LOW)
                    time.sleep(0.5)
                    i += 1            
                p.ChangeDutyCycle(2.5)
                

        
    except KeyboardInterrupt:
        GPIO.cleanup()
        p.stop()

def main():
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
