#include <Arduino.h>
#line 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
#include <PCD8544.h>
#include "pid.h"
#include "simPlanta.h"

PCD8544 pantalla;
PIDControlador controlador;

float Kp;
float Ki;
float kd;

float setpoint;
float temperatura;
float temperatura_ambiente;

int baudrate = 9600;
PCD8544 pantalla;
#line 18 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
double void setup();
#line 29 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
void loop();
#line 18 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
PIDControlador controlador;
double 


void setup()
{
	pantalla.begin()
    Serial.begin(baudrate);
    temperatura = PIDControlador_Update(&controlador, setpoint, temperatura_ambiente);
}

void loop()
{
	pantalla.clear();
    float TempWatts = temperatura * 20.0 / 255;
    temperatura = simPlanta(TempWatts);
    temperatura = PIDControlador_Update(&controlador, setpoint, temperatura);
    

}

