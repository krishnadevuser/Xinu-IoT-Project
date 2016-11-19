
#include <xinu.h>



/* Argument define */

/* Beaglebone black ADC have 7 AIN (0~6) */
#define ADCTSC_AIN_COUNT	7


/* Device register mamory map */
#define ADCTSC_MMAP_ADDR	0x44E0D000
#define ADCTSC_MMAP_LEN	0x2000

/* ADCRANGE operator code */
#define ADCRANGE_MAX_RANGE	0xFFF
#define ADCRANGE_MIN_RANGE	0x000

/* CTRL operator code */
#define CTRL_ENABLE	0x1
#define CTRL_STEP_ID_TAG	0x2


#define BBBIO_CM_PER_ADDR		0x44e00000
#define BBBIO_CM_PER_LEN		0x4000


#define BBBIO_ADC_AIN0	0
#define BBBIO_ADC_AIN1	1
#define BBBIO_ADC_AIN2	2
#define BBBIO_ADC_AIN3	3
#define BBBIO_ADC_AIN4	4
#define BBBIO_ADC_AIN5	5
#define BBBIO_ADC_AIN6	6


/* step config control operator*/
#define BBBIO_ADC_STEP_MODE_SW_ONE_SHOOT	0x0
#define BBBIO_ADC_STEP_MODE_SW_CONTINUOUS	0x1
#define BBBIO_ADC_STEP_MODE_HW_ONE_SHOOT	0x2
#define BBBIO_ADC_STEP_MODE_HW_CONTINUOUS	0x3

#define BBBIO_ADC_STEP_AVG_1	0x0
#define BBBIO_ADC_STEP_AVG_2	0x1
#define BBBIO_ADC_STEP_AVG_4	0x2
#define BBBIO_ADC_STEP_AVG_8	0x3
#define BBBIO_ADC_STEP_AVG_16	0x4

#define BBBIO_ADC_WORK_MODE_BUSY_POLLING	0x1
#define BBBIO_ADC_WORK_MODE_TIMER_INT	0x2


/* ----------------------------------------------------------------------
 * Clock Module Wakeup Registers
 *	@Source : AM335x Technical Reference Manual ,page 976 , Table 8-88. CM_WKUP REGISTERS
 *
*/

#define BBBIO_CM_WKUP_ADDR 			0x44e00400
#define BBBIO_CM_WKUP_OFFSET_FROM_CM_PER	0x400		/* for mapping alignment . BBBIO_CM_WKUP_ADDR is not aligned of page boundary in 4k page .*/
#define BBBIO_CM_WKUP_LEN 			0x100

/* register offset */
#define BBBIO_CM_WKUP_GPIO0_CLKCTRL 		0x8
#define BBBIO_CM_WKUP_ADC_TSC_CLKCTRL		0xBC

struct ADCTSC_struct ADCTSC ;

struct ADCTSC_FIFO_struct
{
	unsigned int *reg_count ;
	unsigned int *reg_data ;
	struct ADCTSC_FIFO_struct *next ;
};

struct ADCTSC_channel_struct
{
	unsigned int enable ;	 /* HW channel en/disable,  */
	unsigned int mode ;
	unsigned int FIFO ;
	/*  13 + O + S cycle per sample, 13 is ADC converting time, O is open delay, S is sample delay .
	 *	Open delay  minmum : 0
	 *	Sample delay  minmum : 1
	 */
	unsigned int delay ;	/* bit 0~17 open delay , bit 24~31 sample delay  */

	/* channel buffer */
	unsigned int *buffer ;
	unsigned int buffer_size ;
	unsigned int buffer_count ;
	unsigned int *buffer_save_ptr ;
};

struct ADCTSC_struct
{
	unsigned int work_mode ;
	unsigned int H_range;
	unsigned int L_range;
	unsigned int ClockDiv;	/* Clock divider , Default ADC clock :24MHz */
	struct ADCTSC_channel_struct channel[8];
	struct ADCTSC_FIFO_struct FIFO[2] ;
	unsigned char channel_en ;	/* Const SW channel en/disable, not real channel en/disable */
	unsigned char channel_en_var;	/* Variable  channel SW enable , for access */
	int fetch_size;
};





struct	adccblk	adctab[Nadc];


int adc_channel_status(volatile uint32	*st_en, int chn_ID , int enable)
{
	//unsigned int *reg = NULL;

	if((chn_ID < 0) || (chn_ID > 6)) {
		return 0;
	}
	else {
		/* step enable */
		if(enable) {
			*st_en |= 0x0001 << (chn_ID+1);
		}
		else {
			*st_en &= ~(0x0001 << (chn_ID+1));
		}
	}
	return 1;
}

/*------------------------------------------------------------------------
 *  adcinit  -  Initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
devcall	adcinit(
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	struct	adccblk	*adcptr;		/* Pointer to ttytab entry	*/
	struct	adc_csreg *aptr;	/* Address of UART's CSRs	*/
	unsigned int *reg = NULL;
	//unsigned int FIFO_count = 0;
	unsigned int FIFO_data = 0;
	int i =0;
	return OK;
	adcptr = &adctab[ devptr->dvminor ];

	//////////////////////////
	adcptr->adchead = adcptr->adctail = 	/* Set up input queue	*/
	&adcptr->adcbuf[0];		/*    as empty		*/
	adcptr->sem = semcreate(0);		/* Input semaphore	*/
	adcptr->clkdiv = 160 ;
	adcptr->chn_ID = 1 ;
	adcptr->open_dly = 0;
	adcptr->sample_dly = 1;
	adcptr->mode = BBBIO_ADC_STEP_MODE_SW_CONTINUOUS;
	adcptr->sample_avg = BBBIO_ADC_STEP_AVG_1;

	//adcptr->work_mode == 1;
	
	///////////////////////////////

	aptr = (struct adc_csreg *)devptr->dvcsr;

	reg = (void *)BBBIO_CM_PER_ADDR + BBBIO_CM_WKUP_OFFSET_FROM_CM_PER +  BBBIO_CM_WKUP_ADC_TSC_CLKCTRL;
	*reg = 0x2 ;

	aptr->ctrl &= ~0x1 ;
	if((adcptr->clkdiv < 1) || (adcptr->clkdiv > 65535)) {
		aptr->clkdiv = 1;
	}
	else {
		adcptr->clkdiv -= 1 ;
	}

	aptr->range |= (adcptr->L_range | adcptr->H_range << 16) ;

	/*if((adcptr->chn_ID > BBBIO_ADC_AIN6) || (adcptr->chn_ID < BBBIO_ADC_AIN0) ||
	   (adcptr->sample_avg > BBBIO_ADC_STEP_AVG_16) || (adcptr->sample_avg < BBBIO_ADC_STEP_AVG_1) ||
	   (adcptr->open_dly > 262143) || (adcptr->open_dly < 0) || (adcptr->sample_dly > 255) || (adcptr->sample_dly < 1)){
		return SYSERR;
	}*/

	/* Disable channel step*/
	aptr->st_en &= ~(0x0001 << (adcptr->chn_ID+1));

	/* cancel step config register protection*/
	aptr->ctrl |= 0x4 ;

	/* set step config */
	
	reg = (void *)(&aptr->st_cnf1 + (adcptr->chn_ID * 0x8));
	*reg &= ~(0x1F) ;	/* pre-maks Mode filed */
	
	*reg |= (adcptr->mode | (adcptr->sample_avg << 2) | (adcptr->chn_ID << 19) | (adcptr->chn_ID << 15) | ((adcptr->chn_ID % 2) << 26) );
	
	/* set open delay */
	if(adcptr->open_dly <0 || adcptr->open_dly >262143) {
		adcptr->open_dly = 0;
	}

	reg = (void *)(&aptr->st_dly1 + adcptr->chn_ID * 0x8);
	*reg =0;
	*reg |= ((adcptr->sample_dly - 1) << 24 | adcptr->open_dly);
	if (adcptr->work_mode == 1){
		/*if(devptr==NULL)
			kprintf("devptr =NULLL \n");
		if(devptr->dvintr==NULL) kprintf(" pointer is null");
		if (devptr && devptr->dvintr)
			kprintf("devptr->dvirq = %x , devptr->dvirq = %u",devptr->dvintr, (uint32)devptr->dvirq);*/
		//set_evec( devptr->dvirq, (uint32)devptr->dvintr );
   		aptr->irq_e_set = ADC_IIR_IRQ | ADC_ASYNC | ADC_F0_OVERRUN;
	}

	/* resume step config register protection*/
	aptr->ctrl &= ~0x4 ;


	for(i = 0 ; i < aptr->fifo_cnt ; i++) {
		FIFO_data =(unsigned int ) aptr->fifo_0_data;
	}

    for(i = 0 ; i < aptr->fifo_1_cnt ; i++) {
		FIFO_data =(unsigned int ) aptr->fifo_1_data;
    }


	/* Initialize values in the adc control block */
	return OK;
}


