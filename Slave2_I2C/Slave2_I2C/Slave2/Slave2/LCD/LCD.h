/*
 * IncFile1.h
 *
 * Created: 23/07/2024 09:59:34
 *  Author: lisar
 */ 

#define F_CPU 16000000
#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>

#define E (1<<PORTC1)
//Función para inicializar LCD en modo 4 bits
void initLCD4bits(void);

//Función para colocar en el puerto un valor
void LCD_Port(char a);

//Función para enviar un comando
void LCD_CMD(char a);


//Función para enviar un caracter
void LCD_Write_Char(char c);

//Función para enviar una cadena
void LCD_Write_String(char *a);

//Función para desplazar a la derecha
void LCD_Shift_Right(void);

//Función para desplazar a la izquierda
void LCD_Shift_Left(void);

//Función para establecer el cursor
void LCD_Set_Cursor(char c, char f);
#endif /* LCD_H_ */