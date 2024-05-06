#!/usr/bin/env python

import serial
import os; os.system('cls')



port = "ttyS0"
baudrate=9600
timeout = 0.1

arduino = serial.Serial(port, baudrate, timeout)

while True:
    mensaje = arduino.readline()
    mensaje = mensaje.decode('utf-8')
    print(f'Mensaje: {mensaje}\n')