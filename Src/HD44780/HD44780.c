/*
 * HD44780.c
 *
 *  Created on: 5 maj 2020
 *      Author: kowma
 */
#include "main.h"

//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wyświetacza (bez rozróżnienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void LCD_Data_Bus_Input(void){
	GPIOB->CRL = 0x88888888;
}
void LCD_Data_Bus_Output(void){
	GPIOB->CRL = 0x77777777;
}


void _LCD_Write(uint8_t data){
	LCD_Data_Bus_Output();

	LCD_RW_GPIO_Port->ODR &= ~LCD_RW_Pin;
	LCD_E_GPIO_Port->ODR |= LCD_E_Pin;
	LCD_D0_GPIO_Port->ODR &= 0xff00;
	LCD_D0_GPIO_Port->ODR |= data;
	simple_delay_us(4);
	LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja odczytu bajtu z wyświetacza (bez rozróżnienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------

uint8_t _LCD_Read(void){
	uint8_t tmp = 0;

	LCD_Data_Bus_Input();

	LCD_RW_GPIO_Port->ODR |= LCD_RW_Pin;
	LCD_E_GPIO_Port->ODR |= LCD_E_Pin;
	simple_delay_us(10);
	tmp = (uint8_t) (LCD_D0_GPIO_Port->IDR & 0x00ff);
	simple_delay_us(2);
	LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
	return tmp;
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyświetlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteCommand(uint8_t command){
	LCD_RS_GPIO_Port->ODR &= ~LCD_RS_Pin;
	_LCD_Write(command);
	simple_delay_us(2);
	while(LCD_ReadStatus() & 0x80);
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja odczytu bajtu statusowego
//
//-------------------------------------------------------------------------------------------------
uint8_t LCD_ReadStatus(void){
	LCD_RS_GPIO_Port->ODR &= ~LCD_RS_Pin;
	simple_delay_us(2);
	return _LCD_Read();
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamięci wyświetlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteData(uint8_t data){
	LCD_RS_GPIO_Port->ODR |= LCD_RS_Pin;
	_LCD_Write(data);
	//simple_delay_ms(10);
	while(LCD_ReadStatus() & 0x80);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja odczytu danych z pamięci wyświetlacza
//
//-------------------------------------------------------------------------------------------------
uint8_t LCD_ReadData(void){
	LCD_RS_GPIO_Port->ODR |= LCD_RS_Pin;
	return _LCD_Read();
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyświetlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteText(char * text){
	while(*text)	LCD_WriteData(*text++);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia współrzędnych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_GoTo(uint8_t x, uint8_t y){
	LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wyświetlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Clear(void){
	LCD_WriteCommand(HD44780_CLEAR);
	simple_delay_ms(6);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywrócenia początkowych współrzędnych wyświetlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Home(void){
	LCD_WriteCommand(HD44780_HOME);
	simple_delay_ms(6);
}
//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void LCD_Initalize(void)
{


	LCD_Data_Bus_Output();
	simple_delay_ms(15);
	LCD_RS_GPIO_Port->ODR &= ~LCD_RS_Pin;
	LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
	LCD_RW_GPIO_Port->ODR  &= ~LCD_RW_Pin;

	for(uint8_t i = 0; i < 3; i++){
		LCD_E_GPIO_Port->ODR |= LCD_E_Pin;
		LCD_D0_GPIO_Port->ODR &= 0xff00;
		LCD_D0_GPIO_Port->ODR |=  0x003F;
		LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
		simple_delay_ms(5);
	  }

	LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_8_BIT);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_CLEAR);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK);

	LCD_WriteText("HC_2020");
}


//void LCD_Display_temperature(uint16_t temperature, uint8_t config)
//{
//	uint8_t start_convertion =0, char_counter =0;
//
//	const char zero = 48; // 0
//	float multiplication_factor, temperature_value;
//	multiplication_factor = 0.5 / (((config & 0x60)>>5)+1);
//
//	temperature_value = (uint16_t)(100*temperature * multiplication_factor);
//
////	for(uint8_t digit_counter = 4; digit_counter <=  0; digit_counter--)
////	{
////		if( ((temperature_value / pow(10,digit_counter)) > 0) && (start_convertion == 0) ){
////			start_convertion =1;
////		}
////		if(start_convertion == 1 )
////		{
////			*(ptr++) = zero +  (char) (temperature_value / pow(10,digit_counter));
////			temperature_value = ((uint16_t)temperature_value % (uint16_t)pow(10,digit_counter));
////		}
////
////	}
////	str[char_counter] = '\0';
//}

