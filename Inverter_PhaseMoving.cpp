/*
 GPIO18 - > ADC LED

 GPIO20 - > PWM LED

 GPIO26 - > SW ADC

 GPIO24 - > SW PWM

 PWM :

 GPIO6 - >  MOS2  	GPIO7 - > MOS1
 GPIO8 - >  MOS4	    GPIO9 - > MOS3
 */
//###########################################################################
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include <string.h>
#include <stdint.h>

#define FREQ_TBPRD	600
#define Initial_Value_Of_CMPA	FREQ_TBPRD
#define Half_FREQ_TBPRD		FREQ_TBPRD/2
#define Expectative_Value	1228
#define High_Bounded_Vo_Value 	3583
#define PHASE_ANGLE 	0
#define DEAD_TIME 	30
//#define High_Bounded_Io_Value 	????

// Prototype statements for functions found within this file.
__interrupt void cpu_timer0_isr(void);
void InitEPwm3Example(void);
void InitEPwm4Example(void);
void InitEPwm5Example(void);
void InitEPwm6Example(void);
void InitPIvalue(void);
void PIvalue(void);

int VoValue[256], IoValue[256], ADCCount = 0, TempVoValue = 0, TempIoValue = 0,
		CMPAValue = 0 ,  IoCalVlue = 0;
volatile int verr, vk1, vk2;
long int vout = 0, vko1, vko2, vou2, v1 = 0, v2 = 0, ZForward = 0,
		ZBackward = 0;
volatile int LED_DELAY = 0;
volatile float kP = 0.05, kI = 0.0005;
volatile int i; // Loop Count
//extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
int sintable1[256] = { 0, 13, 25, 38, 50, 63, 75, 88, 100, 112, 124, 137, 149,
		161, 172, 184, 196, 207, 219, 230, 241, 252, 263, 274, 284, 295, 305,
		315, 325, 334, 344, 353, 362, 371, 379, 388, 396, 404, 411, 419, 426,
		433, 439, 445, 452, 457, 463, 468, 473, 478, 482, 486, 490, 493, 497,
		500, 502, 504, 506, 508, 510, 511, 511, 512, 512, 512, 511, 511, 510,
		508, 506, 504, 502, 500, 497, 493, 490, 486, 482, 478, 473, 468, 463,
		457, 452, 445, 439, 433, 426, 419, 411, 404, 396, 388, 379, 371, 362,
		353, 344, 334, 325, 315, 305, 295, 284, 274, 263, 252, 241, 230, 219,
		207, 196, 184, 172, 161, 149, 137, 124, 112, 100, 88, 75, 63, 50, 38,
		25, 13, 0, -13, -25, -38, -50, -63, -75, -88, -100, -112, -124, -137,
		-149, -161, -172, -184, -196, -207, -219, -230, -241, -252, -263, -274,
		-284, -295, -305, -315, -325, -334, -344, -353, -362, -371, -379, -388,
		-396, -404, -411, -419, -426, -433, -439, -445, -452, -457, -463, -468,
		-473, -478, -482, -486, -490, -493, -497, -500, -502, -504, -506, -508,
		-510, -511, -511, -512, -512, -512, -511, -511, -510, -508, -506, -504,
		-502, -500, -497, -493, -490, -486, -482, -478, -473, -468, -463, -457,
		-452, -445, -439, -433, -426, -419, -411, -404, -396, -388, -379, -371,
		-362, -353, -344, -334, -325, -315, -305, -295, -284, -274, -263, -252,
		-241, -230, -219, -207, -196, -184, -172, -161, -149, -137, -124, -112,
		-100, -88, -75, -63, -50, -38, -25, -13, };
void main(void) {

	InitPIvalue();

	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	//memcpy((uint16_t *)&RamfuncsRunStart,(uint16_t *)&RamfuncsLoadStart, (unsigned long)&RamfuncsLoadSize);
	// MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	memcpy((uint16_t *) &RamfuncsRunStart, (uint16_t *) &RamfuncsLoadStart,
			(unsigned long) &RamfuncsLoadSize);

	InitSysCtrl();

	InitAdc();
	InitFlash();
	//Configure EPwm3 , 2
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	InitEPwm3Gpio();  //Configure GPIO4 as EPwm3A  ,   GPIO5 as EPwm3B
	InitEPwm4Gpio();  //Configure GPIO6 as EPwm3A  ,   GPIO7 as EPwm3B

	InitEPwm3Example();
	InitEPwm4Example();

	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;

	SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;
	SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;

	// configure ADCSCOC0
	EALLOW;
	//AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0 = 0 ; // Single sample mode set for SOC0 and SOC1
	//AdcRegs.ADCSAMPLEMODE.bit.SIMULEN2 = 0 ; // Single sample mode set for SOC2 and SOC3

	//AdcRegs.ADCCTL1.bit.ADCENABLE = 0;  // 不致能ADC (在initadc中已經致能) ////////// ???????????????????????????????????
	//AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1; //ADCINT1 trips after AdcResults latch

	AdcRegs.INTSEL1N2.bit.INT1E = 0;	//disabled ADCINT1
	AdcRegs.INTSEL1N2.bit.INT2E = 0;	//disabled ADCINT2
	AdcRegs.INTSEL1N2.bit.INT1CONT = 0;	//Disable ADCINT1 Continuous mode
	AdcRegs.INTSEL1N2.bit.INT2CONT = 0;	//Disable ADCINT2 Continuous mode

	// AdcRegs.INTSEL1N2.bit.INT1SEL = 0;	//setup EOC0 to trigger ADCINT1 to fire

	AdcRegs.ADCSOC0CTL.bit.CHSEL = 0;	//set SOC0 channel select to ADCINA0
	AdcRegs.ADCSOC0CTL.bit.TRIGSEL = 1;	//set SOC0 start trigger on CPUtimer
	AdcRegs.ADCSOC0CTL.bit.ACQPS = 6;//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC1CTL.bit.CHSEL = 1;	//set SOC1 channel select to ADCINA1
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL = 1;	//set SOC1 start trigger on CPUtimer
	AdcRegs.ADCSOC1CTL.bit.ACQPS = 6; //set SOC1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	//AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

	EDIS;

	// Configure Gpio control
	EALLOW;
	// Initial the Port of Epwm !!
	GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;

	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;          // OUTput
	GpioDataRegs.GPACLEAR.bit.GPIO18 = 1; 		// 當ADC ON/OFF 標示燈

	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;          // OUTput
	GpioDataRegs.GPACLEAR.bit.GPIO20 = 1; 		// 當 ePWM ON/OFF 標示燈

	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;          // OUTput 閃爍用
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;

	// turn off the lights that aren't used
	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
	GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;

	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;

	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
	GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;

	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;

	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;         // GPIO ADC開關
	GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;          // input
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;			// GPIO ePWM開關
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;

	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
	EDIS;

	// Configure Cputimer
	EALLOW;
	// This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;
	// This is needed to disable write to EALLOW protected registers
	InitCpuTimers();   // For this example, only initialize the Cpu Timer
	ConfigCpuTimer(&CpuTimer0, 1, 3906);
	CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0

	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM

	for (;;)
		;
}

void InitPIvalue() {
	for (i = 0; i < 255; i++) {
		VoValue[i] = 0;
		IoValue[i] = 0;
	}

}

void InitEPwm3Example() {
	//SysCtrlRegs.PCLKCR1.bit.EPwm5ENCLK = 1;

	// Setup TBCLK
	EPwm3Regs.TBPRD = FREQ_TBPRD;           // Set timer period 801 TBCLKs
	EPwm3Regs.TBPHS.half.TBPHS = 0;           // Phase is 0
	EPwm3Regs.TBCTR = 0x0000;                      // Clear counter

	// Set Compare values
	EPwm3Regs.CMPA.half.CMPA = Initial_Value_Of_CMPA;     // Set compare A value
	EPwm3Regs.CMPB = Initial_Value_Of_CMPA;               // Set Compare B value

	// Setup counter mode
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Enable phase loading
	EPwm3Regs.TBCTL.bit.PHSDIR = TB_COUNT_UP;
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;       // Clock ratio to SYSCLKOUT
	EPwm3Regs.TBCTL.bit.CLKDIV = 0;

	// EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;

	// Setup shadowing
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	// Set actions
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET; // Set PWM2A on event A, up count             // Set PWM1A on event A, up count
	EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;

	EPwm3Regs.DBCTL.bit.IN_MODE = 0x00;
	EPwm3Regs.DBCTL.bit.OUT_MODE = 0x03;
	EPwm3Regs.DBCTL.bit.POLSEL = 0x00;
	EPwm3Regs.DBCTL.bit.HALFCYCLE = 0x00;
	EPwm3Regs.DBRED = DEAD_TIME;
	EPwm3Regs.DBFED = DEAD_TIME;

}

void InitEPwm4Example() {
	//SysCtrlRegs.PCLKCR1.bit.EPwm5ENCLK = 1;

	// Setup TBCLK
	EPwm4Regs.TBPRD = FREQ_TBPRD;           // Set timer period 801 TBCLKs
	EPwm4Regs.TBPHS.half.TBPHS = PHASE_ANGLE;           // Phase is 0
	EPwm4Regs.TBCTR = 0x0000;                      // Clear counter

	// Set Compare values
	EPwm4Regs.CMPA.half.CMPA = Initial_Value_Of_CMPA;     // Set compare A value
	EPwm4Regs.CMPB = Initial_Value_Of_CMPA;               // Set Compare B value

	// Setup counter mode
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Enable phase loading
	EPwm4Regs.TBCTL.bit.PHSDIR = TB_COUNT_UP;
	EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0;       // Clock ratio to SYSCLKOUT
	EPwm4Regs.TBCTL.bit.CLKDIV = 0;
	EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
	// EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;

	// Setup shadowing
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	// Set actions
	EPwm4Regs.AQCTLA.bit.CAU = AQ_SET; // Set PWM2A on event A, up count             // Set PWM1A on event A, up count
	EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;

	EPwm4Regs.DBCTL.bit.IN_MODE = 0x00;
	EPwm4Regs.DBCTL.bit.OUT_MODE = 0x03;
	EPwm4Regs.DBCTL.bit.POLSEL = 0x00;
	EPwm4Regs.DBCTL.bit.HALFCYCLE = 0x00;
	EPwm4Regs.DBRED = DEAD_TIME;
	EPwm4Regs.DBFED = DEAD_TIME;

}


void PIvalue(){

	v1 = Expectative_Value; //1126 ; //;
			v2 = 0;
			if (TempVoValue > 0) {
				VoValue[ADCCount] = TempVoValue;
			} else {
				VoValue[ADCCount] = TempVoValue * (-1);
			}
			for (i = 0; i < 256; i++) {
				v2 += VoValue[i];
			}

			v2 = ((long int) (v2) >> 8);

			verr = v1 - v2;
			vk2 = verr + ZForward;
			ZForward = verr;
			vko2 = (long int) (vk2 * (kI * 2048)); //( vk2 * kI );
			vou2 = vko2 + (ZBackward); //<< 11
			if (vou2 >= ((long int) Half_FREQ_TBPRD << 11)) {
				vou2 = ((long int) Half_FREQ_TBPRD << 11);
			} else if (vou2 <= ((-1) * ((long int) Half_FREQ_TBPRD << 11))) /////////////////////// 變頻要改
					{
				vou2 = ((-1) * ((long int) Half_FREQ_TBPRD << 11));
			}

			ZBackward = vou2; //>> 11
			vk1 = verr;
			vko1 = (long int) (vk1 * kP * 2048); //(vk1 * kP) ;
			vout = 300;   //(int)( ( vou2 + vko1) >> 11 );
			if (vout >= Half_FREQ_TBPRD) {					///////// 變頻要改
				vout = Half_FREQ_TBPRD;
			} else if (vout <= ((-1) * Half_FREQ_TBPRD)) {
				vout = (-1) * Half_FREQ_TBPRD;
			}

			IoCalVlue = (int) (IoValue[ADCCount] * 0.01) << 9;

			CMPAValue = 300 ; //Half_FREQ_TBPRD+ (((long int) (vout * (sintable1[ADCCount] * (-1))- IoCalVlue)) >> 9);

			EALLOW;
			GpioDataRegs.GPASET.bit.GPIO18 = 1;

			EDIS;

			if (ADCCount != 255)
				ADCCount++;
			else
				ADCCount = 0;
}

__interrupt void cpu_timer0_isr(void) {
	DELAY_US(10);
	TempVoValue = AdcResult.ADCRESULT0 - 2048 + 75;
	TempIoValue = AdcResult.ADCRESULT1 - 2048;
	// Configure protect
	if (TempVoValue >= High_Bounded_Vo_Value - 2048) { //|| TempIoValue >= High_Bounded_Io_Value
		EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;
		EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;
		while (1)
			;
	}

	// Detect the Epwm Switch

	// Detect the ADC Switch
	if (GpioDataRegs.GPADAT.bit.GPIO26 == 1) {
		PIvalue();
	} else {
		EALLOW;
		GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
		EDIS;
		for (i = 0; i < 256; i++) {
			VoValue[i] = 0;
			IoValue[i] = 0;
		}
		ZForward = 0;
		ZBackward = 0;
	}
	if (GpioDataRegs.GPADAT.bit.GPIO24 == 1) {
		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
		EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;
		EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;
		EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;
		EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;

		EPwm3Regs.DBCTL.bit.POLSEL = 0x01;
		EPwm4Regs.DBCTL.bit.POLSEL = 0x01;

		EPwm3Regs.CMPA.half.CMPA = CMPAValue; //Half_FREQ_TBPRD	;
		EPwm4Regs.CMPA.half.CMPA = CMPAValue; //Half_FREQ_TBPRD	;

		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
		EALLOW;
		GpioDataRegs.GPASET.bit.GPIO20 = 1;
		EDIS;
		for (i = 0; i < 256; i++) {
			VoValue[i] = 0;
			IoValue[i] = 0;
		}
		ZForward = 0;
		ZBackward = 0;
	} else {
		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
		EPwm3Regs.CMPA.half.CMPA = FREQ_TBPRD; //Half_FREQ_TBPRD	;
		EPwm4Regs.CMPA.half.CMPA = FREQ_TBPRD; //Half_FREQ_TBPRD	;

		EPwm3Regs.DBCTL.bit.POLSEL = 0x00;
		EPwm4Regs.DBCTL.bit.POLSEL = 0x00;

		SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
		EALLOW;
		GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
		EDIS;
		for (i = 0; i < 256; i++) {
			VoValue[i] = 0;
			IoValue[i] = 0;
		}
		ZForward = 0;
		ZBackward = 0;
	}

	if (GpioDataRegs.GPADAT.bit.GPIO26 == 1 && GpioDataRegs.GPADAT.bit.GPIO24 == 1) {
		EALLOW;
		if (LED_DELAY == 1000) {
			GpioDataRegs.GPATOGGLE.bit.GPIO22 = 1;
			LED_DELAY = 0;
		} else
			LED_DELAY++;
		EDIS;
	} else {
		EALLOW;
		GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
		EDIS;
	}

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//===========================================================================
// No more.
//===========================================================================

