/* ADCCSR.h - definitions for trivial file transfer protocol */


struct	adc_csreg
{
	volatile uint32	rev[4];	/* ADC_REVISION		*/
	volatile uint32	syscon[5];	/* ADC_SYSCONFIG		*/
	volatile uint32	irq_s_raw;	/* ADC_IRQSTATUS_RAW		*/
	volatile uint32	irq_s;		/* ADC_IRQSTATUS		*/
	volatile uint32	irq_e_set;	/* IRQENABLE_SET		*/
	volatile uint32	irq_e_clr;	/* IRQENABLE_CLR		*/
	volatile uint32	irq_wake_up;	/* IRQWAKEUP	*/
	volatile uint32 dm_set;  /*DMAENABLE_SET */
	volatile uint32 dm_clr; /* DMAENABLE_CLR		*/
	volatile uint32 ctrl;	/* GPIO_IRQSTATUS_CLR_1	*/
	volatile uint32 stat;	/* GPIO_IRQWAKEN_0	*/
	volatile uint32 range;   /* GPIO_IRQWAKEN_1	*/
	volatile uint32 clkdiv; /* ADC_CLKDIV */
	volatile uint32 misc;  /* ADC_MISC */
	volatile uint32 crtl;
	volatile uint32 st_en;
	volatile uint32 idle_cnf;
	volatile uint32 ts_chg_stcnf;
	volatile uint32 ts_chg_dly;   
	volatile uint32 st_cnf1;
	volatile uint32 st_dly1; 
	volatile uint32 st_cnf2;  
	volatile uint32 st_dly2;  
	volatile uint32 st_cnf3;  
	volatile uint32 st_dly3;  
	volatile uint32 st_cnf4;  
	volatile uint32 st_dly4;
	volatile uint32 st_cnf5;
	volatile uint32 st_dly5;
	volatile uint32 st_cnf6;
	volatile uint32 st_dly6;
	volatile uint32 st_cnf7;
	volatile uint32 st_dly7;
	volatile uint32 st_cnf8;
	volatile uint32 st_dly8;
	volatile uint32 st_cnf9;
	volatile uint32 st_dly9;
	volatile uint32 st_cnf10;
	volatile uint32 st_dly10;
	volatile uint32 st_cnf11;
	volatile uint32 st_dly11;
	volatile uint32 st_cnf12;
	volatile uint32 st_dly12;
	volatile uint32 st_cnf13;
	volatile uint32 st_dly13;
	volatile uint32 st_cnf14;
	volatile uint32 st_dly14;
	volatile uint32 st_cnf15;
	volatile uint32 st_dly15;
	volatile uint32 st_cnf16;
	volatile uint32 st_dly16;
	volatile uint32 fifo_cnt;
	volatile uint32 fifo_thld;
	volatile uint32 dma_0_req;
	volatile uint32 fifo_1_cnt;
	volatile uint32 fifo_1_thld;
	volatile uint32 dma_1_req[2];
	volatile uint32 fifo_0_data[64];
	volatile uint32 fifo_1_data[64];
};



#define ADC_IIR_IRQ	0x01	/* Interrupt pending bit		*/
#define ADC_IIR_IDMASK 0x0E	/* 3-bit field for interrupt ID		*/
#define ADC_F0_OVERRUN 0x08	/* Reset transmit FIFO			*/
#define ADC_ASYNC 0x400	/* Reset transmit FIFO			*/
