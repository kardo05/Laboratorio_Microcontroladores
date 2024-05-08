#line 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/simPlanta.h"
#ifndef SIMPLANTA_H
#define SIMPLANTA_h


/**
* Esta funcion simula la planta/proceso
* @param Q: Entrada de calor en Watts (o J/s). Para convertir la salida en temperatura del control PID(Output) puede utilizar:
* float TempWatts = (int)Output ∗ 20.0 / 255;
* @return T: Temperatura de salida en la planta
*/

float simPlanta(float Q);

#endif