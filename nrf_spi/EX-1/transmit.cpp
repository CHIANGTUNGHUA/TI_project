#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned char uint;
//****************************************NRF24L01�ݤf�w�q***************************************
sbit 	MISO	=P1^5;
sbit 	MOSI	=P1^1;
sbit	SCK	    =P1^6;
sbit	CE	    =P1^7;
sbit	CSN		=P1^2;
sbit	IRQ		=P1^0;
//************************************����***************************************************
sbit	KEY1=P3^4;
//************************************������***************************************************
sbit 	LED=P3^5;
//***********************************�o�e�w�İ�*********************************************
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
uint const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���a�a�}
uint const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//�����a�}
//***************************************NRF24L01�H�s�����O*******************************************************
#define READ_REG        0x00  	// Ū�H�s�����O
#define WRITE_REG       0x20 	// �g�H�s�����O
#define RD_RX_PLOAD     0x61  	// Ū�������ƾګ��O R_RX_PAYLOAD
#define WR_TX_PLOAD     0xA0  	// �g�ݵo���O W_TX_PAYLOAD
#define FLUSH_TX        0xE1 	// �M��TX FIFO���O 	 
#define FLUSH_RX        0xE2  	// �M��RX FIFO���O
#define REUSE_TX_PL     0xE3  	// ���s�ϥΤW�@�]�o�e�����ļƾڡA��CE = 1�ƾڤ��_�Q���s�o�g 
#define NOP             0xFF  	// �O�dNo Operation. Might be used to read the STATUS register
//*************************************SPI(nRF24L01)�H�s���a�}****************************************************
#define CONFIG          0x00  // �t�m���o���A�ACRC����H�Φ��o���A�T���Ҧ� 	
#define EN_AA           0x01  // �۰������\��]�m
#define EN_RXADDR       0x02  // Enabled RX Addresses
#define SETUP_AW        0x03  // Setup of Address Widths
#define SETUP_RETR      0x04  // Setup of Automatic Retransmission
#define RF_CH           0x05  // RF Channel�u�@�W�v�]�m
#define RF_SETUP        0x06  // RF Setup Register
#define STATUS          0x07  // ���A�H�s��
#define OBSERVE_TX      0x08  // �o�e�ʴ��\��
#define CD              0x09  // �a�}�˴�                
#define RX_ADDR_P0      0x0A  // �W�D0�����ƾڦa�}
#define RX_ADDR_P1      0x0B  // �W�D1�����ƾڦa�}
#define RX_ADDR_P2      0x0C  // �W�D2�����ƾڦa�}
#define RX_ADDR_P3      0x0D  // �W�D3�����ƾڦa�}
#define RX_ADDR_P4      0x0E  // �W�D4�����ƾڦa�}
#define RX_ADDR_P5      0x0F  // �W�D5�����ƾڦa�}
#define TX_ADDR         0x10  // �o�e�a�}�H�s��
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
//*****************************************������*****************************************
void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}
//******************************************************************************************
uint 	bdata sta;   //���A�лx
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;
/******************************************************************************************
/*���ɨ禡
/******************************************************************************************/
void inerDelay_us(unsigned char n)
{
	for(;n>0;n--)
		_nop_();
}
//****************************************************************************************
/*NRF24L01��l��
//***************************************************************************************/
void init_NRF24L01(void)
{
    inerDelay_us(100);
 	CE=0;    // chip enable
 	CSN=1;   // Spi disable 
 	SCK=0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // �g���a�a�}	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // �g�����ݦa�}
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  �W�D0�۰�	ACK�������\		
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ���\�����a�}�u���W�D0
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   �]�m�u�@�W�v2.4GHZ�A���o���ݤ@�P
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //�]�m�����ƾڪ��סA�����]�m��20 bit
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//�]�m�u�@�W�v1MHZ�A�o�g�\�v���̤j��0dB	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ���o�������_�T���A16��CRC�A�D�o�e	 

}
/****************************************************************************************************
/*�禡�Guint SPI_RW(uint uchar)
/*�\��GNRF24L01��SPI�g�ɧ�
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
/*�禡�Guchar SPI_Read(uchar reg)
/*�\��GNRF24L01��SPI�ɧ�
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
/*�\��GNRF24L01Ū�g�H�s����� 
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
/*�禡�Guint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*�\��: �Ω�Ū���ƾڡAreg�G�H�s���a�}�FpBuf�G����Ū�X�ƾڦa�}�Auchars�GŪ�X�ƾڪ��Ӽ� 
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
/*��ơGuint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*�\��: �Ω�g�ƾڡAreg:�H�s���a�}�FpBuf�G�ݼg�J�ƾڦa�}�Fuchars�G�g�J�ƾڪ��Ӽ� 
/*********************************************************************************************************/
uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;            //SPI�ϯ�       
	status = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
		SPI_RW(*pBuf++);
	CSN = 1;           //����SPI
	return(status);    // 
}
/****************************************************************************************************/
/*�禡�Gvoid SetRX_Mode(void)
/*�\��G�ƾڱ����t�m  
/****************************************************************************************************/
void SetRX_Mode(void)
{
	CE=0;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ���o�������T���A16��CRC�A�D����
	CE = 1; 
	inerDelay_us(130);    //���ɤ���ӵu
}
/******************************************************************************************************/
/*��ơGunsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
/*�\��G�ƾ�Ū���Z���rx_buf�����w�İϤ�
/******************************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
{
    unsigned char revale=0;
	sta=SPI_Read(STATUS);	// Ū�����A�H�s���ӧP�_�ƾڱ������p 
	if(RX_DR)				// �P�_�O�_�����ƾ� 
	{
	    CE = 0; 			//SPI�ϯ�
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//�ƾ�Ū�������Х� 
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //������ƾګ�RX_DR,TX_DS,MAX_PT���m���A�z�L�g1�M�����_�лx 
	return revale;
}
/***********************************************************************************************************
/*�禡�Gvoid nRF24L01_TxPacket(unsigned char * tx_buf)
/*�\��G�o�e tx_buf���ƾ� 
/**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE=0;			//StandBy I�Ҧ�	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // �˸������ݦa�}	
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // �˸��ƾ�	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ���o�������T���A16��CRC�A�D�o�e
	CE=1;		 //�m��CE�A�E�o�ƾڵo�e 
	inerDelay_us(10);
}
//************************************�D�禡************************************************************
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
		Delay(10000);      //�i�� 
		SPI_RW_Reg(WRITE_REG+STATUS,0XFF);  
		LED=1;
		Delay(8000); 
		}
	
}
