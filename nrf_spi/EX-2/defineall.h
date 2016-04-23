#define 	beep 			BIT3
#define 	p_beep 			P4OUT
////«öÁä 
#define 	KeyInPort 		P4IN
////nRF24L01Definitions
#define 	nRF24L01_CE 		BIT4//P2.4
#define 	nRF24L01_CSN 		BIT5//P2.5
#define 	nRF24L01_SCK 		BIT6//P2.6 
#define 	nRF24L01_MOSI 		BIT3//P3.3 
#define 	nRF24L01_MISO 		BIT7//P3.7 
#define 	nRF24L01_IRQ 		BIT6//P3.6 
//// 
#define 	nRF24L01_CE_DIR 		P2DIR 
#define 	nRF24L01_CSN_DIR 		P2DIR 
#define 	nRF24L01_SCK_DIR 		P2DIR
#define 	nRF24L01_MOSI_DIR 		P3DIR 
#define 	nRF24L01_MISO_DIR 		P3DIR 
#define 	nRF24L01_IRQ_DIR  		P3DIR
////  
#define 	nRF24L01_CE_OutPort 	P2OUT 
#define 	nRF24L01_CSN_OutPort 	P2OUT 
#define 	nRF24L01_SCK_OutPort 	P2OUT
#define 	nRF24L01_MOSI_OutPort 	P3OUT 
////
#define 	nRF24L01_MISO_InPort 		P3IN 
#define 	nRF24L01_IRQ_InPort 		P3IN
//// 
#define 	nRF24L01_CE_OutDIR() 	setbit(nRF24L01_CE_DIR,nRF24L01_CE) 
#define 	nRF24L01_CSN_OutDIR() 	setbit(nRF24L01_CSN_DIR, nRF24L01_CSN)
#define 	nRF24L01_SCK_OutDIR() 	setbit(nRF24L01_SCK_DIR,nRF24L01_SCK)
#define 	nRF24L01_MOSI_OutDIR() 	setbit(nRF24L01_MOSI_DIR,nRF24L01_MOSI) 
#define 	nRF24L01_MISO_InDIR() 	clrbit(nRF24L01_MISO_DIR,nRF24L01_MISO)
#define 	nRF24L01_IRQ_InDIR() 	clrbit(nRF24L01_IRQ_DIR,nRF24L01_IRQ) 
#define 	nRF24L01_Set_CE() 		setbit(nRF24L01_CE_OutPort,nRF24L01_CE) 
#define 	nRF24L01_Clear_CE() 	clrbit(nRF24L01_CE_OutPort,nRF24L01_CE)
#define 	nRF24L01_Set_CSN() 		setbit(nRF24L01_CSN_OutPort,nRF24L01_CSN) 
#define 	nRF24L01_Clear_CSN() 	clrbit(nRF24L01_CSN_OutPort,nRF24L01_CSN) 
#define 	nRF24L01_Set_SCK() 		setbit(nRF24L01_SCK_OutPort,nRF24L01_SCK)
#define 	nRF24L01_Clear_SCK() 	clrbit(nRF24L01_SCK_OutPort,nRF24L01_SCK)
#define 	nRF24L01_Set_MOSI() 	setbit(nRF24L01_MOSI_OutPort,nRF24L01_MOSI)
#define 	nRF24L01_Clear_MOSI() 	clrbit(nRF24L01_MOSI_OutPort,nRF24L01_MOSI) 
// //SPI Commands 
#define 	nRF24L01_R_REGISTER 	0x00 
#define		nRF24L01_W_REGISTER 	0x20
#define 	nRF24L01_R_RX_PAYLOAD 	0x61 
#define 	nRF24L01_W_TX_PAYLOAD 	0xA0 
#define 	nRF24L01_FLUSH_TX 		0xE1
#define 	nRF24L01_FLUSH_RX 		0xE2 
#define 	nRF24L01_REUSE_TX_PL 	0xE3 
#define 	nRF24L01_NOP 			0xFF 
// // // // // //Register Definitions 
#define 	nRF24L01_CONFIG 		0x00 
#define 	nRF24L01_EN_AA 			0x01 
#define 	nRF24L01_EN_RXADDR 		0x02
#define 	nRF24L01_SETUP_AW 		0x03 
#define 	nRF24L01_SETUP_RETR 	0x04 
#define 	nRF24L01_RF_CH 			0x05
#define 	nRF24L01_RF_SETUP 		0x06 
#define 	nRF24L01_STATUS 		0x07 
#define 	nRF24L01_OBSERVE_TX 	0x08 
#define 	nRF24L01_CD 			0x09
#define 	nRF24L01_RX_ADDR_P0 	0x0A 
#define 	nRF24L01_RX_ADDR_P1 	0x0B 
#define 	nRF24L01_RX_ADDR_P2 	0x0C
#define 	nRF24L01_RX_ADDR_P3 	0x0D 
#define 	nRF24L01_RX_ADDR_P4 	0x0E 
#define 	nRF24L01_RX_ADDR_P5 	0x0F 
#define 	nRF24L01_TX_ADDR 		0x10
#define 	nRF24L01_RX_PW_P0 		0x11 
#define 	nRF24L01_RX_PW_P1 		0x12 
#define 	nRF24L01_RX_PW_P2 		0x13
#define 	nRF24L01_RX_PW_P3 		0x14 
#define 	nRF24L01_RX_PW_P4 		0x15 
#define 	nRF24L01_RX_PW_P5 		0x16 
#define 	nRF24L01_FIFO_STATUS 	0x17
