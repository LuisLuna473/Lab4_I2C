/*
 * I2C.h
 *
 * Created: 1/08/2024 16:16:21
 *  Author: luisa
 */ 


#ifndef I2C_H_
#define I2C_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
//Función para inicializar el maestro I2C
void I2C_Master_Init(unsigned SCL_Clock, uint8_t Prescaler);

//Maestro escribe
uint8_t I2C_Master_Write(uint8_t dato);

//Función para inicializar I2C esclavo
void I2C_Slave_Init(uint8_t address);

//Función de recepción de los datos enviados por el esclavo al maestro
//Esta función es para leer los datos que estan en el esclavo
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack); 

//Función de parada I2C Maestro
void I2C_Master_Stop(void); 

//Función para inicio de la comunicación I2C Maestro
uint8_t I2C_Master_Start(void);

#endif /* I2C_H_ */