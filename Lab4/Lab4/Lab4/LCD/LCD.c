/*
 * LCD.c
 *
 * Created: 23/07/2024 10:01:30
 *  Author: lisar
 */ 
#define F_CPU 16000000
#include "LCD.h"
#include <util/delay.h>

//Función para inicializar LCD en modo 4 bits
void initLCD4bits(void){
	//Definiendo el puerto C como salida
	DDRD |= (1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
	DDRC |= (1<<DDC0)|(1<<DDC1);
	PORTD = 0;
	
	LCD_Port(0x00);
	_delay_ms(20);
	
	LCD_CMD(0x03);
	_delay_ms(5);
	
	LCD_CMD(0x03);
	_delay_ms(11);
	
	LCD_CMD(0x03);
	
	LCD_CMD(0x02);
	////////////////////////////////////////
	// Definiendo el Function set
	LCD_CMD(0x02);
	LCD_CMD(0x08);
	
	//Definiendo el display on-off
	LCD_CMD(0x00);
	LCD_CMD(0x0C);
	
	//Entry Mode
	LCD_CMD(0x00);
	LCD_CMD(0x06);
	
	//Clear display
	LCD_CMD(0x00);
	LCD_CMD(0x01);
	
}


//Función para colocar en el puerto un valor
void LCD_CMD(char a){
	//RS = 0;  //RS=0 Dato en el puerto lo va a interpretar como comando
	PORTC &= ~(1<<PORTC0); 
	LCD_Port(a);
	
	//EN = 1; // E=1
	PORTC |= (1<<PORTC1);
	_delay_ms(4);
	
	//EN = 0; // E=0
	PORTC &= ~E; 
}


//Función para enviar un comando
void LCD_Port(char a) {
	if (a & 1) //Comparo si "a" es igual a 1
	//D4 = 1;
	PORTD |= (1<<PORTD4); //Para poner D4 en 1
	else
	//D4 = 0;
	PORTD &= ~(1<<PORTD4); //Para poner D4 en 0
	
	if (a & 2)
	//D5 = 1;
	PORTD |= (1<<PORTD5);
	else
	//D5 = 0;
	PORTD &= ~(1<<PORTD5);
	
	if (a & 4)
	//D6 = 1;
	PORTD |= (1<<PORTD6);
	else
	//D6 = 0;
	PORTD &= ~(1<<PORTD6);
	
	if (a & 8)
	//D7 = 1;
	PORTD |= (1<<PORTD7);
	else
	//D7 = 0;
	PORTD &= ~(1<<PORTD7); 
}

//Función para enviar un caracter
void LCD_Write_Char(char c){
	char Cbajo, Calto;  //Caracter alto y Caracter bajo
	Cbajo= c & 0x0F;  
	Calto = (c & 0xF0)>>4; //11110000 => >>4 => 00001111    Lo movimos 4 espacios hacia abajo 
	
	//RS = 1;  //RS=1 Dato en el puerto lo va a interpretar como CARACTER (DATO)
	PORTC = (1<<PORTC0);
	LCD_Port(Calto);
	
	//EN = 1; // E=1
	PORTC |= (1<<PORTC1);
	_delay_ms(4);
		
	//EN = 0; // E=0
	PORTC &= ~E;
	
	LCD_Port(Cbajo);
	
	//EN = 1; // E=1
	PORTC |= (1<<PORTC1);
	_delay_ms(4);
	
	//EN = 0; // E=0
	PORTC &= ~E;
}

//Función para enviar una cadena
void LCD_Write_String(char *a){
	int i;
	for (i = 0; a[i] != '\0'; i++) //Vamos a hacer un recorrido de la cadena hasta llegar a un caracter nulo
	LCD_Write_Char(a[i]);
}


//Función para desplazar a la derecha
void LCD_Shift_Right(void){
	LCD_CMD(0x01);
	LCD_CMD(0x0C);
}

//Función para desplazar a la izquierda
void LCD_Shift_Left(void){
	LCD_CMD(0x01);
	LCD_CMD(0x08);
}

//Función para establecer el cursor
void LCD_Set_Cursor(char c, char f){ //f de fila y c de columna
	char temp, z, y;
	if (f == 1){
		temp = 0x80 + c - 1;
		z = temp >> 4;
		y = temp & 0x0F;
		LCD_CMD(z);
		LCD_CMD(y);	
		}else if (f == 2) {
		temp = 0xC0 + c - 1;
		z = temp >>4;
		y = temp & 0x0F;
		LCD_CMD(z);
		LCD_CMD(y);
	
}
}