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
int tiempo0;
int tiempo1;
int tiempo2;
int tiempo3;
int B1_B2;
int estado_siguiente;

void me()

int main(void) {
    estado_siguiente = estado_A;
    estado = estado_siguiente;

    // Configuración de puertos y pines
    PORTB = 0b00000000;
    PIND = 0b00000000;
    PORTD = 0b00000000;

    DDRB = 0b01111111;
    DDRD = 0b11110011;

    // Configuración de interrupciones externas para botones
    GIMSK |= (1 << INT0); 
    GIMSK |= (1 << INT1);  
    MCUCR |= (1 << ISC00) | (1 << ISC01);
    MCUCR |= (1 << ISC10) | (1 << ISC11);  
    TCCR0A = 0;
    TCCR0B = (1 << CS02) | (1 << CS00); 
    TIMSK |= (1 << TOIE0); 

    sei(); // Habilitar interrupciones globales

    while (1) {
        me();
    }

    return 0;

// 


}

while (1) {
      switch (estado)
      {
      case A:
        paso_autos();
        if(B1_B2 == 1){
          contador = 0;
          tiempo1 = 0;
          tiempo3 = 0;
          estado = B;
        }
        break;

      case B:
        parpadeo_vehicular();
        _delay_ms(500);
        if(tiempo3 == 1){
          PORTB &= ~(1 << VEHICULAR_VERDE);
          PORTB |= (1 << VEHICULAR_ROJO);
        if(tiempo1 == 4){
          contador = 0;
          tiempo1 = 0;
          tiempo2 = 0;
          estado = C;
        }
        }
      break;

      case C:
        reinicio();
        detener_autos();
        if(tiempo2 == 1){
          reinicio();
          contador = 0;
          tiempo1 = 0;
          tiempo3 = 0;
          state = D;
          B1_B2 = 0;
          
        }
      break;

      case D:
        parpadeo_peatonal();
        _delay_ms(500);
        if(tiempo3 == 1){
          PORTD |= (1 << APEATONAL_VERDE);
          PORTD |= (1 << BPEATONAL_VERDE);
          PORTD &= ~(1 << APEATONAL_ROJO);
          PORTD &= ~(1 << BPEATONAL_ROJO);
        if(tiempo1 == 4){
          reinicio();
          estado = A;
        }
        }
      break;

      default:
        estado = A;
        break;
      }
    }
}
