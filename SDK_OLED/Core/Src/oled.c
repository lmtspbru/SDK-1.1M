#include "oled.h"
#include "i2c.h"

static uint8_t OLED_Buffer[1024];

static OLED_t OLED;


static void oled_WriteCommand(uint8_t command)
{
	HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x00,1,&command,1,10);
}


uint8_t oled_Init(void)
{
	HAL_Delay(100);

	/* Init screen */
		oled_WriteCommand(0xAE); //display off
		oled_WriteCommand(0x20); //Set Memory Addressing Mode
		oled_WriteCommand(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		oled_WriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
		oled_WriteCommand(0xC8); //Set COM Output Scan Direction
		oled_WriteCommand(0x00); //---set low column address
		oled_WriteCommand(0x10); //---set high column address
		oled_WriteCommand(0x40); //--set start line address
		oled_WriteCommand(0x81); //--set contrast control register
		oled_WriteCommand(0xFF);
		oled_WriteCommand(0xA1); //--set segment re-map 0 to 127
		oled_WriteCommand(0xA6); //--set normal display
		oled_WriteCommand(0xA8); //--set multiplex ratio(1 to 64)
		oled_WriteCommand(0x3F); //
		oled_WriteCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		oled_WriteCommand(0xD3); //-set display offset
		oled_WriteCommand(0x00); //-not offset
		oled_WriteCommand(0xD5); //--set display clock divide ratio/oscillator frequency
		oled_WriteCommand(0xF0); //--set divide ratio
		oled_WriteCommand(0xD9); //--set pre-charge period
		oled_WriteCommand(0x22); //
		oled_WriteCommand(0xDA); //--set com pins hardware configuration
		oled_WriteCommand(0x12);
		oled_WriteCommand(0xDB); //--set vcomh
		oled_WriteCommand(0x20); //0x20,0.77xVcc
		oled_WriteCommand(0x8D); //--set DC-DC enable
		oled_WriteCommand(0x14); //
		oled_WriteCommand(0xAF); //--turn on oled panel

	// Clear screen
	oled_Fill(Black);

	// Flush buffer to screen
	oled_UpdateScreen();

	// Set default values for screen object
	OLED.CurrentX = 0;
	OLED.CurrentY = 0;

	OLED.Initialized = 1;

	return 1;
}

//
//  Fill the whole screen with the given color
//
void oled_Fill(OLED_COLOR color)
{
	/* Set memory */
	uint32_t i;

	for(i = 0; i < sizeof(OLED_Buffer); i++)
	{
		OLED_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}


void oled_UpdateScreen(void)
{
	uint8_t i;

	for (i = 0; i < 8; i++) {
		oled_WriteCommand(0xB0 + i);
		oled_WriteCommand(0x00);
		oled_WriteCommand(0x10);

		HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x40,1,&OLED_Buffer[OLED_WIDTH * i],OLED_WIDTH,100);
	}
}

//
//	Draw one pixel in the screenbuffer
//	X => X Coordinate
//	Y => Y Coordinate
//	color => Pixel color
//
void oled_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color)
{
	if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
	{
		// Don't write outside the buffer
		return;
	}

	// Check if pixel should be inverted
	if (OLED.Inverted)
	{
		color = (OLED_COLOR)!color;
	}

	// Draw in the right color
	if (color == White)
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
	}
	else
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
	}
}

//
//  Draw 1 char to the screen buffer
//	ch 		=> char om weg te schrijven
//	Font 	=> Font waarmee we gaan schrijven
//	color 	=> Black or White
//
char oled_WriteChar(char ch, FontDef Font, OLED_COLOR color)
{
	uint32_t i, b, j;

	// Check remaining space on current line
	if (OLED_WIDTH <= (OLED.CurrentX + Font.FontWidth) ||
			OLED_HEIGHT <= (OLED.CurrentY + Font.FontHeight))
	{
		// Not enough space on current line
		return 0;
	}

	// Use the font to write
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000)
			{
				oled_DrawPixel(OLED.CurrentX + j, (OLED.CurrentY + i), (OLED_COLOR) color);
			}
			else
			{
				oled_DrawPixel(OLED.CurrentX + j, (OLED.CurrentY + i), (OLED_COLOR)!color);
			}
		}
	}

	// The current space is now taken
	OLED.CurrentX += Font.FontWidth;

	// Return written char for validation
	return ch;
}

//
//  Write full string to screenbuffer
//
char oled_WriteString(char* str, FontDef Font, OLED_COLOR color)
{
	// Write until null-byte
	while (*str)
	{
		if (oled_WriteChar(*str, Font, color) != *str)
		{
			// Char could not be written
			return *str;
		}

		// Next char
		str++;
	}

	// Everything ok
	return *str;
}

//
//	Position the cursor
//
void oled_SetCursor(uint8_t x, uint8_t y)
{
	OLED.CurrentX = x;
	OLED.CurrentY = y;
}
