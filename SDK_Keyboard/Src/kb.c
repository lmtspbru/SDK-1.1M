#include "main.h"
#include "pca9538.h"
#include "kb.h"
#include "sdk_uart.h"
#include "usart.h"

#define KBRD_RD_ADDR 0xE3
#define KBRD_WR_ADDR 0xE2
#define ROW1 0x1E
#define ROW2 0x3D
#define ROW3 0x7B
#define ROW4 0xF7

HAL_StatusTypeDef Set_Keyboard( void ) {
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf;

	buf=0x70;
	ret = PCA9538_Write_Register(KBRD_WR_ADDR, CONFIG, &buf);
	if( ret != HAL_OK ) {
		UART_Transmit("Error write config\n");
		goto exit;
	}

	buf = 0;
	ret = PCA9538_Write_Register(KBRD_WR_ADDR, OUTPUT_PORT, &buf);
	if( ret != HAL_OK ) {
		UART_Transmit("Error write output\n");
	}

exit:
	return ret;
}

uint8_t Check_Row( uint8_t  Nrow ) {
	uint8_t Nkey = 0x00;
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf[4]={0,0,0,0};
	uint8_t kbd_in;

	ret = Set_Keyboard();
	if( ret != HAL_OK ) {
		UART_Transmit("Error write config\n");
	}

	buf[0] = Nrow;

	ret = PCA9538_Write_Register(KBRD_WR_ADDR, OUTPUT_PORT, buf);
	if( ret != HAL_OK ) {
		UART_Transmit("Error write output\n");
	}

	buf[0] = 0;
	ret = PCA9538_Read_Inputs(KBRD_RD_ADDR, buf);
	if( ret != HAL_OK ) {
		UART_Transmit("Read error\n");
	}
	kbd_in = buf[0] & 0x70;
	Nkey = kbd_in;
	if( kbd_in != 0x70) {
		if( !(kbd_in & 0x10) ) {
			switch (Nrow) {
				case ROW1:
					Nkey = 0x04;
					break;
				case ROW2:
					Nkey = 0x04;
					break;
				case ROW3:
					Nkey = 0x04;
					break;
				case ROW4:
					Nkey = 0x04;
					break;
			}
		}
		if( !(kbd_in & 0x20) ) {
			switch (Nrow) {
				case ROW1:
					Nkey = 0x02;
					break;
				case ROW2:
					Nkey = 0x02;
					break;
				case ROW3:
					Nkey = 0x02;
					break;
				case ROW4:
					Nkey = 0x02;
					break;
			}
		}
		if( !(kbd_in & 0x40) ) {
			switch (Nrow) {
				case ROW1:
					Nkey = 0x01;
					break;
				case ROW2:
					Nkey = 0x01;
					break;
				case ROW3:
					Nkey = 0x01;
					break;
				case ROW4:
					Nkey = 0x01;
					break;
			}
		}
	}
	else Nkey = 0x00;

	return Nkey;
}
