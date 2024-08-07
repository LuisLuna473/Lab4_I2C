/*
 * Slave1_I2C.c
 *
 * Created: 1/08/2024 17:46:40
 * Author : luisa
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "I2C/I2C.h"
#include "ADC/ADC.h"

#define SlaveAddress 0x30

uint8_t buffer = 0;
volatile uint8_t POT1 = 0; 



int main(void)
{
   cli();
   DDRD |= (1<<DDD2);
   PORTD &= ~(1<<PORTD2);
   
   I2C_Slave_Init(SlaveAddress);
   SetupADC(PC6);
   sei();
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);
		_delay_ms(1000);
		/*if(buffer == 'R'){
			_delay_ms(1000);
			PORTD |= (1<<PORTD2);
			
			//buffer = 0;
			//buffer = POT1; 
			 
			_delay_ms(1000);
			PORTD &= ~(1<<PORTD2);
			 
		}*/
		
		
			
    }
}

ISR(TWI_vect){
	uint8_t estado = TWSR & 0xF8;
	switch(estado){
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
			PORTD |= (1<<PORTD2);
			TWDR = POT1;  // Enviar valor de ADC
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

ISR(ADC_vect){
	POT1 = ADCH;
	ADCSRA |= (1<<ADIF); 
}