#include <Arduino.h>
#line 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
#include <PCD8544.h>

#include "simPlanta.h"

struct pid{

    float Kp;
    float Ki;
    float Kd;


    float posicion_actual;
    float error;
    float integral;
    float derivada;
    float var_control;
    float ultimo_error;


};


void PIDControlador_Init(PIDControlador *pid);
float PIDControlador_Update(PIDControlador *pid, float setpoint, float posicion_actual);


PCD8544 pantalla;
PIDControlador controlador;
PIDControlador *pid = &controlador;

pid.Kp = 1;
pid.Ki = 1;
pid.Kd = 1;

float setpoint;
float temperatura;
float temperatura_ambiente = 25;

int baudrate = 9600;
int potenciometro = A0;


#line 43 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
void setup();
#line 52 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
void loop();
#line 74 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
float PIDControlador_Update(PIDControlador *pid, float set_point, float medicion);
#line 43 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
void setup()
{
	PIDControlador_Init(pid);
    pantalla.begin();
    Serial.begin(baudrate);
    setpoint = analogRead(potenciometro)*12/1000 +30;
    temperatura = PIDControlador_Update(pid, setpoint, temperatura_ambiente);
}

void loop()
{
	pantalla.clear();
    float TempWatts = temperatura * 20.0 / 255;
    temperatura = simPlanta(TempWatts);
    setpoint = analogRead(potenciometro)*12/1000 +30;
    temperatura = PIDControlador_Update(pid, setpoint, temperatura);
    

}
  
void PIDControlador_Init(PIDControlador *pid){

    pid.integral = 0.0;
    pid.ultimo_error = 0.0;
    pid.derivada = 0.0;
    pid.posicion_actual = 0.0;

    pid.var_control = 0.0;

}
  
float PIDControlador_Update(PIDControlador *pid, float set_point, float medicion) {


    //Leer entrada
    pid.posicion_actual = medicion;
    //Calcular el error
    pid.error = set_point - pid.posicion_actual ;
    //Calcular la integral
    pid.integral = pid.integral + pid.error ;
    //Calcular la derivada
    pid.derivada = pid.error - pid.ultimo_error ;
    //Calcular variable de control
    pid.var_control = (pid.Kp * pid.error ) + ( pid.Ki * pid.integral ) + (pid.Kd * pid.derivada ) ;
    //Acotar variable de control
    if( pid.var_control > 255){
        pid.var_control = 255;
    }    
    else{
        if( pid.var_control < 0){
            pid.var_control = 0;
            
        }
    }
    pid.ultimo_error = pid.error; 
    return pid.var_control;    
}

    
    
