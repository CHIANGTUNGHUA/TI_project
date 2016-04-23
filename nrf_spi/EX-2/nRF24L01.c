#include "defineall.h"
#include "delay.h"
#include <msp430x13x.h>
//
void Init_nRF24L01_IO(void)
{
	nRF24L01_CE_OutDIR();
	nRF24L01_CSN_OutDIR();
	nRF24L01_SCK_OutDIR();
	nRF24L01_MOSI_OutDIR();
	nRF24L01_MISO_InDIR();
	nRF24L01_IRQ_InDIR();
//
	nRF24L01_Clear_CE();
	nRF24L01_Set_CSN();
	nRF24L01_Clear_SCK();
	nRF24L01_Clear_MOSI();
}
//function SpiWrite();
/******************************************************************************************/
void nRF24L01SpiWrite(unsigned char byte){
	unsigned char i;
	nRF24L01_Clear_SCK();
	Delay(1);
	for (i=0;i<8;i++) // Setup byte circulation bits
	{
		 if ((byte&BIT7)==BIT7) // Put DATA_BUF.7 on data line
		      nRF24L01_Set_MOSI(); //MOSI=1;
		 else
			  nRF24L01_Clear_MOSI(); //MOSI=0;
			  
		nRF24L01_Set_SCK(); // Set clock line high
		Delay(2);
		byte=byte<<1;
		nRF24L01_Clear_SCK(); // Set clock line low
		Delay(2);
	}
	Delay(1);
}
//function SpiRead(); 
/*****************************************************************************************/
unsigned char nRF24L01SpiRead(void){
	unsigned char i;
	unsigned char temp=0;
	nRF24L01_Clear_SCK();
	Delay(2);
	for (i=0;i<8;i++) // Setup byte circulation bits
	{
		 nRF24L01_Set_SCK(); // Set clock line high
		 Delay(2);
		 temp=temp<<1; // Right shift DATA_BUF
		 if ((nRF24L01_MISO_InPort&nRF24L01_MISO)==nRF24L01_MISO){temp|=1;} // Read data
		 nRF24L01_Clear_SCK(); // Set clock line low
	 	 Delay(2);
	}
	Delay(2);
	return temp; // Return function parameter
}
/******************************************************************************************/
void nRF24L01_Flush_TX_FIFO(void)//Clear TX FIFO
{
	nRF24L01_Set_CSN();
	nRF24L01_Clear_CSN();
	nRF24L01SpiWrite(nRF24L01_FLUSH_TX);
	nRF24L01_Set_CSN();
}
/******************************************************************************************/
void nRF24L01_Flush_RX_FIFO(void)//Clear RX FIFO
{
	nRF24L01_Set_CSN();
	nRF24L01_Clear_CSN();
	nRF24L01SpiWrite(nRF24L01_FLUSH_RX);
	nRF24L01_Set_CSN();
}
/******************************************************************************************/
void nRF24L01SpiWriteReg(unsigned char SpiCommand,unsigned char Content)
{
	nRF24L01_Set_CSN();
	nRF24L01_Clear_CSN();
	nRF24L01SpiWrite(SpiCommand); 
	nRF24L01SpiWrite(Content);
	nRF24L01_Set_CSN();
}
/*******************************************************************************************/
void nRF24L01_Set_Rx_Address(unsigned char RX_Address_Pipex,unsigned char *Address,unsigned char Length)//Local Address
{
	unsigned char i=0;
	nRF24L01_Set_CSN();
	nRF24L01_Clear_CSN();
	nRF24L01SpiWrite(RX_Address_Pipex);
	for(i=0;i<Length;i++)
	{
		nRF24L01SpiWrite(*Address);
		Address++;
	}
	nRF24L01_Set_CSN();
}
/*********************************************************************************************/
void nRF24L01_Set_Tx_Address(unsigned char TX_AddressReg10,unsigned char *Address,unsigned char Length)//Remote Address
{
	unsigned char i=0;
	nRF24L01_Set_CSN();
	nRF24L01_Clear_CSN();
	nRF24L01SpiWrite(TX_AddressReg10);
	for(i=0;i<Length;i++)
	{
		nRF24L01SpiWrite(*Address);
		Address++;
	}
	nRF24L01_Set_CSN();
}
/***********************************************************************************************/
void nRF24L01_Read_Rx_Payload(unsigned char *DataBuff,unsigned char Length)//Payload Out
{
	unsigned char i=0;
	nRF24L01_Set_CSN();
	nRF24L01_Clear_CSN(); 
	nRF24L01SpiWrite(nRF24L01_R_RX_PAYLOAD);
	for(i=0;i<Length;i++)
	{
		*DataBuff=nRF24L01SpiRead();
		DataBuff++;
	}
	nRF24L01_Set_CSN();
}
/**************************************************************************************************/
void nRF24L01_Write_Tx_Payload(unsigned char *DataByte, unsigned char Length)//Payload IN
{
	unsigned char i=0;
	nRF24L01_Set_CSN();
	nRF24L01_Clear_CSN();
	nRF24L01SpiWrite(nRF24L01_W_TX_PAYLOAD);
	for(i=0;i<Length;i++)
	{
		nRF24L01SpiWrite(*DataByte);
		DataByte++;
	}
	nRF24L01_Set_CSN();
}
/**************************************************************************************************/
void nRF24L01_Transmit_Data(void)//Transmit Pulse
{
	Delay(8000);
	nRF24L01_Set_CE();
	Delay(8000);
	nRF24L01_Clear_CE();
}
/**************************************************************************************************/
void nRF24L01_Reset_Tx_DS(void)
{
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_STATUS,0x2e);///
}
/**************************************************************************************************/
void nRF24L01_Reset_Rx_DS(void)
{ 
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_STATUS,0x4e);///
}
/*************************************************************************************************/
void nRF24L01_Setup1(void)//Setup1
{
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_CONFIG,0x3e);//Reflect RX_DR
	//Mask interrupt caused by TX_DS;
	//Mask interrupt caused by MAX_RT;
	//Enable CRC;
	//CRC encoding scheme 2 bytes;
	//POWER UP;
	//PTX;
	Delay(16000);//????
}
/**************************************************************************************************/
void nRF24L01_Setup2(void)//Setup2
{
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_EN_AA,0x00);////Disable auto_acknowledgment,6 pipes
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_EN_RXADDR,0x01);//Enabled RX Addresses.Enable data pipe 0
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_SETUP_AW,0x03);//RX/TX Address field width 5 bytes.
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_SETUP_RETR,0x00);//Re-Transmit disabled.
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RF_CH,0x02);//RF Channel.
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RF_SETUP,0x06);//Air Data Rate 1Mbps.RF_PWR:0dBm.LNA Gain 0.
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RX_PW_P0,0x03);//Pipe0 3 Bytes Payload.
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RX_PW_P1,0x00);//Pipe1
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RX_PW_P2,0x00);//Pipe2
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RX_PW_P3,0x00);//Pipe3
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RX_PW_P4,0x00);//Pipe4
	nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_RX_PW_P5,0x00);//Pipe5
}
