#include<lpc21xx.h>
#include"can.h"
#include"uart.h"
#include"delay.h"
#include"lcd.h"
#define buz  1<<20

void buzzer(void)
{
	IOSET1=buz;
	delay(400);
	IOCLR1=buz;
}

void lcd_title(void)
{
	lcd_cmd(0x80);
	lcd_string("Digital Dashboard");
	lcd_cmd(0xc0);
	lcd_string("in car using CAN");
	delay(5000);
	lcd_init();
}

void dash_board()
{
	lcd_cmd(0x80);
	lcd_string("Mode:");//80 81 82 83 84
	lcd_cmd(0xc0);
	lcd_string("Bat stat:");// c0 c1 c2 c3 c4 c5 c5 c6 c7
}

int main()
{
	CAN2 m1;
	can2_init();
	uart_config();
	lcd_init();
	cg_write();
	uart_str("Main ECU\r\n");
	lcd_title();

	IODIR1=1<<20;

	dash_board();
	while(1){		
		can2_rx(&m1);
		switch(m1.id)
		{
		/* Speed monitoring*/
			case 0x150:
			switch (m1.byteA)
			{
				case 50:
				lcd_cmd(0x85);
				lcd_data(6);
				buzzer();
				uart_str("Over Speed \r\n");
				break;
				case 100:
				lcd_cmd(0x85);
				lcd_data('E');
				uart_str("Normal Speed \r\n");
				break;
			}

			break;
			/* Batery monitoring */

			case 0x100:
				switch (m1.byteA)
				{
				case 0x1:
					uart_str("Low battery\r\n");
					lcd_cmd(0xc8);
					lcd_data(1);
					buzzer();
					break;
				case 0x02:
					uart_str("Mid battery\r\n");
					lcd_cmd(0xc8);
					lcd_data(2);
					buzzer();
					break;
				case 0x03:
					uart_str("Full battery\r\n");
					lcd_cmd(0xc8);
					lcd_data(3);
					buzzer();
					break;
			}
			break;

			/*Engine temperature */

			case 0x120:
			switch (m1.byteA)
			{
				case 0x20:
				lcd_cmd(0x8e);
				lcd_data(5);
				buzzer();
				uart_str("Engine temp warning \r\n");
				break;
				case 0x40:
				lcd_cmd(0x8e);
				lcd_data(0);
				uart_str("Normal temp \r\n");
				break;
			}
			break;

			case 0x420:
			switch (m1.byteA)
			{
				case 0x01:
				lcd_cmd(0xce);
				lcd_data(4);
				buzzer();
				uart_str("door open \r\n");
				break;
				case 0x02:
				lcd_cmd(0xce);
				lcd_data(0);
				uart_str("door close  \r\n");
				break;
			}
			break;

		}

		
	}
		
}





