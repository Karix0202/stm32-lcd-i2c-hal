#include "lcd_i2c.h"
#include "stdio.h"

void lcd_send(LCD *lcd, uint8_t data, uint8_t controlBits)
{
	uint8_t newData[4];

	newData[0] = (data & 0xf0) | LCD_I2C_E | controlBits;
	newData[1] = (data & 0xf0) | controlBits;
	newData[2] = (data << 4) | LCD_I2C_E | controlBits;
	newData[3] = (data << 4) | controlBits;

	for (uint8_t i = 0; i < 4; i++)
		if (lcd->backLight)
			newData[i] |= LCD_I2C_BL;

	HAL_I2C_Master_Transmit(LCD_I2C_DEF, (lcd->deviceAddr << 1), newData, 4, 100);
	HAL_Delay(10);
}

void lcd_init(LCD *lcd)
{
	HAL_Delay(40);
	lcd_send(lcd, LCD_I2C_FUNCTION_SET | LCD_I2C_FUNCTION_SET_DL, 0);
	HAL_Delay(5);
	lcd_send(lcd, LCD_I2C_FUNCTION_SET | LCD_I2C_FUNCTION_SET_DL, 0);
	HAL_Delay(1);
	lcd_send(lcd, LCD_I2C_FUNCTION_SET | LCD_I2C_FUNCTION_SET_DL, 0);

	lcd_send(lcd, LCD_I2C_FUNCTION_SET, 0);
	lcd_send(lcd, LCD_I2C_FUNCTION_SET | LCD_I2C_FUNCTION_SET_N, 0);
	lcd_send(lcd, LCD_I2C_DISPLAY_CONTROL | LCD_I2C_DISPLAY_CONTROL_D, 0);
	lcd_send(lcd, LCD_I2C_ENTRY_MODE_SET | LCD_I2C_ENTRY_MODE_SET_ID, 0);

	lcd_clear(lcd);
}

void lcd_clear(LCD *lcd)
{
	lcd_send(lcd, LCD_I2C_CLEAR_DISPLAY, 0);
}

void lcd_print(LCD *lcd, char *text)
{
	char buffer[33];
	uint8_t length = snprintf(buffer, 33, text);

	lcd_clear(lcd);

	lcd_send(lcd, LCD_I2C_FIRST_LINE_FIRST_CHARACTER, 0);
	lcd_send_string(lcd, buffer, 16, 0);

	if (length <= 16) return;

	lcd_send(lcd, LCD_I2C_SECOND_LINE_FIRST_CHARACTER, 0);
	lcd_send_string(lcd, buffer, 32, 15);
}

void lcd_print_from_position(LCD *lcd, uint8_t cursorPosition, uint8_t row, char *text)
{
	if (row > 2 || row == 0) return;
	if (cursorPosition > 15) return;

	lcd_clear(lcd);

	char buffer[33];
	uint8_t length = snprintf(buffer, 33, text);

	if (row == 2)
	{
		lcd_send(lcd, LCD_I2C_SECOND_LINE_FIRST_CHARACTER | cursorPosition, 0);
		lcd_send_string(lcd, buffer, length, 0);
	}
	else
	{
		lcd_send(lcd, LCD_I2C_FIRST_LINE_FIRST_CHARACTER | cursorPosition, 0);
		lcd_send_string(lcd, buffer, length, 0);

		if (length <= 16 - cursorPosition) return;

		lcd_send(lcd, LCD_I2C_SECOND_LINE_FIRST_CHARACTER, 0);
		lcd_send_string(lcd, buffer, length, 16 - cursorPosition);
	}
}

void lcd_send_string(LCD *lcd, char *text, uint8_t length, uint8_t startPosition)
{
	uint8_t i = startPosition;

	while (text[i] && i < length)
	{
		lcd_send(lcd, text[i], LCD_I2C_RS);
		i++;
	}
}
