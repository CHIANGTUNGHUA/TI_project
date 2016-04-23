////ShockBurst通信方式，1Mbps，2字節CRC校驗，無自動應答机制，RF頻道2，管道0，5字節地址，
//3字節數據，當查詢到有無線數據進來的時候，讀出
#include<msp430x13x.h>
#include "MSP430_M.h"
#include "delay.h"
#include "initial.h"
#include "defineall.h"
#include "nRF24L01.h"
void Beep(unsigned int BeepCount);//////
unsigned char Rx_Address[5]={100,101,102,103,104};//本机地址
unsigned char Rx_Buff[32];
int main(void){
WDTCTL = WDTPW + WDTHOLD; // Stop WDT
init_port();
init_clock();
Init_nRF24L01_IO();
nRF24L01_Setup2();//
nRF24L01_Set_Rx_Address(nRF24L01_W_REGISTER+nRF24L01_RX_ADDR_P0,Rx_Address,5);//
nRF24L01_Setup1();//
nRF24L01_Flush_RX_FIFO();
nRF24L01_Set_CE();//進入RX 模式
Beep(2);
while(1)
{
	if((nRF24L01_IRQ_InPort&nRF24L01_IRQ)!=nRF24L01_IRQ)//如果有無線數據 
	{
		nRF24L01_Reset_Rx_DS();//清標志位
		nRF24L01_Read_Rx_Payload(Rx_Buff,3);//讀取數據 
		nRF24L01_Flush_RX_FIFO();
		Delay(64000);
		Beep(1);//提示音
	}
		Delay(64000);
	}
}
//
//
void Beep(unsigned int BeepCount)
{
unsigned int i=0;
for(i=0;i<BeepCount;i++)
{
setbit(p_beep,beep);
Delay(60000);
clrbit(p_beep,beep);
Delay(60000);
}
} 
