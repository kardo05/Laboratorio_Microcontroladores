#!/usr/bin/env python

import serial
import os; os.system('cls')

import csv




os.system('socat PTY,link=/tmp/ttyS0,raw,echo=0 PTY,link=/tmp/ttyS1,raw,echo=0')

port = "/tmp/ttyS0"
baudrate=9600
timeout = 0.1

arduino = serial.Serial(port, baudrate, timeout)
header = ['Temperatura_operacion', 'Senal_control', 'Temperatura_Sensada']
while True:
    mensaje = arduino.readline()
    mensaje = mensaje.decode('utf-8')
    print(f'Mensaje: {mensaje}\n')
    
    with open('output.csv', 'w', encoding='UTF8', newline='') as f:
        writer = csv.writer(f)

        # write the header
        writer.writerow(header)

        # write the data
        writer.writerow(mensaje,delimiter=",")    
