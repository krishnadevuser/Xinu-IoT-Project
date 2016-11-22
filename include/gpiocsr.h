
/* GPIOCSR.h - definitions for trivial file transfer protocol */


struct	gpio_csreg
{
	volatile uint32	rev[4];	/* GPIO_REVISION		*/
	volatile uint32	syscon[4];	/* GPIO_SYSCONFIG		*/
	volatile uint32	eoi;	/* GPIO_EOI	*/
	volatile uint32	irq_s_raw_0;	/* GPIO_IRQSTATUS_RAW_0		*/
	volatile uint32	irq_s_raw_1;	/* GPIO_IRQSTATUS_RAW_1		*/
	volatile uint32	irq_s_0;	/* GPIO_IRQSTATUS_0		*/
	volatile uint32	irq_s_1;	/* GPIO_IRQSTATUS_1		*/
	volatile uint32	irq_s_set_0;	/* GPIO_IRQSTATUS_SET_0		*/
	volatile uint32 irq_s_set_1;  /*GPIO_IRQSTATUS_SET_1 */
	volatile uint32 irq_s_clr_0;/* GPIO_IRQSTATUS_CLR_0		*/
	volatile uint32 irq_s_clr_1;	/* GPIO_IRQSTATUS_CLR_1	*/
	//volatile uint32 irq_waken_0;	/* GPIO_IRQWAKEN_0	*/
	//volatile uint32 irq_waken_1;   /* GPIO_IRQWAKEN_1	*/
	volatile uint32 gap[53];
	volatile uint32 sys_stat[6];  /* GPIO_SYSSTATUS */
	volatile uint32 crtl;
	volatile uint32 oe;
	volatile uint32 data_in;
	volatile uint32 data_out;
	volatile uint32 lvl_det_0;   /* GPIO_LEVELDETECT0 */
	volatile uint32 lvl_det_1;
	volatile uint32 rise_det;  /* GPIO_RISINGDETECT */
	volatile uint32 fall_det;  /* GPIO_FALLINGDETECT */
	volatile uint32 deb;  /* GPIO_DEBOUNCABLE */
	volatile uint32 deb_tm[15];  /* GPIO_DEBOUNCINGTIME */
	volatile uint32 clr_data_out;  /* GPIO_CLEARDATAOUT */
	volatile uint32 set_data_out;  /* GPIO_SETDATAOUT */
};



