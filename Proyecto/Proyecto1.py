# Código Proyecto AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Brayan Castillo

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed
import serial
import time


ser = serial.Serial('COM3', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_HRZL95Dn8EWqjqieZfP7mpKuhJXj"
ADAFRUIT_IO_USERNAME = "BrayanCastillo"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    ser.close()
    ser.open()
    #Readval pyserial
    dataser0 = str(ser.read(6))    # b'1,75,0'
    print(dataser0)
    dataser1 = dataser0.split(',') # [b'1,75,0'] -> dataser1[2] = 0' -> dataser[1] = 75
    d1 = dataser1[1]
    d2 = dataser1[2]
    print(d1)
    print(d2)


    #Digital1 Feed
    digital1_feed = aio.feeds('hum')
    aio.send_data(digital1_feed.key, d1)
    digital_data1 = aio.receive(digital1_feed.key)
    print(f'Humedad: {digital_data1.value}')

    #Digital2 Feed
    digital2_feed = aio.feeds('estado')
    aio.send_data(digital2_feed.key, d2)
    digital_data2 = aio.receive(digital2_feed.key)
    print(f'Estado del Basurero: {digital_data2.value}')

    
    time.sleep(5)