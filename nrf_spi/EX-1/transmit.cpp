#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned char uint;
//****************************************NRF24L01端口定義***************************************
sbit 	MISO	=P1^5;
sbit 	MOSI	=P1^1;
sbit	SCK	    =P1^6;
sbit	CE	    =P1^7;
sbit	CSN		=P1^2;
sbit	IRQ		=P1^0;
//************************************按鍵***************************************************
sbit	KEY1=P3^4;
//************************************蜂明器***************************************************
sbit 	LED=P3^5;
//***********************************發送緩衝區*********************************************
uchar TxBuf[32]={
0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08,
0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,
0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32,};	 
//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 20 uints TX payload
#define RX_PLOAD_WIDTH  32  	// 20 uints TX payload
uint const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
uint const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址
//***************************************NRF24L01寄存器指令*******************************************************
#define READ_REG        0x00  	// 讀寄存器指令
#define WRITE_REG       0x20 	// 寫寄存器指令
#define RD_RX_PLOAD     0x61  	// 讀取接收數據指令 R_RX_PAYLOAD
#define WR_TX_PLOAD     0xA0  	// 寫待發指令 W_TX_PAYLOAD
#define FLUSH_TX        0xE1 	// 清除TX FIFO指令 	 
#define FLUSH_RX        0xE2  	// 清除RX FIFO指令
#define REUSE_TX_PL     0xE3  	// 重新使用上一包發送的有效數據，當CE = 1數據不斷被重新發射 
#define NOP             0xFF  	// 保留No Operation. Might be used to read the STATUS register
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收發狀態，CRC校驗以及收發狀態響應模式 	
#define EN_AA           0x01  // 自動應答功能設置
#define EN_RXADDR       0x02  // Enabled RX Addresses
#define SETUP_AW        0x03  // Setup of Address Widths
#define SETUP_RETR      0x04  // Setup of Automatic Retransmission
#define RF_CH           0x05  // RF Channel工作頻率設置
#define RF_SETUP        0x06  // RF Setup Register
#define STATUS          0x07  // 狀態寄存器
#define OBSERVE_TX      0x08  // 發送監測功能
#define CD              0x09  // 地址檢測                
#define RX_ADDR_P0      0x0A  // 頻道0接收數據地址
#define RX_ADDR_P1      0x0B  // 頻道1接收數據地址
#define RX_ADDR_P2      0x0C  // 頻道2接收數據地址
#define RX_ADDR_P3      0x0D  // 頻道3接收數據地址
#define RX_ADDR_P4      0x0E  // 頻道4接收數據地址
#define RX_ADDR_P5      0x0F  // 頻道5接收數據地址
#define TX_ADDR         0x10  // 發送地址寄存器
#define RX_PW_P0        0x11  // Number of bytes in RX payload in data pipe 0 (1 to 32 bytes).
#define RX_PW_P1        0x12  // Number of bytes in RX payload in data pipe 1 (1 to 32 bytes).
#define RX_PW_P2        0x13  // Number of bytes in RX payload in data pipe 2 (1 to 32 bytes).
#define RX_PW_P3        0x14  // Number of bytes in RX payload in data pipe 3 (1 to 32 bytes).
#define RX_PW_P4        0x15  // Number of bytes in RX payload in data pipe 4 (1 to 32 bytes).
#define RX_PW_P5        0x16  // Number of bytes in RX payload in data pipe 5 (1 to 32 bytes). 
#define FIFO_STATUS     0x17  // FIFO Status Register
//**************************************************************************************
void Delay(unsigned int s);
void inerDelay_us(unsigned char n);
void init_NRF24L01(void);
uint SPI_RW(uint uchar);
uchar SPI_Read(uchar reg);
void SetRX_Mode(void);
uint SPI_RW_Reg(uchar reg, uchar value);
uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars);
uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars);
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);
//*****************************************長延時*****************************************
void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}
//******************************************************************************************
uint 	bdata sta;   //狀態標誌
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;
/******************************************************************************************
/*延時函式
/******************************************************************************************/
void inerDelay_us(unsigned char n)
{
	for(;n>0;n--)
		_nop_();
}
//****************************************************************************************
/*NRF24L01初始化
//***************************************************************************************/
void init_NRF24L01(void)
{
    inerDelay_us(100);
 	CE=0;    // chip enable
 	CSN=1;   // Spi disable 
 	SCK=0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 寫本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 寫接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  頻道0自動	ACK應答允許		
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允許接收地址只有頻道0
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   設置工作頻率2.4GHZ，收發必需一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //設置接收數據長度，本次設置為20 bit
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//設置工作頻率1MHZ，發射功率為最大值0dB	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收發完成中斷響應，16位CRC，主發送	 

}
/****************************************************************************************************
/*函式：uint SPI_RW(uint uchar)
/*功能：NRF24L01的SPI寫時序
/****************************************************************************************************/
uint SPI_RW(uint uchar)
{
	uint bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		MOSI = (uchar & 0x80);         // output 'uchar', MSB to MOSI
		uchar = (uchar << 1);           // shift next bit into MSB..
		SCK = 1;                      // Set SCK high..
		uchar |= MISO;       		  // capture current MISO bit
		SCK = 0;            		  // ..then set SCK low again
   	}
    return(uchar);           		  // return read uchar
}
/****************************************************************************************************
/*函式：uchar SPI_Read(uchar reg)
/*功能：NRF24L01的SPI時序
/****************************************************************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	
	CSN = 0;                // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}
/****************************************************************************************************/
/*功能：NRF24L01讀寫寄存器函數 
/****************************************************************************************************/
uint SPI_RW_Reg(uchar reg, uchar value)
{
	uint status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status uchar
}
/****************************************************************************************************/
/*函式：uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*功能: 用於讀取數據，reg：寄存器地址；pBuf：為待讀出數據地址，uchars：讀出數據的個數 
/****************************************************************************************************/
uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	status = SPI_RW(reg);       		// Select register to write to and read status uchar
	
	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
		pBuf[uchar_ctr] = SPI_RW(0);    // 
	
	CSN = 1;                           
	
	return(status);                    // return nRF24L01 status uchar
}
/*********************************************************************************************************
/*函數：uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*功能: 用於寫數據，reg:寄存器地址；pBuf：待寫入數據地址；uchars：寫入數據的個數 
/*********************************************************************************************************/
uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;            //SPI使能       
	status = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
		SPI_RW(*pBuf++);
	CSN = 1;           //關閉SPI
	return(status);    // 
}
/****************************************************************************************************/
/*函式：void SetRX_Mode(void)
/*功能：數據接收配置  
/****************************************************************************************************/
void SetRX_Mode(void)
{
	CE=0;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ收發完成中響應，16位CRC，主接收
	CE = 1; 
	inerDelay_us(130);    //延時不能太短
}
/******************************************************************************************************/
/*函數：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
/*功能：數據讀取后放至rx_buf接收緩衝區中
/******************************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
{
    unsigned char revale=0;
	sta=SPI_Read(STATUS);	// 讀取狀態寄存器來判斷數據接收狀況 
	if(RX_DR)				// 判斷是否接收數據 
	{
	    CE = 0; 			//SPI使能
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//數據讀取完成標示 
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到數據後RX_DR,TX_DS,MAX_PT都置高，透過寫1清除中斷標誌 
	return revale;
}
/***********************************************************************************************************
/*函式：void nRF24L01_TxPacket(unsigned char * tx_buf)
/*功能：發送 tx_buf中數據 
/**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE=0;			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 裝載接收端地址	
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 裝載數據	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收發完成中響應，16位CRC，主發送
	CE=1;		 //置高CE，激發數據發送 
	inerDelay_us(10);
}
//************************************主函式************************************************************
void main(void)
{
	uchar temp =0;	
    init_NRF24L01() ;
	nRF24L01_TxPacket(TxBuf);	// Transmit Tx buffer data
	Delay(6000);
	P0=0xBF;
	while(1)
	{	
		nRF24L01_TxPacket(TxBuf);	// Transmit Tx buffer data
		LED=0;
		Delay(10000);      //可變 
		SPI_RW_Reg(WRITE_REG+STATUS,0XFF);  
		LED=1;
		Delay(8000); 
		}
	
}
