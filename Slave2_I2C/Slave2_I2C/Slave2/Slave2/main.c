//Código Slave 2


#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "I2C/I2C.h"
#include "LCD/LCD.h"

#define SlaveAddress 0x40

volatile uint8_t contador = 0;

void setup(void)
{
	cli(); // Deshabilitar interrupciones globales

	// Configuración de pines de entrada
	DDRB &= ~(1 << PINB0); // Definir PB0 como entrada
	PORTB |= (1 << PINB0); // Habilitar pull-up interno en PB0
	
	DDRB &= ~(1 << PINB1); // Definir PB1 como entrada
	PORTB |= (1 << PINB1); // Habilitar pull-up interno en PB1
	
	// Configuración de pines de salida para LEDs
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5); // Configurar PD2 a PD5 como salidas
	PORTD &= ~((1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5)); // Inicializar LEDs apagados
	
	// Configuración de interrupciones
	PCICR |= (1 << PCIE0); // Habilitar interrupciones de cambio de pin para PCINT0 (PB0) y PCINT1 (PB1)
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1); // Habilitar máscara de interrupciones para PCINT0 y PCINT1
	
	// Inicialización del I2C
	I2C_Slave_Init(SlaveAddress);
	
	sei(); // Habilitar interrupciones globales
}

int main(void)
{
	setup(); // Configuración inicial

	while (1)
	{
		// Limitar el contador a 4 bits (0-15)
		if (contador > 15)
		contador = 0;
		else if (contador < 0)
		contador = 15;

		// Actualizar LEDs según el valor del contador usando switch
		PORTD &= ~((1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5)); // Apagar todos los LEDs
		switch (contador) {
			case 1:
			PORTD |= (1 << PORTD2); // Encender PD2
			break;
			case 2:
			PORTD |= (1 << PORTD3); // Encender PD3
			break;
			case 3:
			PORTD |= (1 << PORTD2) | (1 << PORTD3); // Encender PD2 y PD3
			break;
			case 4:
			PORTD |= (1 << PORTD4); // Encender PD4
			break;
			case 5:
			PORTD |= (1 << PORTD2) | (1 << PORTD4); // Encender PD2 y PD4
			break;
			case 6:
			PORTD |= (1 << PORTD3) | (1 << PORTD4); // Encender PD3 y PD4
			break;
			case 7:
			PORTD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4); // Encender PD2, PD3 y PD4
			break;
			case 8:
			PORTD |= (1 << PORTD5); // Encender PD5
			break;
			case 9:
			PORTD |= (1 << PORTD2) | (1 << PORTD5); // Encender PD2 y PD5
			break;
			case 10:
			PORTD |= (1 << PORTD3) | (1 << PORTD5); // Encender PD3 y PD5
			break;
			case 11:
			PORTD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD5); // Encender PD2, PD3 y PD5
			break;
			case 12:
			PORTD |= (1 << PORTD4) | (1 << PORTD5); // Encender PD4 y PD5
			break;
			case 13:
			PORTD |= (1 << PORTD2) | (1 << PORTD4) | (1 << PORTD5); // Encender PD2, PD4 y PD5
			break;
			case 14:
			PORTD |= (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5); // Encender PD3, PD4 y PD5
			break;
			case 15:
			PORTD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5); // Encender todos los LEDs
			break;
			default:
			// Para cualquier otro valor, no encender LEDs
			break;
		}

		_delay_ms(100); // Pequeño retraso para evitar parpadeo rápido
	}
}

ISR(TWI_vect)
{
	uint8_t estado = TWSR & 0xF8;
	switch (estado) {
		case 0x60:  // SLA+W recibido, ACK enviado
		case 0x70:  // Dirección de transmisión general recibida, ACK enviado
		TWCR |= (1<<TWINT) | (1<<TWEA);  // Preparar para recibir más datos
		break;
		case 0x80:  // Dato recibido, ACK enviado
		case 0x90:  // Dato recibido en modo transmisión general, ACK enviado
		TWCR |= (1<<TWINT) | (1<<TWEA);  // Preparar para recibir más datos
		break;
		case 0xA8:  // SLA+R recibido, ACK enviado
		case 0xB8:  // Dato transmitido, ACK recibido
		TWDR = contador;  // Enviar valor del contador
		TWCR |= (1<<TWINT) | (1<<TWEA);  // Preparar para la siguiente transmisión
		break;
		case 0xC0:  // Dato transmitido, NACK recibido
		case 0xC8:  // Último dato transmitido, ACK recibido
		TWCR |= (1<<TWINT) | (1<<TWEA);  // Preparar para la próxima recepción
		break;
		default:  // Se libera el bus de cualquier error
		TWCR |= (1<<TWINT) | (1<<TWSTO);
	}
}

ISR(PCINT0_vect)
{
	_delay_ms(50); // Pequeño retraso para el debounce de los botones

	if ((PINB & (1 << PINB1)) == 0) // Verificar si se presionó el botón de incremento
	contador++;
	
	if ((PINB & (1 << PINB0)) == 0) // Verificar si se presionó el botón de decremento
	contador--;
}