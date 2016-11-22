
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

 /* Device register offset */
#define ADCTSC_REVISION	0x0
#define ADCTSC_SYSCONFIG	0x10
#define ADCTSC_IRQSTATUS_RAW	0x24
#define ADCTSC_IRQSTATUS	0x28
#define ADCTSC_IRQENABLE_SET	0x2C
#define ADCTSC_IRQENABLE_CLR	0x30
#define ADCTSC_IRQWAKEUP	0x34
#define ADCTSC_DMAENABLE_SET	0x38
#define ADCTSC_DMAENABLE_CLR	0x3C
#define ADCTSC_CTRL	0x40
#define ADCTSC_ADCSTAT	0x44
#define ADCTSC_ADCRANGE	0x48
#define ADCTSC_ADC_CLKDIV	0x4C
#define ADCTSC_ADC_MISC	0x50
#define ADCTSC_STEPENABLE	0x54
#define ADCTSC_IDLECONFIG	0x58
#define ADCTSC_TS_CHARGE_STEPCONFIG	0x5C
#define ADCTSC_TS_CHARGE_DELAY	0x60
#define ADCTSC_STEPCONFIG1	0x64
#define ADCTSC_STEPDELAY1	0x68
#define ADCTSC_STEPCONFIG2	0x6C
#define ADCTSC_STEPDELAY2	0x70
#define ADCTSC_STEPCONFIG3	0x74
#define ADCTSC_STEPDELAY3	0x78
#define ADCTSC_STEPCONFIG4	0x7C
#define ADCTSC_STEPDELAY4	0x80
#define ADCTSC_STEPCONFIG5	0x84
#define ADCTSC_STEPDELAY5	0x88
#define ADCTSC_STEPCONFIG6	0x8C
#define ADCTSC_STEPDELAY6	0x90
#define ADCTSC_STEPCONFIG7	0x94
#define ADCTSC_STEPDELAY7	0x98
#define ADCTSC_STEPCONFIG8	0x9C
#define ADCTSC_STEPDELAY8	0xA0
#define ADCTSC_STEPCONFIG9	0xA4
#define ADCTSC_STEPDELAY9	0xA8
#define ADCTSC_STEPCONFIG10	0xAC
#define ADCTSC_STEPDELAY10	0xB0
#define ADCTSC_STEPCONFIG11	0xB4
#define ADCTSC_STEPDELAY11	0xB8
#define ADCTSC_STEPCONFIG12	0xBC
#define ADCTSC_STEPDELAY12	0xC0
#define ADCTSC_STEPCONFIG13	0xC4
#define ADCTSC_STEPDELAY13	0xC8
#define ADCTSC_STEPCONFIG14	0xCC
#define ADCTSC_STEPDELAY14	0xD0
#define ADCTSC_STEPCONFIG15	0xD4
#define ADCTSC_STEPDELAY15	0xD8
#define ADCTSC_STEPCONFIG16	0xDC
#define ADCTSC_STEPDELAY16	0xE0
#define ADCTSC_FIFO0COUNT	0xE4
#define ADCTSC_FIFO0THRESHOLD	0xE8
#define ADCTSC_DMA0REQ	0xEC
#define ADCTSC_FIFO1COUNT	0xF0
#define ADCTSC_FIFO1THRESHOLD	0xF4
#define ADCTSC_DMA1REQ	0xF8
#define ADCTSC_FIFO0DATA	0x100
#define ADCTSC_FIFO1DATA	0x200

/* register offset */
#define BBBIO_CM_WKUP_GPIO0_CLKCTRL 		0x8
#define BBBIO_CM_WKUP_ADC_TSC_CLKCTRL		0xBC
#define _IDLEST_MASK  0x03<<16
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


int adc_channel_status(struct	adc_csreg *aptr, int chn_ID , int enable)
{
	unsigned int *reg = NULL;
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_STEPENABLE;
	if((chn_ID < 0) || (chn_ID > 6)) {
		return 0;
	}
	else {
		/* step enable */
		if(enable) {
			*reg |= 0x0001 << (chn_ID+1);
		}
		else {
			*reg &= ~(0x0001 << (chn_ID+1));
		}
	}
	return 1;
}

int adc_channel_enable( int chn_ID ,struct	adccblk	*adcptr, struct	adc_csreg *aptr){
	/* Disable channel step*/
	unsigned int *reg = NULL;
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_STEPENABLE;//&aptr->st_en;
	*reg &= ~(0x0001 << (chn_ID+1));
	//aptr->st_en = 1 << (adcptr->chn_ID+1);

	/* cancel step config register protection*/
	//aptr->ctrl |= 0x4 ;
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;//&aptr->ctrl;
	*reg  |= 0x4 ;

	/* set step config */
	//kprintf("ADC init 5\n");
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_STEPCONFIG1+ (chn_ID * 0x8);//(&aptr->st_cnf1 + (chn_ID * 0x8));
	*reg &= ~(0x1F) ;	/* pre-maks Mode filed */
	
	*reg |= (adcptr->mode | (adcptr->sample_avg << 2) | (chn_ID << 19) | (chn_ID << 15) | ((chn_ID % 2) << 26) );
	
	/* set open delay */
	if(adcptr->open_dly <0 || adcptr->open_dly >262143) {
		adcptr->open_dly = 0;
	}
	//kprintf("ADC init 6\n");
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_STEPDELAY1+chn_ID * 0x8;//(&aptr->st_dly1 + chn_ID * 0x8);
	*reg =0;
	*reg |= ((adcptr->sample_dly - 1) << 24 | adcptr->open_dly);
	if (adcptr->work_mode == 1){
		//set_evec( devptr->dvirq, (uint32)devptr->dvintr );
   		//aptr->irq_e_set = ADC_IIR_IRQ | ADC_ASYNC | ADC_F0_OVERRUN;
	}

	/* resume step config register protection*/
	//aptr->ctrl &=  ~0x4 ;
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;//&aptr->ctrl;
	*reg  &=  ~0x4 ;
}



void ADCTSC_module_ctrl( struct	adccblk	*adcptr, struct	adc_csreg *aptr){
	unsigned int *reg = NULL;

	if((adcptr->clkdiv < 1) || (adcptr->clkdiv > 65535)) {
		aptr->clkdiv = 0x1;
	}
	else {
		//aptr->clkdiv = adcptr->clkdiv - 0x1 ;
		reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_ADC_CLKDIV;//&aptr->clkdiv;
		*reg = (adcptr->clkdiv  -1) ;
	}

	adc_channel_enable(adcptr->chn_ID,adcptr,aptr);
}


#define ADC_STEPCONFIG_WRITE_PROTECT_OFF (0x01<<2)
#define ADC_TSC (0x44E0D000)
#define ADCSTEPCONFIG1 (ADC_TSC+0x64)
#define ADCSTEPDELAY1  (ADC_TSC+0x68)
#define ADCSTEPCONFIG2 (ADC_TSC+0x6C)
#define ADCSTEPDELAY2  (ADC_TSC+0x70)
#define ADCSTEPCONFIG3 (ADC_TSC+0x74)
#define ADCSTEPDELAY3  (ADC_TSC+0x78)
#define ADCSTEPCONFIG4 (ADC_TSC+0x7C)
#define ADCSTEPDELAY4  (ADC_TSC+0x80)
#define ADCSTEPCONFIG5 (ADC_TSC+0x84)
#define ADCSTEPDELAY5  (ADC_TSC+0x88)
#define ADCSTEPCONFIG6 (ADC_TSC+0x8C)
#define ADCSTEPDELAY6  (ADC_TSC+0x90)
#define ADCSTEPCONFIG7 (ADC_TSC+0x94)
#define ADCSTEPDELAY7  (ADC_TSC+0x98)
#define ADCSTEPCONFIG8 (ADC_TSC+0x9C)
#define ADCSTEPDELAY8  (ADC_TSC+0xA0)
#define ADC_FIFO0DATA (ADC_TSC+0x100)
#define ADC_FIFO_MASK (0xFFF)

#define ADC_CTRL (ADC_TSC+0x40)
#define ADC_STEPCONFIG_WRITE_PROTECT_OFF (0x01<<2)
#define ADC_STEPENABLE (ADC_TSC+0x54)


#define CM_WKUP (0x44E00400)
#define CM_WKUP_ADC_TSC_CLKCTRL (CM_WKUP+0xBC)
#define CM_WKUP_MODULEMODE_ENABLE (0x02)
#define CM_WKUP_IDLEST_DISABLED (0x03<<16)
#define ADC_AVG16 (0x100) 



void adc_init(){
 	unsigned int *reg = NULL;
 	kprintf("ADC init 3\n");
	//reg = (void *)CM_WKUP_ADC_TSC_CLKCTRL;
	/*
	*reg |= 0x2 ;

	 while(!(*reg & 0x2)){

	 }*/
	 reg = (void *)BBBIO_CM_WKUP_ADDR   +  BBBIO_CM_WKUP_ADC_TSC_CLKCTRL;
	*reg = 0x2 ;

	 while(*reg & _IDLEST_MASK);
	 *reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;//&aptr->ctrl;
	*reg  |= ADC_STEPCONFIG_WRITE_PROTECT_OFF;

	reg = (void *)ADCSTEPCONFIG1;
	*reg = 0x00<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY1;
	*reg = (0x0F)<<24;
	reg = (void *)ADCSTEPCONFIG2;
	*reg  = 0x01<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY2; 
	*reg  = (0x0F)<<24;
	reg = (void *)ADCSTEPCONFIG3;
	*reg = 0x02<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY3; 
	*reg  = (0x0F)<<24;
	reg = (void *)ADCSTEPCONFIG4;
	*reg = 0x03<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY4;
	*reg = (0x0F)<<24;
	reg = (void *)ADCSTEPCONFIG5;
	*reg  = 0x04<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY5;
	*reg = (0x0F)<<24;
	reg = (void *)ADCSTEPCONFIG6;
	*reg = 0x05<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY6; 
	*reg = (0x0F)<<24;
	reg = (void *)ADCSTEPCONFIG7; 
	*reg = 0x06<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY7;
	*reg  = (0x0F)<<24;
	reg = (void *)ADCSTEPCONFIG8;
	*reg  = 0x07<<19 | ADC_AVG16;
	reg = (void *)ADCSTEPDELAY8;
	*reg = (0x0F)<<24;


	*reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;//&aptr->ctrl;
	*reg  |= 0x01;

 }



int analogRead() {
	unsigned int *reg = NULL;
	
	// the clock module is not enabled
	reg = (void *)CM_WKUP_ADC_TSC_CLKCTRL;
	if(*reg & CM_WKUP_IDLEST_DISABLED)
		adc_init();
	
	// enable the step sequencer for this pin
	reg = (void *)ADC_STEPENABLE;
	*reg  |= (0x01<<(1+1));

	// return the the FIFO0 data register
	reg = (void *)ADC_FIFO0DATA;
	return * reg & ADC_FIFO_MASK;
}
/*------------------------------------------------------------------------
 *  adcinit  -  Initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
devcall	adcinit(
	  struct dentry	*devptr		
	)
{
	//adc_init();
	return OK;
	struct	adccblk	*adcptr;		
	struct	adc_csreg *aptr;	
	unsigned int *reg = NULL;
	//unsigned int FIFO_count = 0;
	unsigned int FIFO_data = 0;
	int i =0;
	kprintf("ADC init starts\n");
	adcptr = &adctab[ devptr->dvminor ];
	kprintf("ADC init 1\n");
	//////////////////////////
	adcptr->adchead = adcptr->adctail = 	/* Set up input queue	*/
	&adcptr->adcbuf[0];		/*    as empty		*/
	adcptr->sem = semcreate(0);		/* Input semaphore	*/
	adcptr->clkdiv = 1 ;
	adcptr->open_dly = 0;
	adcptr->sample_dly = 1;
	adcptr->mode = BBBIO_ADC_STEP_MODE_SW_CONTINUOUS;
	adcptr->sample_avg = BBBIO_ADC_STEP_AVG_1;
	adcptr->L_range = ADCRANGE_MIN_RANGE;
	adcptr->H_range = ADCRANGE_MAX_RANGE;
	kprintf("ADC init 2\n");
	//adcptr->work_mode == 1;
	
	///////////////////////////////

	aptr = (struct adc_csreg *)devptr->dvcsr;
	
	kprintf("ADC init 3\n");
	reg = (void *)BBBIO_CM_WKUP_ADDR   +  BBBIO_CM_WKUP_ADC_TSC_CLKCTRL;
	*reg = 0x2 ;

	 while(*reg & _IDLEST_MASK);
	//kprintf("ADC init aptr->ctrl=%u\n",aptr->ctrl);
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;//&aptr->ctrl;
	*reg  &= ~0x1;
	kprintf("ADC init 3.5\n");
	if((adcptr->clkdiv < 1) || (adcptr->clkdiv > 65535)) {
		aptr->clkdiv = 0x1;
	}
	else {
		//aptr->clkdiv = adcptr->clkdiv - 0x1 ;
		reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_ADC_CLKDIV;//&aptr->clkdiv;
		*reg = (adcptr->clkdiv  -1) ;
	}
	kprintf("ADC init 4\n");
	reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_ADCRANGE;//&aptr->range;
	*reg |= (adcptr->L_range | adcptr->H_range << 16) ;
	aptr->range |= (adcptr->L_range | adcptr->H_range << 16) ;



	for(i = 0 ; i < 7; i++) {
		adc_channel_enable(0,adcptr,aptr);
   }
   //reg = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;//&aptr->ctrl;
	//*reg  &= 0x01 ;
   unsigned int FIFO_count = 0;
   FIFO_count = *((unsigned int*)((void *)ADCTSC_MMAP_ADDR + ADCTSC_FIFO0COUNT));
	
	kprintf("ADC init 7 = %u aptr->fifo_cnt=%x   aptr->fifo_0_data=%u\n",FIFO_count, &aptr->fifo_cnt,aptr->fifo_0_data);
	kprintf("data value = %f",((float)*aptr->fifo_0_data / 4095.0f) * 1.8f);
	for(i = 0 ; i < FIFO_count ; i++) {
		FIFO_data = *((unsigned int*)((void *)ADCTSC_MMAP_ADDR + ADCTSC_FIFO0DATA));
	}
	/*for(i = 0 ; i < aptr->fifo_cnt ; i++) {
		FIFO_data =(unsigned int ) aptr->fifo_0_data;
	}*/

	FIFO_count = *((unsigned int*)((void *)ADCTSC_MMAP_ADDR + ADCTSC_FIFO1COUNT));
	kprintf("ADC init 7 = %u aptr->fifo_cnt=%x   aptr->fifo_0_data=%u\n",FIFO_count, &aptr->fifo_cnt,aptr->fifo_0_data);
     for(i = 0 ; i < FIFO_count ; i++) {
		FIFO_data = *((unsigned int*)((void *)ADCTSC_MMAP_ADDR + ADCTSC_FIFO1DATA));
     }
   /* for(i = 0 ; i < aptr->fifo_1_cnt ; i++) {
		FIFO_data =(unsigned int ) aptr->fifo_1_data;
   }*/

   ///////////////////////////////
   adcptr->clkdiv = 160 ;
	adcptr->chn_ID = 1 ;
	adcptr->open_dly = 0;
	adcptr->sample_dly = 1;
	adcptr->mode = BBBIO_ADC_STEP_MODE_SW_CONTINUOUS;
	adcptr->sample_avg = BBBIO_ADC_STEP_AVG_1;


   ///////////////////////////////


	gpio_adc_in(adcptr,aptr);


	/* Initialize values in the adc control block */
	return OK;
}


