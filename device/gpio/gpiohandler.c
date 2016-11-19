/* gpiohandle_in.c - gpiohandle_in */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  gpiohandle_in  -  Handle an output on a tty device by sending more
 *		     characters to the device FIFO (interrupts disabled)
 *------------------------------------------------------------------------
 */

 char gpiohandler(uint32 xnum)
{
	struct	dentry	*devptr;	/* Address of device control blk*/
	int32	avail;			/* Chars available in buffer	*/
	//avail = semcount(gpioptr->sem);
	/*if (avail < 0) {		
		avail = 0;
	}*/
	/*if (avail >= GPIO_BUFLEN) { 
		return;
	}*/	

	devptr = (struct dentry *) &devtab[27];
	struct gpio_csreg * csrptr = (struct uart_csreg *) devptr->dvcsr;
	struct gpiocblk* gpioptr = &gpiotab[ devptr->dvminor ];
	//struct gpio_csreg *csrptr = (struct gpio_csreg *) 0x4804C000;

	
	if ((csrptr->data_in & PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1]))
	{
		*gpioptr->gpiotail = 1;
		*gpioptr->gpiotail++;
		kprintf("Interrupt called value 1 \n" );
	}
	else
	{
		kprintf("Interrupt called value 0 \n" );
		*gpioptr->gpiotail = 0;
		*gpioptr->gpiotail++;
	}
	if (gpioptr->gpiotail>=&gpioptr->gpiobuf[GPIO_BUFLEN]) {
		gpioptr->gpiotail = gpioptr->gpiobuf;
	}
	csrptr->irq_s_0 = PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1];
	
	signal(gpioptr->sem);

	
	return 0;
}
