#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include "stdint.h"
#include "stdbool.h"
#include "i2c.h"

#define LCD_I2C_DEF &hi2c1

#define LCD_I2C_RS 0x01
#define LCD_I2C_RW 0x02
#define LCD_I2C_E  0x04
#define LCD_I2C_BL 0x08

#define LCD_I2C_CLEAR_DISPLAY 0x01

#define LCD_I2C_RETURN_HOME 0x02

#define LCD_I2C_ENTRY_MODE_SET    0x04
#define LCD_I2C_ENTRY_MODE_SET_ID 0x02
#define LCD_I2C_ENTRY_MODE_SET_S  0x01

#define LCD_I2C_DISPLAY_CONTROL   0x08
#define LCD_I2C_DISPLAY_CONTROL_D 0x04
#define LCD_I2C_DISPLAY_CONTROL_C 0x02
#define LCD_I2C_DISPLAY_CONTROL_B 0x01

#define LCD_I2C_CURSOR_DISPLAY_SHIFT    0x10
#define LCD_I2C_CURSOR_DISPLAY_SHIFT_SC 0x08
#define LCD_I2C_CURSOR_DISPLAY_SHIFT_RL 0x04

#define LCD_I2C_FUNCTION_SET    0x20
#define LCD_I2C_FUNCTION_SET_DL 0x10
#define LCD_I2C_FUNCTION_SET_N  0x08
#define LCD_I2C_FUNCTION_SET_F  0x04

#define LCD_I2C_FIRST_LINE_FIRST_CHARACTER  0x80
#define LCD_I2C_SECOND_LINE_FIRST_CHARACTER 0xc0

typedef struct
{
	uint8_t deviceAddr;
	bool backLight;
} LCD;

void lcd_send(LCD *lcd, uint8_t data, uint8_t controlBits);
void lcd_init(LCD *lcd);
void lcd_clear(LCD *lcd);
void lcd_print(LCD *lcd, char *text);
void lcd_print_from_position(LCD *lcd, uint8_t cursorInitialPosition, uint8_t initialRow, char *text);
void lcd_send_string(LCD *lcd, char *text, uint8_t length, uint8_t startPosition);

#endif /* INC_LCD_I2C_H_ */
