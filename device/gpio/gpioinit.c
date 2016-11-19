
#include <xinu.h>


struct	gpiocblk	gpiotab[Ngpio];

/* GPIO Port ID set of Beaglebone Black P8  ,
 * 0 as GND , offset X as GPIO ID m this value must combine with GPIO number
 */
const unsigned int p8_PortIDSet[] = {0,	0,	1<<6,	1<<7,	1<<2,	1<<3,	1<<2,	1<<3,	
				1<<5,	1<<4,	1<<13,	1<<12,	1<<23,	1<<26,	1<<15,	
				1<<14,	1<<27,	1<<1,	1<<22,	1<<31,	1<<30,	1<<5,	
				1<<4,	1<<1,	1<<0,	1<<29,	1<<22,	1<<24,	1<<23,	
				1<<25,	1<<10,	1<<11,	1<<9,	1<<17,	1<<8,	1<<16,	
				1<<14,	1<<15,	1<<12,	1<<13,	1<<10,	1<<11,	1<<8,	
				1<<9,	1<<6,	1<<7};



const unsigned int p9_PortIDSet[]={0,	0,	0,	0,	0,	0,	0,	0,	
				0,	0,	1<<30,	1<<28,	1<<31,	1<<18,	1<<16,	1<<19,	
				1<<5,	1<<4,	1<<13,	1<<12,	1<<3,	1<<2,	1<<17,	
				1<<15,	1<<21,	1<<14,	1<<19,	1<<17,	1<<15,	1<<16,	
				1<<14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1<<20,	
				1<<7,	0,	0,	0,	0};


/*------------------------------------------------------------------------
 *  ttyinit  -  Initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
 unsigned int* PortIDSet_ptr[2];


void gpioint_en (
	 struct	gpiocblk *gpioptr,	/* Ptr to gpiotab entry		*/
	 struct	gpio_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{
	if (gpioptr->int_mode) {
		csrptr->rise_det = PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
		csrptr->fall_det = PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
		csrptr->irq_s_set_0 = PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
	}
	else {
		csrptr->rise_det &= ~PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
		csrptr->fall_det &= ~PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
		csrptr->irq_s_clr_0 = PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
	}
	
}


devcall	gpioinit(
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	struct	gpiocblk	*gpioptr;		/* Pointer to ttytab entry	*/
	struct	gpio_csreg *gptr;	/* Address of UART's CSRs	*/
	/* Initialize mapping of port and reg value for GPIO */
	PortIDSet_ptr[0]=(unsigned int*)p8_PortIDSet;
	PortIDSet_ptr[1]=(unsigned int*)p9_PortIDSet;
	gpioptr = &gpiotab[ devptr->dvminor ];
	gptr = (struct gpio_csreg *)devptr->dvcsr;

	//////////////////////////
	if(devptr->dvminor == 0) {
		gpioptr->port = 8;
		gpioptr->pin = 12;
	} else {
		gpioptr->port = 8;
		gpioptr->pin = 11;
		gpioptr->int_mode = 1;
	}
	gpioptr->gpiohead = gpioptr->gpiotail = &gpioptr->gpiobuf[0];	
	*gpioptr->gpiohead = 1;
	gpioptr->sem = semcreate(0);		/* Input semaphore	*/
	if (gpioptr->int_mode ) {
		set_evec( devptr->dvirq, (uint32)devptr->dvintr );
		gpioint_en(gpioptr,gptr);
	}


	if(devptr->dvminor == 0) {
		gpiohandle_out(gpioptr,gptr); //for testing
	} 
	return OK;
}


devcall	diginit(
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	struct	gpiocblk	*gpioptr;		/* Pointer to ttytab entry	*/
	struct	gpio_csreg *gptr;	/* Address of UART's CSRs	*/
	/* Initialize mapping of port and reg value for GPIO */
	PortIDSet_ptr[0]=(unsigned int*)p8_PortIDSet;
	PortIDSet_ptr[1]=(unsigned int*)p9_PortIDSet;
	gpioptr = &gpiotab[ devptr->dvminor ];
	gptr = (struct gpio_csreg *)devptr->dvcsr;

	//////////////////////////
	
	gpioptr->port = 8;
	gpioptr->pin = 11;
	gpioptr->int_mode = 1;
	gpioptr->gpiohead = gpioptr->gpiotail = &gpioptr->gpiobuf[0];	
	*gpioptr->gpiohead = 1;
	gpioptr->sem = semcreate(0);		/* Input semaphore	*/
	if (gpioptr->int_mode ) {
		set_evec( devptr->dvirq, (uint32)devptr->dvintr );
		gpioint_en(gpioptr,gptr);
	}

	return OK;

	///////////////////////////////

	
}



