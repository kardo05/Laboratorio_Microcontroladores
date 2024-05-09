#include <PCD8544.h>
#include "pid.h"
#include "simPlanta.h"

PCD8544 pantalla;
PIDControlador controlador;
int verde      = 11;
int azul       = 12;
int rojo       = 13;
potenciometro  = 0;
potenciometro1 = 0;
int pot_pin    = A0;
int cambio     = 8;
int transistor = 9;
float Kp       = 2;
float Ki       = 5;
float kd       = 1;
float setpoint;
float temperatura;
float temperatura_ambiente;

int baudrate = 9600;
PCD8544 pantalla;
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

    if (temp <= 30){
     digitalWrite(azul, HIGH);
    }
    else {
        digitalWrite(azul, LOW);
    }
    if (temp >= 42){
        digitalWrite(rojo, HIGH);
    }
    else {
        digitalWrite(rojo, LOW);
    }
    if (30 <= temp <= 42){
        digitalWrite(verde, HIGH);
    }
    else {
        digitalWrite(verde, LOW);
    }
}
    delay(2000);