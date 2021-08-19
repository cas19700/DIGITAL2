# Código de ejemplo AdafruitIO
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

ADAFRUIT_IO_KEY = "aio_qUgW69hvKiG9y9tbjsKSDN3CZ3uz"
ADAFRUIT_IO_USERNAME = "BrayanCastillo"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    ser.close()
    ser.open()
    #Readval pyserial
    dataser0 = str(ser.read(9))
    print(dataser0)
    dataser1 = dataser0.split(',')
    d1 = dataser1[1]
    print(d1)


    #Digital1 Feed
    digital1_feed = aio.feeds('contador')
    aio.send_data(digital1_feed.key, d1)
    digital_data1 = aio.receive(digital1_feed.key)
    print(f'contador: {digital_data1.value}')


    #Valor a recibir
    digital3_feed = aio.feeds('val')
    digital_data3 = aio.receive(digital3_feed.key)
    print(f'valor de barra: {digital_data3.value}')
    d3 = int(digital_data3.value)
    dat3 = d3.to_bytes(3, 'big')
    ser.write(dat3)
    time.sleep(5)
