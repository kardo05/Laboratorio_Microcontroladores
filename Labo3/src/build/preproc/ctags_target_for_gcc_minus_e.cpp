# 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino"
# 2 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/incubadora.ino" 2



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
float senalControl;
float var_referencia;

int baudrate = 9600;
int potenciometro = A0;
int switchSerial = A5;
int switchPantalla = A4;

int sclk = 3;
int sdin = 4;
int dc = 5;
int reset = 6;
int sce = 7;

int BLUE = 10;
int RED = 8;
int GREEN = 9;

int analogValue1;
int analogValue2;


void setup()
{
 pid->Kp = 10;
    pid->Ki = 1;
    pid->Kd = 0;

    pinMode(sclk, 0x1);
    pinMode(sdin, 0x1);
    pinMode(dc, 0x1);
    pinMode(reset, 0x1);
    pinMode(sce, 0x1);
    pinMode(BLUE, 0x1);
    pinMode(RED, 0x1);
    pinMode(GREEN, 0x0);

    analogValue1 =0;
    analogValue2 =0;

    PIDControlador_Init(pid);
    pantalla.begin();
    Serial.begin(baudrate);
    setpoint = analogRead(potenciometro)*12/1000 +30;
    senalControl = PIDControlador_Update(pid, setpoint, temperatura_ambiente);
}

void loop()
{
 analogValue1 = analogRead(switchPantalla);
    analogValue2 = analogRead(switchSerial);

    pantalla.clear();

    float TempWatts = senalControl * 20.0 / 255;
    temperatura = simPlanta(TempWatts);
    var_referencia = analogRead(potenciometro)*12/1000 +30;

    if(var_referencia!= setpoint){
        setpoint = var_referencia;
        senalControl = PIDControlador_Update(pid, setpoint, temperatura);
    }



    if(temperatura < 30){
        digitalWrite(BLUE, 0x1);
        digitalWrite(GREEN, 0x0);
        digitalWrite(RED, 0x0);
    }else{
        if(temperatura > 42){
            digitalWrite(RED, 0x1);
            digitalWrite(BLUE, 0x0);
            digitalWrite(GREEN, 0x0);
        }else{
            if(temperatura <=42 && temperatura>=30){
                digitalWrite(GREEN, 0x1);
                digitalWrite(BLUE, 0x0);
                digitalWrite(RED, 0x0);
            }

        }
    }





    if(analogValue1 != 0){
        desplegarDatosPantallaLCD(setpoint,senalControl,temperatura);
    }

    if(analogValue2 != 0){
        enviarDatosUSART(setpoint,senalControl,temperatura);
    }



    delay(10);
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
    static float T = Tamb; // Temperatura en C
    static uint32_t last = 0;
    uint32_t interval = 100; // ms

    if ( millis () - last >= interval) {
        last += interval;
        // 0−transferencia de calor
        T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) * area * h;
    }

    return T;
}


void desplegarDatosPantallaLCD(float temperaturaOperacion, float senalControl, float temperaturaSensada){
    pantalla.setCursor(0, 0);
    pantalla.print("T_o: ");
    pantalla.print(temperaturaOperacion);
    pantalla.print(" *C");
    pantalla.setCursor(0,1);
    pantalla.print("Senal: ");
    pantalla.print(senalControl*20/255);
    pantalla.print("*C");
    pantalla.setCursor(0, 2);
    pantalla.print("T_s: ");
    pantalla.print(temperaturaSensada);
    pantalla.print(" *C ");
    pantalla.print(" %\t");
}

void enviarDatosUSART(float temperaturaOperacion, float senalControl, float temperaturaSensada){

    if(Serial.available()<0){


        Serial.print(temperaturaOperacion);
        Serial.print(",");
        Serial.print(senalControl);
        Serial.print(",");
        Serial.println(temperaturaSensada);

    }
}
