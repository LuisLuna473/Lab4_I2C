//Código MAESTRO modificado para Slave 2

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "I2C/I2C.h"
#include "LCD/LCD.h"

#define slave1 0x30
#define slave2 0x40

uint8_t direccion;
uint8_t temp;
uint8_t bufferI2C;
uint8_t ver; 
uint8_t receivedValue = 0;  // Variable para almacenar el valor recibido del esclavo
uint8_t receivedValue2 = 0;  // Variable para almacenar el valor recibido del esclavo

char lista[10] = {'0','1','2','3','4','5','6','7','8','9'};
char lista1[4], lista2[4], lista3[4] = {'0', '0', '0'};



char CONTlista[10] = {'0','1','2','3','4','5','6','7','8','9'};
	
//Contador tiene que llegar a 15 (Dos dígitos)
char listaCONT1[4], listaCONT2[4] = {'0', '0'};

//void newLista(char *lista, int valor);
void UpdateCONTADOR(char *CONTlista, int valorCONT);

void Slave1(void);
void Slave2(void);

int main(void)
{
    
	I2C_Master_Init(100000,1); //inicializar como Master Fscl 100Khz, preescaler 1
	initLCD4bits();
	LCD_Set_Cursor(1,1); //
	LCD_Write_String("S1:");
	
	
	LCD_Set_Cursor(6,1);
	LCD_Write_String("S2:");
	
//	DDRD |= (1<<DDD2);
	DDRC |= (1<<DDC3);
	 
    while (1) 
    {
		
		//_delay_ms(1000);
		/*PORTD |= (1<<PORTD2);
		
		I2C_Master_Start();
		bufferI2C = slave1 << 1 & 0b11111110; //Mandando Maestro a escribir, si el bit final lo cam
		//bufferI2C &= ~(0<<1); 
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
		}
		
		ver = I2C_Master_Write('R');
		if(ver == 1){
			I2C_Master_Stop();
			continue;
			
		}*/
		
		Slave1();
		
		PORTD &= ~(1<<PORTD2);
		_delay_ms(100);
		//_delay_ms(2000);
		
		newLista(lista1, receivedValue);
		newLista(lista3, 255);
		
		LCD_Set_Cursor(1,2);
		LCD_Write_String(lista1);
		//LCD_Write_String('0');
		
		
		Slave2();
		PORTC &= ~(1<<PORTC3);
		_delay_ms(100);
		
		UpdateCONTADOR(listaCONT1, receivedValue2);
		UpdateCONTADOR(listaCONT2, 15);
		
		LCD_Set_Cursor(6,2);
		LCD_Write_String(listaCONT1);
		
		
		

    }
}

void newLista(char *lista, int valor) {
	lista[0] = '0' + (valor / 100);
	lista[1] = '0' + ((valor / 10) % 10);
	lista[2] = '0' + (valor % 10);
	lista[3] = '\0';
}

void UpdateCONTADOR(char *CONTlista, int valorCONT) {
	CONTlista[0] = '0' + (valorCONT / 10); // Decena
	CONTlista[1] = '0' + (valorCONT % 10); // Unidad
	CONTlista[2] = '\0'; // Fin de cadena
}


void Slave1(void){
	I2C_Master_Start();
	bufferI2C = (slave1 << 1) | 0b00000001;  // Mandando Maestro a leer (SLA + R)
	if (I2C_Master_Write(bufferI2C) != 1) {  // Si no se recibe ACK, detener la comunicación
		I2C_Master_Stop();
	}
	
	if (I2C_Master_Read(&receivedValue, 0) == 1) {  // Leer el valor enviado por el esclavo
		// El valor recibido está en receivedValue
	}
	
	I2C_Master_Stop();
}

void Slave2(void){
	I2C_Master_Start();
	bufferI2C = (slave2 << 1) | 0b00000001;  // Mandando Maestro a leer (SLA + R)
	if (I2C_Master_Write(bufferI2C) != 1) {  // Si no se recibe ACK, detener la comunicación
		I2C_Master_Stop();
	}
	
	if (I2C_Master_Read(&receivedValue2, 0) == 1) {  // Leer el valor enviado por el esclavo
		// El valor recibido está en receivedValue
	}
	
	I2C_Master_Stop();
}