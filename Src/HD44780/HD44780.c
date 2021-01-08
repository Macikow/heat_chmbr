/*
 * HD44780.c
 *
 *  Created on: 5 maj 2020
 *      Author: kowma
 */
#include "main.h"

#define LCD_COLS 20
#define LCD_ROWS 2

const uint8_t custom_font_pattern[][8] =
{
		{0x1f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f}, // window
		{0x8, 0xc, 0xe, 0xf, 0xe, 0xc, 0x8, 0x0}, // selected arrow
		{0x0, 0x0, 0x7, 0x4, 0x4, 0x1f, 0xe, 0x4}, // down arrow
		{0x0, 0x0, 0x4, 0xa, 0x11, 0x1f, 0x0, 0x0}, // delta
		{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, //1 line
		{0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8}, // 2 line
		{0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2}, // 4 line
		{0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1}  // 5 line
};



uint8_t write_buf_x_pos = 0;
uint8_t write_buf_y_pos = 0;
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wyświetacza (bez rozróżnienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void LCD_Data_Bus_Input(void){
	GPIOB->CRL &= 0x000000ff;
	GPIOB->CRL |= 0x88888800; 	// TODO sprawdzić czy nie będzie problemów z czujnikami
}
void LCD_Data_Bus_Output(void){
	GPIOB->CRL &= 0x000000ff;
	GPIOB->CRL |= 0x77777700;	// TODO sprawdzić czy nie będzie problemów z czujnikami
}

void _LCD_Send_Half(uint8_t half_byte)
{
	half_byte = (half_byte & 0x0f);
	LCD_E_GPIO_Port->ODR |= LCD_E_Pin;
	simple_delay_us(8);

	LCD_D4_GPIO_Port->ODR &= 0xfff0;
	LCD_D4_GPIO_Port->ODR |= half_byte;

	simple_delay_us(8);
	LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
}


void _LCD_Write(uint8_t data){
	LCD_Data_Bus_Output();

	LCD_RW_GPIO_Port->ODR &= ~LCD_RW_Pin;

	_LCD_Send_Half(data >> 4);
	_LCD_Send_Half(data);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja odczytu bajtu z wyświetacza (bez rozróżnienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
uint8_t _LCD_Read_Half()
{
	uint8_t tmp = 0;
	LCD_E_GPIO_Port->ODR |= LCD_E_Pin;
	simple_delay_us(8);
	tmp = (uint8_t) (LCD_D4_GPIO_Port->IDR & 0x000f);
	simple_delay_us(8);
	LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
	return tmp;
}



uint8_t _LCD_Read(void){
	uint8_t tmp = 0;

	LCD_Data_Bus_Input();
	LCD_RW_GPIO_Port->ODR |= LCD_RW_Pin;
	//LCD_E_GPIO_Port->ODR |= LCD_E_Pin;
	simple_delay_us(8);
	tmp |= ((_LCD_Read_Half() << 4) & 0xf0);
	tmp |= (_LCD_Read_Half() & 0x0f);

	simple_delay_us(8);
	//LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
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
	simple_delay_us(8);
	while(LCD_ReadStatus() & 0x80);
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja odczytu bajtu statusowego
//
//-------------------------------------------------------------------------------------------------
uint8_t LCD_ReadStatus(void){
	LCD_RS_GPIO_Port->ODR &= ~LCD_RS_Pin;
	simple_delay_us(8);
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
	simple_delay_us(15);
	//while(LCD_ReadStatus() & 0x80);
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
	simple_delay_us(10);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywrócenia początkowych współrzędnych wyświetlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Home(void){
	LCD_WriteCommand(HD44780_HOME);
	simple_delay_us(10);
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
		LCD_D4_GPIO_Port->ODR &= 0xfff0;
		LCD_D4_GPIO_Port->ODR |=  0x0003;
		LCD_E_GPIO_Port->ODR &= ~LCD_E_Pin;
		simple_delay_ms(5);
	  }

	LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_CLEAR);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
	simple_delay_ms(1);
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK);

	for(uint8_t i=0; i< 8 ;i++)
	{
		LCD_WriteCommand(0x40 + i*8);
		for(uint8_t j =0; j< 8; j++)
		{
			LCD_WriteData(custom_font_pattern[i][j]);
		}
	}
}


void lcd_buf_go_to(uint8_t buf_x, uint8_t buf_y )
{
	write_buf_x_pos = buf_x;
	write_buf_y_pos = buf_y;
}


void lcd_char(char c)
{
	lcd_buf[write_buf_y_pos][write_buf_x_pos++] = c;
}


void lcd_buf_clear(void)
{
	for(uint8_t iy = 0 ; iy <LCD_ROWS ; iy++ ){
		for(uint8_t ix = 0; ix < LCD_COLS; ix ++)
		{
			lcd_buf[iy][ix] = ' ';
		}
	}
}


void lcd_buf_write_text(char * text)
{
	while(*text)
	{
		lcd_buf[write_buf_y_pos][write_buf_x_pos++] = *text++;
	}
}


void lcd_handler()
{
	static uint8_t locate_flag = 0;
	static uint8_t x = 0, y = 0;


	if((LCD_ReadStatus() & 0x80) == 0)
	{
		if(x == LCD_COLS)
		{
				x = 0;
				y++;
				if(y == LCD_ROWS)
				{
					y=0;
					LCD_GoTo(0,y);
					return;
				}
		}
		if( lcd_buf[y][x] != lcd_buf_old[y][x] )
		{
			if(!locate_flag)
			{
				LCD_GoTo(x,y);
				locate_flag = 1;
				return;
			}
			LCD_WriteData(lcd_buf[y][x]);
			lcd_buf_old[y][x] = lcd_buf[y][x];
		}
		else
		{
			locate_flag = 0;
		}
		x++;
	}
}
void lcd_circle_bufer_refresh()
{
	static uint8_t xc = 0, yc = 0;
	LCD_GoTo(xc,yc);
	LCD_WriteData(lcd_buf[yc][xc]);
	if(++xc == 20)
	{
		xc = 0;
		if(++yc == 2)
		{
			 yc = 0;
		}
	}
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
