# 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
# 2 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino" 2

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
<<<<<<< HEAD
int verde = 11;
int azul = 12;
int rojo = 13;
potenciometro = 0;
potenciometro1 = 0;
int pot_pin = A0;
int cambio = 8;
int transistor = 9;
float Kp = 2;
float Ki = 5;
float kd = 1;
=======
PIDControlador *pid = &controlador;



>>>>>>> 7b812936a77da2997442de154e5016aa2894186d
float setpoint;
float temperatura;
float temperatura_ambiente = 25;

int baudrate = 9600;
int potenciometro = A0;


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
<<<<<<< HEAD
    temperatura = PIDControlador_Update(&controlador, setpoint, temperatura);
    float senal = 1;
    potenciometro = analogRead(pot_pin)*12/1000 +30;
    pottenciometro1= (potenciometro-30)/12*255;

     input=temp;
    if (setpoint != potenciometro){
    setpoint=potenciometro;
    }
    senal=output/255*80;
    analogWrite(transistor, output);
    lcd.setCursor(0, 0);
    lcd.print("T_o: ");
    lcd.print(potenciometro);
    lcd.print(" *C");
    lcd.setCursor(0,1);
    lcd.print("Senal: ");
    lcd.print(senal);
    lcd.print("*C");
    lcd.setCursor(0, 2);
    lcd.print("T_s: ");
    lcd.print(temp);
    lcd.print(" *C ");
=======
    setpoint = analogRead(potenciometro)*12/1000 +30;
    temperatura = PIDControlador_Update(pid, setpoint, temperatura);

>>>>>>> 7b812936a77da2997442de154e5016aa2894186d

    if (temp <= 30){
     digitalWrite(azul, 0x1);
    }
    else {
        digitalWrite(azul, 0x0);
    }
    if (temp >= 42){
        digitalWrite(rojo, 0x1);
    }
    else {
        digitalWrite(rojo, 0x0);
    }
    if (30 <= temp <= 42){
        digitalWrite(verde, 0x1);
    }
    else {
        digitalWrite(verde, 0x0);
    }
}
<<<<<<< HEAD
    delay(2000);
=======

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

>>>>>>> 7b812936a77da2997442de154e5016aa2894186d
