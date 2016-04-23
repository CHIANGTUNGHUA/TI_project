//ShockBurst通信方式，1Mbps，2字節CRC校驗，無自動應答机制，RF頻道2，管道0，5字節地址，
//3字節數據，當有按鍵按下，就發送數據出去
#include<msp430x13x.h>
#include"MSP430_M.h"
#include"delay.h"
#include"initial.h"
#include"defineall.h"
#include"nRF24L01.h"
voidBeep(unsigned int BeepCount);				//蜂鳴器叫函式
unsigned char KeyScan(void);				    //按鍵掃描函式 
unsigned char Tx_Payload[32]={1,2,3};
unsigned char Tx_Address[5]={100,101,102,103,104};
unsigned char ucharKeyValue=0;
int main(void){
	WDTCTL=WDTPW+WDTHOLD;			
	StopWDTinit_port();						//初始化
	IOinit_clock();							//初始化晶片時脈：MCLK時脈源為XT2CLK,8M//
	Init_nRF24L01_IO();						//GoodBeginning, HalfSuccess.
	nRF24L01_Setup1();//
	Standby-InRF24L01_Setup2();//
	nRF24L01_Set_Rx_Address(nRF24L01_W_REGISTER+nRF24L01_RX_ADDR_P0,Tx_Address,5);//Nosense////
	while(1){
		Delay(64000);
		ucharKeyValue=KeyScan();		 //按鍵掃描
		if(ucharKeyVal<0xf0){					 
		   nRF24L01_Set_Tx_Address(nRF24L01_W_REGISTER+nRF24L01_TX_ADDR,Tx_Address,5);//指定接收
		   Tx_Payload[0]=ucharKeyValue;	//清空 
		   nRF24L01_Write_Tx_Payload(Tx_Payload,3);//把3字節數據放入 
		   nRF24L01_Transmit_Data();  //啟動發送，發送完成后入Standby-I模式
		}
	}
}////蜂鳴器函式 
	voidBeep(unsignedintBeepCount){
			 unsigned int i=0;
			 for(i=0;i<BeepCount;i++){
			 	 setbit(p_beep,beep);Delay(60000);clr
			 	 bit(p_beep,beep);
			 	 Delay(60000);
				  }
	}////按鍵掃描函式 
	unsigned char KeyScan(void){
		unsigned char KeyValue=0xf0;
		unsigned char Ram1=0;
		static unsigned KeyPushFlag=0;
		Ram1=(KeyInPort&0xf0);
		if(Ram1==0xf0)//沒有按鍵輸入
		  {KeyPushFlag=0;}
		else//有鍵按著 
		{
			if(KeyPushFlag==0)//上回掃描的時候沒有按鍵按著 
			{ 
			   KeyPushFlag=1;//置標志位
               Delay(64000);//去按鍵抖動 
               Ram1=(KeyInPort&0xf0);
               if(Ram1<0xf0)//真的有按鍵輸入
			     {KeyValue=Ram1;}//獲得鍵值
			}
		}
	return KeyValue;//返回
	} 
