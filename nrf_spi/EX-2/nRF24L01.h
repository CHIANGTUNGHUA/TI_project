extern void Init_nRF24L01_IO(void);
extern void nRF24L01SpiWrite(unsignedcharbyte);
extern unsigned char nRF24L01SpiRead(void);
extern void nRF24L01_Setup1(void);
extern void nRF24L01_Setup2(void);
extern void nRF24L01_Set_Rx_Address(unsigned char RX_Address_Pipex,unsigned char *Address,unsigned char Length);
extern void nRF24L01_Read_Rx_Payload(unsigned char *DataBuff,unsigned char Length);
extern void nRF24L01_Set_Tx_Address(unsigned char TX_AddressReg10,unsigned char *Address,unsigned char Length);//
extern void nRF24L01_Write_Tx_Payload(unsigned char *DataByte,unsigned char Length);
extern void nRF24L01_Transmit_Data(void);
extern void nRF24L01_Reset_Tx_DS(void);
extern void nRF24L01_Reset_Rx_DS(void);
extern void nRF24L01_Flush_TX_FIFO(void);
extern void nRF24L01_Flush_RX_FIFO(void);
extern void nRF24L01SpiWriteReg(unsigned char SpiCommand,unsigned char Content);

