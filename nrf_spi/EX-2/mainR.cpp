////ShockBurst�q�H�覡�A1Mbps�A2�r�`CRC����A�L�۰��������ARF�W�D2�A�޹D0�A5�r�`�a�}�A
//3�r�`�ƾڡA��d�ߨ즳�L�u�ƾڶi�Ӫ��ɭԡAŪ�X
#include<msp430x13x.h>
#include "MSP430_M.h"
#include "delay.h"
#include "initial.h"
#include "defineall.h"
#include "nRF24L01.h"
void Beep(unsigned int BeepCount);//////
unsigned char Rx_Address[5]={100,101,102,103,104};//����a�}
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
nRF24L01_Set_CE();//�i�JRX �Ҧ�
Beep(2);
while(1)
{
	if((nRF24L01_IRQ_InPort&nRF24L01_IRQ)!=nRF24L01_IRQ)//�p�G���L�u�ƾ� 
	{
		nRF24L01_Reset_Rx_DS();//�M�ЧӦ�
		nRF24L01_Read_Rx_Payload(Rx_Buff,3);//Ū���ƾ� 
		nRF24L01_Flush_RX_FIFO();
		Delay(64000);
		Beep(1);//���ܭ�
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
