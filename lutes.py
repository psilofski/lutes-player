#!/usr/bin/env python  
#coding=utf-8  

from subprocess import call
from gpiozero import LED, Button, DistanceSensor
from signal import pause
from time import sleep
#import thread

wav_path = '/home/pi/wav_samples/'
soundfiles = ['1.wav', '2.wav', '3.wav', '4.wav']
max_distance = 1.5
threshold_distance = 0.2
#
sensor = DistanceSensor( 23, 24, max_distance, threshold_distance=0.2 )
leds = [LED(5),
        LED(6),
        LED(13),
        LED(19),
        LED(26),
        LED(21)]

sleeptime = 0.1

def play( threadname=None ):
        for i,filename in enumerate(soundfiles):
            leds[i].on()
            print 'Turning On led ', i
            #play the test sound
            sndfile = wav_path + filename
            call( ['aplay', sndfile] )
            leds[i].off()
            print 'Turning Off led ', i
            sleep(sleeptime)

            object_distance = sensor.distance
            print object_distance
            #breaking the for loop
            if object_distance > max_distance:
                break;


#contr = Controller()
#sensor.when_in_range = play()
#sensor.when_out_of_range = thread.start_new_thread( play, "Thread-1" )
while True:
    if sensor.distance < max_distance:
        play()
        sleep(sleeptime)

#pause()
