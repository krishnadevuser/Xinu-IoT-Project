/* gpiohandle_in.c - gpiohandle_in */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  gpiohandle_in  -  Handle an output on a tty device by sending more
 *		     characters to the device FIFO (interrupts disabled)
 *------------------------------------------------------------------------
 */


char gpiohandler(
	 struct	gpiocblk *gpioptr,	/* Ptr to gpiotab entry		*/
	 struct	gpio_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{

	int32	avail;			/* Chars available in buffer	*/
	avail = semcount(gpioptr->sem);
	/*if (avail < 0) {		
		avail = 0;
	}*/
	/*if (avail >= GPIO_BUFLEN) { 
		return;
	}*/	
	if ((csrptr->data_in & PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1])) {
		*gpioptr->gpiotail = 1;
		*gpioptr->gpiotail++;
	}
	else {
		*gpioptr->gpiotail = 0;
		*gpioptr->gpiotail++;
	}
	if (gpioptr->gpiotail>=&gpioptr->gpiobuf[GPIO_BUFLEN]) {
		gpioptr->gpiotail = gpioptr->gpiobuf;
	}
	signal(gpioptr->sem);

	
	return 0;
}
