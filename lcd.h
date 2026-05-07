#include<stdio.h>								    
 #define lcd_dp 0xf<<14
 #define lcd_rs 1<<8
 #define lcd_en 1<<9

 void lcd_init(void);
 void lcd_cmd(unsigned char);
 void lcd_data(char);
 void lcd_string(char *);
 void lcd_int(int);
 void lcd_float(float);
 
 void delay_ms(int ms)
 {
 	T0PR=60000-1;
	T0TCR=0X01;
	while(T0TC<ms);
	T0TCR=0X03;
	T0TCR=0X00;
 }

 void lcd_init(void)
 {
  IODIR0 |= lcd_dp | lcd_rs | lcd_en;
  lcd_cmd(0x03); 
  lcd_cmd(0x02); 
  lcd_cmd(0x28); 
  lcd_cmd(0x0C); 
  lcd_cmd(0x01); 
  lcd_cmd(0x06); 
 }

 void lcd_cmd(unsigned char cmd)
 {
  IOCLR0 = lcd_dp | lcd_rs;
  IOSET0 = (cmd >> 4 & 0x0f)<< 14;
  IOSET0 = lcd_en;
   delay_ms(2);
  IOCLR0 = lcd_en;

  IOCLR0 = lcd_dp | lcd_rs;
  IOSET0 = (cmd & 0x0f)<< 14;
  IOSET0 = lcd_en;
  delay_ms(2);
  IOCLR0 = lcd_en;
 }
 void lcd_data(char data)
 {
  IOCLR0 = lcd_dp;
  IOSET0 = lcd_rs;
  IOSET0 = (data >> 4 & 0x0f) << 14;
  IOSET0 = lcd_en; 
  delay_ms(2);
  IOCLR0 = lcd_en;
  IOCLR0 = lcd_dp;
  IOSET0 = lcd_rs;
  IOSET0 = (data & 0x0f) << 14;
  IOSET0 = lcd_en;
   delay_ms(2);
  IOCLR0 = lcd_en;
 }

 void lcd_string(char *s)
 {
  while(*s)
  {
   lcd_data(*s++);
  }
 }

 void lcd_int(int n)
 {
  char a[100];
  sprintf(a,"%d",n);
  lcd_string(a);
 }

 void lcd_float(float f)
 {
 	char buff[15];
	sprintf(buff,"%.2f",f);
	lcd_string(buff);
 }






