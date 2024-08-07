/*
 * I2C.c
 *
 * Created: 1/08/2024 17:51:42
 *  Author: luisa
 */ 
#include "I2C.h"

//Función para inicializar I2C Maestro
void I2C_Master_Init(unsigned SCL_Clock, uint8_t Prescaler){
	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	switch(Prescaler){
		case 1:
		TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		break;
		
		case 4:
		TWSR &= ~(1<<TWPS1);
		TWSR |= (1<<TWPS0);
		break;
		case 16:
		TWSR &= ~(1<<TWPS0);
		TWSR |= (1<<TWPS1);
		break;
		
		case 64:
		TWSR |= (1<<TWPS1)|(1<<TWPS0);
		break;
		
		default:
		TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		Prescaler = 1;
		break;
	}
	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler);
	TWCR |= (1<<TWEN);
}

//Función para inicio de la comunicación I2C Maestro
uint8_t I2C_Master_Start(void){
	uint8_t estado;
	TWCR = (1<< TWINT)|(1<<TWSTA)|(1<<TWEN); //Iniciar condiciones de start
	while(!(TWCR &(1<<TWINT))); //Espera a que termine la flag TWINT
	
	estado = TWSR & 0xF8; //Verifica estado
	if((estado != 0x08) || (estado != 0x10)){
		return 1;
		}else{
		return estado;
	}
}

//Función de parada I2C Maestro
void I2C_Master_Stop(void){
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO); //Inicia el envio secuencia de parada
	while(TWCR & (1<<TWSTO)); //Esperamos a que el bit se limpie
}

//Maestro escribe
uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	TWDR = dato; //Cargar dato
	TWCR = (1<<TWEN)|(1<<TWINT); //inicia el envio
	
	while(!(TWCR & (1<<TWINT))); //Espera al flag TWINT
	estado = TWSR & 0xF8;
	//Verificar si se transmitio una SLA +W con ACK, SLA +R con ACK, o un Dato
	if(estado == 0x18 || estado == 0x28 || estado == 0x40){
		return 1;
		}else{
		return estado;
	}
}

//Función de recepción de los datos enviados por el esclavo al maestro
//Esta función es para leer los datos que estan en el esclavo
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if(ack){
		TWCR |= (1<<TWEA); //Lectura con ACK
		}else{
		TWCR &= ~(1<<TWEA); //Lectura sin ACK
	}

	TWCR |= (1<<TWINT); //Iniciamos la Lectura
	while (!(TWCR & (1<<TWINT))); //Espera al flag TWINT
	estado = TWSR & 0xF8; //Verifica el estado
	//Verifica dato leido con ACK o sin ACK
	if(estado == 0x58 || estado == 0x50){
		*buffer = TWDR;
		return 1;
		}else{
		return estado;
	}
}

//Función para inicializar I2C esclavo
void I2C_Slave_Init(uint8_t address){
	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	
	TWAR = address << 1 & 0b11111110; //Se asigna la dirección que tendra
	//TWAR = (address << 1| 0x01); //Se asigna la dirección que tendra y habilita
	
	//Se habilita la interfaz, ACK automatico, se habilita la ISR
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}