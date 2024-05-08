#include <Arduino.h>
#line 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
#include <PCD8544.h>

//#include "simPlanta.h"

struct PIDControlador{

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



float setpoint;
float temperatura;
float temperatura_ambiente = 25;

int baudrate = 9600;
int potenciometro = A0;


#line 41 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
void setup();
#line 54 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
void loop();
#line 76 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
float PIDControlador_Update(PIDControlador *pid, float set_point, float medicion);
#line 103 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
float simPlanta(float Q);
#line 41 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
void setup()
{
	pid->Kp = 1;
    pid->Ki = 1;
    pid->Kd = 1;

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

    pid->integral = 0.0;
    pid->ultimo_error = 0.0;
    pid->derivada = 0.0;
    pid->posicion_actual = 0.0;

    pid->var_control = 0.0;

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

float simPlanta(float Q) {
    // simula un bloque de aluminio de 1x1x2cm con un calentador y con enfreamiento pasivo
    // float C = 237; // W/mK coeficiente de conductividad termica para el Aluminio
    float h = 5; // W/m2K coeficiente de conveccion termica para el Aluminio
    float Cps = 0.89; // J/gC
    float area = 1e-4; // m2 area por conveccion
    float masa = 10 ; // g
    float Tamb = 25; // Temperatura ambiente en C
    float T = Tamb; // Temperatura en C
    static uint32_t last = 0;
    uint32_t interval = 100; // ms

    if ( millis () - last >= interval) {
        last += interval;
        // 0−transferencia de calor
        T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) * area * h;
    }

    return T;
}
    
