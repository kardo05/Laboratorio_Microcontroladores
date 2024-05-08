#include "pid.h"
    
  
void PIDControlador_Init (PIDControlador *pid){

    pid->integral = 0.0;
    pid->ultimo_error = 0.0;
    pid->derivada = 0.0;
    pid->posicion_actual = 0.0f;

    pid->var_control = 0.0f;

}
  
float PIDControlador_Update(PIDControlador *pid, float set_point, float medicion) {


    //Leer entrada
    pid->posicion_actual = medicion;
    //Calcular el error
    pid->error = set_point - pid->posicion_actual ;
    //Calcular la integral
    pid->integral = pid->integral + pid->error ;
    //Calcular la derivada
    pid->derivada = pid->error - pid->ultimo_error ;
    //Calcular variable de control
    pid->var_control = (pid->Kp * pid->error ) + ( pid->Ki * pid->integral ) + (pid->Kd * pid->derivada ) ;
    //Acotar variable de control
    if( pid->var_control > 255){
        pid->var_control = 255;
    }    
    else{
        if( pid->var_control < 0){
            pid->var_control = 0;
            
        }
    }
    pid->ultimo_error = pid->error; 
    return pid->var_control;    
}

    
    