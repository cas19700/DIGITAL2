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

ADAFRUIT_IO_KEY = "aio_ZUOM94oubSBzWiIlZWvA4D35PIWr"
ADAFRUIT_IO_USERNAME = "BrayanCastillo"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    ser.close()
    ser.open()
    #Readval pyserial
    dataser0 = str(ser.read(10))    # Leer los valores
    print(dataser0)
    dataser1 = dataser0.split(',')  # Dividirlos para obtener el valor de cada sensor
    d1 = dataser1[1]                # Guardar en arrays e imprimir los valores
    d2 = dataser1[2]
    d3 = dataser1[3]
    print(d1)
    print(d2)
    print(d3)


    #Enviar el valor de Humedad
    digital1_feed = aio.feeds('hum')
    aio.send_data(digital1_feed.key, d1)
    digital_data1 = aio.receive(digital1_feed.key)
    print(f'Humedad: {digital_data1.value}')

    #Enviar el estado del basurero
    digital2_feed = aio.feeds('estado')
    aio.send_data(digital2_feed.key, d2)
    digital_data2 = aio.receive(digital2_feed.key)
    print(f'Estado del Basurero: {digital_data2.value}')
    
    #Enviar el contador del basurero
    digital3_feed = aio.feeds('cb')
    aio.send_data(digital3_feed.key, d3)
    digital_data3 = aio.receive(digital3_feed.key)
    print(f'Contador: {digital_data3.value}')

    #Esperar     
    time.sleep(5)