#ifndef PID_CONTROLADOR_H
#define PID_CONTROLADOR_H


typedef struct {

    double Kp;
    double Ki;
    double Kd;


    double posicion_actual;
    double error;
    double integral;
    double derivada;
    double var_control;
    double ultimo_error;


}PIDControlador;

void PIDControlador_Init(PIDControlador *pid);
double PIDControlador_Update(PIDControlador *pid, double setpoint, double posicion_actual);


#endif