//ShockBurst�q�H�覡�A1Mbps�A2�r�`CRC����A�L�۰��������ARF�W�D2�A�޹D0�A5�r�`�a�}�A
//3�r�`�ƾڡA��������U�A�N�o�e�ƾڥX�h
#include<msp430x13x.h>
#include"MSP430_M.h"
#include"delay.h"
#include"initial.h"
#include"defineall.h"
#include"nRF24L01.h"
voidBeep(unsigned int BeepCount);				//���ﾹ�s�禡
unsigned char KeyScan(void);				    //���䱽�y�禡 
unsigned char Tx_Payload[32]={1,2,3};
unsigned char Tx_Address[5]={100,101,102,103,104};
unsigned char ucharKeyValue=0;
int main(void){
	WDTCTL=WDTPW+WDTHOLD;			
	StopWDTinit_port();						//��l��
	IOinit_clock();							//��l�ƴ����ɯߡGMCLK�ɯ߷���XT2CLK,8M//
	Init_nRF24L01_IO();						//GoodBeginning, HalfSuccess.
	nRF24L01_Setup1();//
	Standby-InRF24L01_Setup2();//
	nRF24L01_Set_Rx_Address(nRF24L01_W_REGISTER+nRF24L01_RX_ADDR_P0,Tx_Address,5);//Nosense////
	while(1){
		Delay(64000);
		ucharKeyValue=KeyScan();		 //���䱽�y
		if(ucharKeyVal<0xf0){					 
		   nRF24L01_Set_Tx_Address(nRF24L01_W_REGISTER+nRF24L01_TX_ADDR,Tx_Address,5);//���w����
		   Tx_Payload[0]=ucharKeyValue;	//�M�� 
		   nRF24L01_Write_Tx_Payload(Tx_Payload,3);//��3�r�`�ƾک�J 
		   nRF24L01_Transmit_Data();  //�Ұʵo�e�A�o�e�����Z�JStandby-I�Ҧ�
		}
	}
}////���ﾹ�禡 
	voidBeep(unsignedintBeepCount){
			 unsigned int i=0;
			 for(i=0;i<BeepCount;i++){
			 	 setbit(p_beep,beep);Delay(60000);clr
			 	 bit(p_beep,beep);
			 	 Delay(60000);
				  }
	}////���䱽�y�禡 
	unsigned char KeyScan(void){
		unsigned char KeyValue=0xf0;
		unsigned char Ram1=0;
		static unsigned KeyPushFlag=0;
		Ram1=(KeyInPort&0xf0);
		if(Ram1==0xf0)//�S�������J
		  {KeyPushFlag=0;}
		else//������� 
		{
			if(KeyPushFlag==0)//�W�^���y���ɭԨS��������� 
			{ 
			   KeyPushFlag=1;//�m�ЧӦ�
               Delay(64000);//�h����ݰ� 
               Ram1=(KeyInPort&0xf0);
               if(Ram1<0xf0)//�u���������J
			     {KeyValue=Ram1;}//��o���
			}
		}
	return KeyValue;//��^
	} 
