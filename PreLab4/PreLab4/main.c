/*****************************************************************************
 * Universidad del Valle de Guatemala
 * IE2023: Programación de microcontroladores
 * Autor: Oscar Melchor
 * Proyecto: Sumador de 8 bits
 * Archivo: main.c
 * Hardware : ATMEGA328P
 * Created: 10/04/2024 08:34:02
 *****************************************************************************/ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000 //Frecuencia a 16 MHz

uint8_t Contador = 0; ; //Contador de 8 bits

//****************************************************************************

int main(void){ //Función principal
	
	setup();
	while (1){	//Loop infinito
	
	PORTD = Contador; //Toogle Puerto D
	}	
}
//****************************************************************************
void setup(void){ //Función setup
	
	cli();	//Deshabilita interrupciones
	
	UCSR0B = 0;	//Deshabilita módulo comunicación serial
	
	// Output Puerto D
	DDRD = 0xFF;
	PORTD = 0;
	
	// Inputs PC0 y PC1
	DDRC &= ~(1 << PINC0) | ~(1 << PINC1);
	PORTC |= (1 << PINC0) | (1 << PINC1);
	
	// Configuración de interrupciones
	PCICR |= (1<<PCIE1); // Habilita interrupciones en el puerto C
	PCMSK1 |= ((1<<PCINT8)|(1<<PCINT9)); // Habilita la interrupciones en PC0 y PC1
	
	sei(); // Habilita las interrupciones 
}
 //****************************************************************************
ISR(PCINT1_vect){ // Interrupciones de pines
	
	_delay_ms(40);	//Delay antirebote
	
	if (!(PINC & (1 << PINC0))) { // Botón incremento
		Contador++;
		}
	else if (!(PINC & (1 << PINC1))) { // Botón decremento
		Contador--;
	}
	else if((!(PINC & (1 << PINC0))) && (Contador==255))  { // Overflow
		Contador = 0;
	}
	else if((!(PINC & (1 << PINC1))) && (Contador==255))  { // Underflow
		Contador = 254;
	}
	else { // Mantener valor
		Contador = Contador;
	}
}
//****************************************************************************