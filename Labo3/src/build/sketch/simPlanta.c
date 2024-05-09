#line 1 "/home/wilberahr/Documents/Laboratorio_Microcontroladores/Labo3/src/incubadora/simPlanta.c"
#include <stdint.h>
 


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