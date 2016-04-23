void nRF24L01_Setup1(void)//
{
nRF24L01SpiWriteReg(nRF24L01_W_REGISTER+nRF24L01_CONFIG,0x3f);//Reflect RX_DR;
//Mask interrupt caused by TX_DS;
//Mask interrupt caused by MAX_RT;
//EnableCRC;
//CRC encoding scheme 2bytes;
//POWERUP; 
//PRX; 
Delay(16000);
} 
