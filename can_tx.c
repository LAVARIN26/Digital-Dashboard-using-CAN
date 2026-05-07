#include<lpc21xx.h>
#include"can.h"
#include"lcd.h"
#include"spi.h"
void delay_s(unsigned int sec)
{
	T0PR=60000000-1;
	T0TCR=0X01;
	while(T0TC<sec);
	T0TCR=0X03;
	T0TCR=0X00;
}

void pwm_init()
{
	PINSEL1 |= (1<<10);	//p0.21 pwm5
	PWMMR0 = 15000 - 1;    //100
	PWMMR5 = 7500;			 //50
	PWMMCR = 0x02;
	PWMPCR = (1<<13);
	PWMLER = (1<<0) | (1<<5);
	PWMTCR = 0x09;
}

int duty_convertor(float volt)
{
	return (volt *4000);
}

void speed_msg(CAN2_MSG *m1)
{
	m1->id=0x00000150;
	m1->rtr=0;//data frame
	m1->dlc=4;
	m1->byteA=0;
	m1->byteB=0;
}

void battery_msg(CAN2_MSG *m1)
{
	m1->id=0x00000100;
	m1->rtr=0;//data frame
	m1->dlc=4;
	m1->byteA=0xBBBBBBBB;
	m1->byteB=0;
}

int main()
{	
	CAN2_MSG m1,m2;
	float val1,val2;
	int duty,rpm_key=-1,bat_key=-1;
	can2_init();
	spi_init();
	lcd_init();
	pwm_init();
	speed_msg(&m1);
	battery_msg(&m2);
	lcd_cmd(0x80);
	lcd_string("Can tx");
	while(1)
	{
		val1 = MCP3204(0);
		val2 = MCP3204(1);
		lcd_cmd(0xc0);
		lcd_float(val2);
		duty = duty_convertor(val1);
		PWMMR5 = duty;
 		PWMLER = (1<<5);
		if(duty>10000)
		{
			if(rpm_key==-1||rpm_key==1)
			{
				m1.byteA=50;
				can2_tx(m1);
			}
			rpm_key=0;
		}
		else
		{
			if(rpm_key==-1||rpm_key==0)
			{
				m1.byteA=100;
				can2_tx(m1);
			}
			rpm_key=1;
		}
		if(val2>3)
		{
			if(bat_key==-1||bat_key==1||bat_key==2)
			{
				m2.byteA =0x3;
				can2_tx(m2);
			}
			bat_key=0;

		}
		else if(val2>1 &&val2<3)
		{
			if(bat_key==-1||bat_key==0||bat_key==2)
			{
				m2.byteA =0x2;
				can2_tx(m2);
			}
			bat_key=1;
		}
		else
		{
			if(bat_key==-1||bat_key==1||bat_key==0)
			{
				m2.byteA =0x1;
				can2_tx(m2);
			}
			bat_key=2;
		}
		delay_s(1);			 
	}
}

