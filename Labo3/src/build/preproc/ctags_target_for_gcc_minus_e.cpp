# 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
# 2 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino" 2
# 3 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino" 2
# 4 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino" 2

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
