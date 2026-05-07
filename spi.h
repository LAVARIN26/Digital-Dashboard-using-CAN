char stat;
void spi_init(void)
 {
 	PINSEL0|=0X1500;
	S0SPCCR=150; //100 kbps
	S0SPCR=0x38; //00111000
	IODIR0=1<<7;
	IOSET0=1<<7;
 }
char spio(char data)
 {
	 stat = S0SPSR;
	 S0SPDR = data;
	 while(((S0SPSR>>7)&1)==0);
	 return S0SPDR;	
 }

 
float MCP3204(unsigned char channelNo)
{
  unsigned char hByte,lByte;
  unsigned int adcVal=0;
  IOCLR0=1<<7;
  spio(0x06);
  hByte = spio(channelNo<<6);
  lByte = spio(0x00);
  IOSET0=1<<7;
  adcVal=((hByte&0x0f)<<8)|lByte;
  return ((adcVal*3.3)/4096);
}




