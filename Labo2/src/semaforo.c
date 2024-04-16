#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

// Definicion de estados
#define estado_A 1
#define estado_B 2
#define estado_C 3
#define estado_D 4

#define VEHICULAR_ROJO PINB3
#define VEHICULAR_VERDE PINB2
#define APEATONAL_ROJO PIND1
#define APEATONAL_VERDE PIND0
#define BPEATONAL_ROJO PIND5
#define BPEATONAL_VERDE PIND4


// Variables globales
int estado;
int contador;
int tiempo_tres_segundos;
int tiempo_diez_segundos;
int contador_tiempo_tres_segundos;
int contador_tiempo_diez_segundos;
int B1_B2;
int estado_siguiente;


// Definicion de funciones
void maquina_estados();

// Configuracion interrupcion del timer por overflow
ISR(TIMER0_OVF_vect) {
    if (contador == 63){
      contador = 0;
      ++contador_tiempo_tres_segundos;
      ++contador_tiempo_diez_segundos;
    } else contador++;

    if (contador_tiempo_diez_segundos >= 10){
      tiempo_diez_segundos = 1;
    }

    if (contador_tiempo_tres_segundos >= 3){
      tiempo_tres_segundos = 1;
    }
}


// Configuracion interrupcion al presionar el boton B1/B2 
ISR(INT0_vect){
    B1_B2 = 1;
}

ISR(INT1_vect){
    B1_B2 = 1;
}

int main(void) {


    // Configuración de puertos y pines
  PORTB = 0b00000000;

  PORTD = 0b00000000;

    // GIMSK: General Interrupt Mask Register
    // PCMSK0: Pin Change Mask Register 0
    // Configuración de interrupciones externas para pines 6 y 7 (puerto D)
    GIMSK |= (1 << INT0); // Habilitacion de interrupciones para PD2 (Pin6)
    GIMSK |= (1 << INT1); // Habilitacion de interrupciones para PD3 (Pin7)

    // MCUCR: Micro Controller Units (MCU) Control Register
    // Bits de control el control de sensibilidad de interrupciones
    MCUCR &= ~(1 << ISC10); // Se inicializan las demas registros de interrupciones de INT0 Y INT1 a cero
    MCUCR &= ~(1 << ISC00); 
    MCUCR |= (1 << ISC11) | (1 << ISC01); Flanco decreciente de INTO y INT1 genera un pedido de interrupcion
    
    
    // Time/Counter Control Register 
    TCCR0A = 0;// Registro TCCR2A inicializado en 0
    TCCR0B = 0;// Registro TCCR2A inicializado en 0
    TCCR0B |= (1 << CS02) | (1 << CS00); //Clock B con escalamiento: clk_I/O / 1024 del prescaler
    
    // Habilitar interrupciones globales
    sei(); 
    
    TCNT0  = 0;// Contador inicializado en 0


    // Timer Mask Register
    TIMSK |= (1 << TOIE0); // Timer/Counter0 Overflow habilitado

    DDRB |= (1 << VEHICULAR_ROJO) | (1 << VEHICULAR_VERDE); 
    DDRD |= (1 << APEATONAL_ROJO) | (1 << APEATONAL_VERDE) | (1 << BPEATONAL_ROJO) | (1 << BPEATONAL_VERDE);



    PORTB &= ~(1 << VEHICULAR_ROJO) & ~(1 << VEHICULAR_VERDE);
    PORTD &= ~(1 << APEATONAL_ROJO) & ~(1 << APEATONAL_VERDE) & ~(1 << BPEATONAL_ROJO) & ~(1 << BPEATONAL_VERDE);


    estado = estado_A;

    B1_B2 = 0;

    // Loop infinito para la ejecucion de la maquina de estados
    while (1) {
        maquina_estados();
    }

    return 0;

// 

}


void maquina_estados(){
  
  
  while (1) {
        switch (estado)
        {
          case estado_A:
            // Luz vehicular en verde y peatonales en rojo
            PORTD &= ~(1 << APEATONAL_VERDE) | ~(1 << BPEATONAL_VERDE); // luces verdes de los semaforos A y B peatonales se apagan
            PORTD |= (1 << APEATONAL_ROJO) | (1 << BPEATONAL_ROJO); // luces rojas de los semaforos A y B peatonales se encienden
            PORTB &= ~(1 << VEHICULAR_ROJO); // pin conectado a luz roja del semaforo vehicular se apaga
            PORTB |= (1 << VEHICULAR_VERDE); // pin conectado a luz verde del semaforo vehicular se enciende
            if(B1_B2 == 1){
              contador = 0;
              contador_tiempo_tres_segundos = 0;
              tiempo_tres_segundos = 0;
              estado = estado_B;
            }
            break;

          case estado_B:
            // Parpadeo de la luz verde del semaforo vehicular
            PORTB ^= (1 << VEHICULAR_VERDE);
            if(tiempo_tres_segundos == 1){
              PORTB &= ~(1 << VEHICULAR_VERDE);
              PORTB |= (1 << VEHICULAR_ROJO);
            if(contador_tiempo_tres_segundos == 4){
              contador = 0;
              contador_tiempo_diez_segundos = 0;
              tiempo_diez_segundos = 0;
              estado = estado_C;
            }
            }
          break;

          case estado_C:
            // Luz de semaforo vehicular a rojo y semaforos peatonales a verde
            PORTB &= ~(1 << VEHICULAR_ROJO) & ~(1 << VEHICULAR_VERDE);
            PORTD &= ~(1 << APEATONAL_ROJO) & ~(1 << APEATONAL_VERDE) & ~(1 << BPEATONAL_ROJO) & ~(1 << BPEATONAL_VERDE);
            PORTB |= (1 << VEHICULAR_ROJO);
            PORTB &= ~(1 << VEHICULAR_VERDE);
            PORTD &= ~(1 << APEATONAL_ROJO) & ~(1 << BPEATONAL_ROJO);
            PORTD |= (1 << APEATONAL_VERDE) | (1 << BPEATONAL_VERDE);
            if(tiempo_diez_segundos == 1){
              contador = 0;
              contador_tiempo_tres_segundos = 0;
              tiempo_tres_segundos = 0;
              estado = estado_D;
              B1_B2 = 0;
              
            }
          break;

          case estado_D:
            // Parpadeo de semaforos peatonales
            PORTD ^= (1 << APEATONAL_VERDE);
            PORTD ^= (1 << BPEATONAL_VERDE);

            if(tiempo_diez_segundos == 1){
              PORTD |= (1 << APEATONAL_VERDE);
              PORTD |= (1 << BPEATONAL_VERDE);
              PORTD &= ~(1 << APEATONAL_ROJO);
              PORTD &= ~(1 << BPEATONAL_ROJO);
            if(contador_tiempo_tres_segundos == 4){
              PORTB &= ~(1 << VEHICULAR_ROJO) & ~(1 << VEHICULAR_VERDE);
              PORTD &= ~(1 << APEATONAL_ROJO) & ~(1 << APEATONAL_VERDE) & ~(1 << BPEATONAL_ROJO) & ~(1 << BPEATONAL_VERDE);
              estado = estado_A;
            }
            }
          break;

          default:
            estado = estado_A;
            break;
        }
      }
}

