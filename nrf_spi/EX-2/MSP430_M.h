#define clrbit(reg,bit) reg&=~(bit) //清寄存器的某1比特位
#define bitclr(reg,bit) reg&=~(bit) //清寄存器的某1比特位
#define setbit(reg,bit) reg|=(bit)  //設置寄存器的某1比特位
#define bitset(reg,bit) reg|=(bit)  //設置寄存器的某1比特位
#define cplbit(reg,bit) reg^=(bit)  //對寄存器的某1比特位取反向 
#define bitcpl(reg,bit) reg^=(bit)  //對寄存器的某1比特位取反向  
