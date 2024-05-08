#line 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/pid.h"
#ifndef PID_CONTROLADOR_H
#define PID_CONTROLADOR_H


typedef struct {

    float Kp;
    float Ki;
    float Kd;


    float posicion_actual;
    float error;
    float integral;
    float derivada;
    float var_control;
    float ultimo_error;


}PIDControlador;

void PIDControlador_Init(PIDControlador *pid);
float PIDControlador_Update(PIDControlador *pid, float setpoint, float posicion_actual);


#endif