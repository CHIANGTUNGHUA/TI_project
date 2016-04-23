#include <msp430x13x.h>
#include "defineall.h"
/*端口初始化*/
void init_port(void)
{
P1SEL = 0;//
P1DIR = BIT1+BIT2+BIT3+BIT6+BIT7;
P1OUT = BIT7;
P2SEL = 0;//
P2DIR = BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6;
P2OUT = BIT0+BIT1+BIT5;
//P2DIR = 0x00;
P3SEL = BIT5+BIT4; // P3.4,5 = USART0 TXD/RXD
P3DIR = BIT1+BIT2+BIT3;//
P3OUT = BIT1;
P4SEL = 0;
P4DIR=BIT3;//
P4OUT=0;
P5SEL=0x00;//選擇P5端口為I/O端口
P5DIR=0;//
P5OUT=0;
P6SEL=BIT3+BIT5;
P6DIR=BIT6+BIT7;
P6OUT=0;}
//*****時脈設置初始化*****//在MSP430F1XX系列中，時脈的控制由3個寄存器來完成：DCOCTL，BCSCTL1及BCSCTL2
void init_clock(void){
     unsignedinti;
	 BCSCTL1=XT2OFF&(~0x80);	//XT2振盪器開啟+LFXT1低頻模式+ACLK不分頻 
	 do{
	    clrbit(IFG1,OFIFG);
		for(i=0x0fff;i>0;--i);
		}
		while((IFG1&OFIFG));
		BCSCTL2=SELM_2+SELS;	//MCLK時脈源選為XT2CLK,8M
		} 
