#include <xinu.h>

//sid32 readSem;			// Read semaphore

struct	adccblk	adctab[Ngpio];

#define CM_PER                                (0x44E00000)		
#define CM_PER_L3S_CLKSTCTRL                  (0x44E00004)
#define CM_PER_L3_CLKSTCTRL                   (0x44E0000C)
#define CM_PER_L3_INSTR_CLKCTRL               (0x44E000DC)
#define CM_PER_L3_CLKCTRL                     (0x44E000E0)
#define CM_PER_OCPWP_L3_CLKSTCTRL             (0x44E0012C)

#define CM_WKUP                               (0x44E00400)		
#define CM_WKUP_CLKSTCTRL                     (0x44E00400)
#define CM_WKUP_CONTROL_CLKCTRL               (0x44E00404)
#define CM_WKUP_L4WKUP_CLKCTRL                (0x44E0040C)
#define CM_L3_AON_CLKSTCTRL                   (0x44E00418)
#define CM_WKUP_ADC_TSC_CLKCTRL               (0x44E004BC)
#define CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL      (0x44E004CC)

//Interrupt controller register (part)
#define INTERRUPT_CONTROLLER                  (0x48200000)
#define INTC_THRESHOLD                        (0x48200068)

//MACRO to read and write register
#define HWREG(n)	(*((volatile unsigned int *)(n)))


void InitialL4Clock()
{


	HWREG(CM_WKUP_CONTROL_CLKCTRL) |= 0x02;                             //enbale wkup control clk
	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & 0x03 ) != 0x02 );          //wait 

	HWREG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   //enbale wkup st clk
	while(( HWREG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                //wait 

	HWREG(CM_WKUP_CLKSTCTRL) |= 0x02;                                   //enbale wkup st clk
	while(( HWREG(CM_WKUP_CLKSTCTRL) & 0x03 ) != 0x02 );                //wait

	HWREG(CM_L3_AON_CLKSTCTRL) |= 0x02;                                 //enbale wkup st clk
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & 0x03 ) != 0x02 );              //wait

	HWREG(CM_WKUP_ADC_TSC_CLKCTRL) |= 0x02;                             //enbale wkup st clk
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & 0x03 ) != 0x02 );          //wait

	while(( HWREG(CM_WKUP_CONTROL_CLKCTRL) & (0x03<<16) ) != 0x00 );            //wait wkup control clk fully functional
	while(( HWREG(CM_L3_AON_CLKSTCTRL) & (0x01<<3) ) == 0x00 );                 //wait L3 AON gclk active
	while(( HWREG(CM_WKUP_L4WKUP_CLKCTRL) & (0x03<<16) ) != 0x00 );             //wait L4 WKUP CLK fully functional
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<2) ) == 0x00 );                   //wait L4 WKUP GCLK active
	while(( HWREG(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) & (0x01<<2) ) == 0x00 );    //wait L4 WKUP aon CLK active
	while(( HWREG(CM_WKUP_CLKSTCTRL) & (0x01<<14) ) == 0x00 );                  //wait ADC fclk active
	while(( HWREG(CM_WKUP_ADC_TSC_CLKCTRL) & (0x03<<16) ) != 0x00 );            //wait ADC module fully functinoal
}


void adc_step_enable(struct adc_csreg *aReg, unsigned int selStep)
{
	aReg->stepEnable |= (0x01<<selStep);
}


void adc_step_disable(struct adc_csreg *aReg, unsigned int selStep)
{
	aReg->stepEnable &= ~(0x01<<selStep);
}

void ADCStepConfig(struct adc_csreg *pReg, unsigned int stepSelect, 
					unsigned int positiveChannel,unsigned int positiveRef,
					unsigned int negativeChannel,unsigned int negativeRef)
{
	//configure the nagtive reference
	pReg->step[stepSelect].stepConfig &= ~ SEL_RFM_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= negativeRef<<SEL_RFM_SWC_SHIFT;     //write

	//configure the positive input channel
	pReg->step[stepSelect].stepConfig &= ~ SEL_INP_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= positiveChannel<<SEL_INP_SWC_SHIFT; //write

	//configure the negative input channel,
	pReg->step[stepSelect].stepConfig &= ~ SEL_INM_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= negativeChannel<<SEL_INM_SWC_SHIFT; //write

	//configure the positive reference
	pReg->step[stepSelect].stepConfig &= ~ SEL_RFP_SWC_CLEAR;                 //clear
	pReg->step[stepSelect].stepConfig |= positiveRef<<SEL_RFP_SWC_SHIFT;      //write
}


void adc_interrupt_init(struct	dentry *devptr)
{
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;   
	csrptr->threshold = 0X0FF;                                     
	set_evec(devptr->dvirq, (uint32)devptr->dvintr);                
	csrptr->ilr[devptr->dvirq] &= ~(0x01);                         
	csrptr->ilr[devptr->dvirq] |= (0x0A<<2);                    	 

}


devcall	adcinit (struct	dentry *devptr)
{
	//kprintf("adc init start running\n");


	struct adc_csreg* pReg = (struct adc_csreg*)devptr->dvcsr; //get the register start address

	//create a semaphore, when interrupt finish reading, it will add sem by one, and read function
	//will wait this semaphore
	unsigned int *reg = NULL;
 	struct	adccblk	*adcptr;
 	adcptr = &adctab[ devptr->dvminor ];
 	//adcptr->sem = semcreate(0);	 // remove
 	//Setup clk divison for ADC

 	InitialL4Clock();

	adc_interrupt_init(devptr);

	adcptr->adchead = adcptr->adctail = 	/* Set up input queue	*/
	&adcptr->adcbuf[0];		/*    as empty		*/
	adcptr->sem = semcreate(0);		/* Input semaphore	*/

//BBB bbb1
	adcptr->open_dly = 0;
	adcptr->sample_dly = 1;
 	unsigned int clkDiv = 24000000/adcptr->freq - 1;  ///3 MHZ frequency
	pReg->adcClkDiv &= ~(0xFFFF);
	pReg->adcClkDiv |= clkDiv;

	//write protect off
	pReg->ctrl &= ~ (0x01<<2);
	pReg->ctrl |= (1<<2);

	pReg->step[0].stepConfig &= ~ADC_STEPCONFIG_DIFF_CNTRL;

	//ADCStepConfig(pReg, 0, ADC_CHANNEL1, ADC_POSITIVE_REF_VDDA, ADC_CHANNEL1, ADC_NEGATIVE_REF_VSSA);
	//configure the nagtive reference
	pReg->step[0].stepConfig &= ~ SEL_RFM_SWC_CLEAR;                //clear
	pReg->step[0].stepConfig |= ADC_NEGATIVE_REF_VSSA<<SEL_RFM_SWC_SHIFT;     //write

	//configure the positive input channel
	pReg->step[0].stepConfig &= ~ SEL_INP_SWC_CLEAR;                //clear
	pReg->step[0].stepConfig |= adcptr->chn_ID<<SEL_INP_SWC_SHIFT; //write

	//configure the negative input channel,
	pReg->step[0].stepConfig &= ~ SEL_INM_SWC_CLEAR;                //clear
	pReg->step[0].stepConfig |= adcptr->chn_ID<<SEL_INM_SWC_SHIFT; //write

	//configure the positive reference
	pReg->step[0].stepConfig &= ~ SEL_RFP_SWC_CLEAR;                 //clear
	pReg->step[0].stepConfig |= ADC_POSITIVE_REF_VDDA<<SEL_RFP_SWC_SHIFT;      //write


	pReg->step[0].stepConfig &= ~ ADC_STEPCONFIG_XPPSW_SWC_CLEAR;
	pReg->step[0].stepConfig |= 0<<ADC_STEPCONFIG_XPPSW_SWC_SHIFT;

	pReg->step[0].stepConfig &= ~ ADC_STEPCONFIG_XNPSW_SWC_CLEAR;
	pReg->step[0].stepConfig |= 0<<ADC_STEPCONFIG_XNPSW_SWC_SHIFT;

	pReg->step[0].stepConfig &= ~ ADC_STEPCONFIG_YPPSW_SWC_CLEAR;
	pReg->step[0].stepConfig |= 0<<ADC_STEPCONFIG_YPPSW_SWC_SHIFT;



	pReg->step[0].stepConfig &= ~ ADC_STEPCONFIG_XNNSW_SWC_CLEAR;
	pReg->step[0].stepConfig |=  0<<ADC_STEPCONFIG_XNNSW_SWC_SHIFT;

	pReg->step[0].stepConfig &= ~ ADC_STEPCONFIG_YPNSW_SWC_CLEAR;
	pReg->step[0].stepConfig |=  0<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	pReg->step[0].stepConfig &= ~ ADC_STEPCONFIG_YNNSW_SWC_CLEAR;
	pReg->step[0].stepConfig |=  0<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	pReg->step[0].stepConfig &= ~ ADC_STEPCONFIG_WPNSW_SWC_CLEAR;
	pReg->step[0].stepConfig |=  0<<ADC_STEPCONFIG_WPNSW_SWC_SHIFT;

	pReg->step[0].stepConfig &= ~ ADC_FIFO_SELECTION_CLEAR;
	pReg->step[0].stepConfig |= ADC_FIFO_0<<ADC_FIFO_SELECTION_SHIFT;

	pReg->step[0].stepConfig &= ~ ADC_MODE_CLEAR;
	pReg->step[0].stepConfig |= ADC_MODE_SW_CONTINUOUS<<ADC_MODE_SHIFT;


	if(adcptr->sample_rate == 2)
		pReg->step[0].stepConfig |= 0x01<<2;
	else if(adcptr->sample_rate == 4)
		pReg->step[0].stepConfig |= 0x02<<2;
	else if(adcptr->sample_rate == 8)
		pReg->step[0].stepConfig |= 0x03<<2;	
	else if(adcptr->sample_rate == 16)
		pReg->step[0].stepConfig |= 0x04<<2;

	pReg->irqStatus |= 0x7FFF;

	pReg->irqEnableSet |=ADC_END_OF_SEQUENCE_INTR;

	pReg->ctrl |= 0x01;	

	
	return 1;
}