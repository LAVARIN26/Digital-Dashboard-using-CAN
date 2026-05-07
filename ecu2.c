#include<lpc21xx.h>
#include"spi.h"
#include"can.h"
#include"lcd.h"
#define door1 12
#define door2 13

void temp_msg(CAN2_MSG *m1)
{
	m1->id=0x00000120;
	m1->rtr=0;//data frame
	m1->dlc=4;
	m1->byteA=0;
	m1->byteB=0;
}

void door_msg(CAN2_MSG *m1)
{
	m1->id=0x00000420;
	m1->rtr=0;//data frame
	m1->dlc=4;
	m1->byteA=0;
	m1->byteB=0;
}

int main()
{
	float val;
	int temp_key=-1,door_key=-1;
	CAN2_MSG m1,m2;
	can2_init();
	spi_init();
	lcd_init();
	temp_msg(&m1);
	door_msg(&m2);
	lcd_cmd(0x80);
	lcd_string("Temp");
	while(1)
	{
		val = MCP3204(0);
		lcd_cmd(0xc0);
		lcd_float(val*100);
		if(val>2)
		{
			if(temp_key==0||temp_key==-1)
			{
				m1.byteA=0x20;
			 	can2_tx(m1);
			}
			temp_key=1;
			
		}
		else
		{
			if(temp_key==1||temp_key==-1)
			{
				m1.byteA=0x40;
				can2_tx(m1);
			}
			temp_key=0;
		}

		if(((IOPIN0>>door1)&1)==0||((IOPIN0>>door2)&1)==0)
		{
			if(door_key==0||door_key==-1)
			{
				m2.byteA=0x01;
				can2_tx(m2);
			}
			door_key=1;
		}
		else
		{
			if(door_key==1||door_key==-1)
			{
				m2.byteA=0x02;
				can2_tx(m2);
			}
			door_key=0;
			
		}
		delay_ms(500);
	}
}

