#include<stdio.h>

void uart_config(void)
{
	PINSEL0|=0X5;
	U0LCR=0X83;
	U0DLL=32;
	U0DLM=0;
	U0LCR=0X3;
}  
void uart_tx(unsigned char data)
{
	U0THR=data;
	while(((U0LSR>>5)&1)==0);
}

unsigned char uart_rx(void)
{
	while(((U0LSR>>0)&1)==0);
	return U0RBR;
}

void uart_str(char *p)
{
	while(*p)
	{
		uart_tx(*p++);
	}
}



void uart_tx_hex(int num){
	char buf[10];
	sprintf(buf,"%X",num);
	uart_str(buf);
}



