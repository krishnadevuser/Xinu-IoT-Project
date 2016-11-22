
#ifndef	Ngpio
#define	Ngpio		4		/* Number of serial tty lines	*/
#endif

#ifndef	Nadc
#define	Nadc		4		/* Number of serial tty lines	*/
#endif



#ifndef	GPIO_BUFLEN
#define	GPIO_BUFLEN	1		/* Num. chars in input queue	*/
#endif

struct	gpiocblk	{			/* Tty line control block	*/
	char	port;		/* Next input char to read	*/
	char	pin;		/* Next slot for arriving char	*/
	char	*gpiohead;		/* Next input char to read	*/
	char	*gpiotail;		/* Next slot for arriving char	*/
	char	gpiobuf[GPIO_BUFLEN];	/* Input buffer (holds one line)*/
	sid32	sem;			/* Input semaphore */
	bool8 	int_mode;
	bool8 	mode;
	
};



#ifndef	ADC_BUFLEN
#define	ADC_BUFLEN	64		/* Num. chars in input queue	*/
#endif


struct	adccblk	{			/* Tty line control block	*/
	char	chn_ID;		/* Next input char to read	*/
	uint16	clkdiv;		/* Next slot for arriving char	*/
	char work_mode;
	int H_range;
	int L_range;
	int sample_avg;
	int sample_dly;
	int open_dly;
	int mode;
	char	*adchead;		/* Next input char to read	*/
	char	*adctail;		/* Next slot for arriving char	*/
	char	adcbuf[ADC_BUFLEN];	/* Input buffer (holds one line)*/
	sid32	sem;			/* Input semaphore		*/
};

extern	struct	gpiocblk	gpiotab[];
extern struct	adccblk	adctab[];
extern unsigned int* PortIDSet_ptr[2];

