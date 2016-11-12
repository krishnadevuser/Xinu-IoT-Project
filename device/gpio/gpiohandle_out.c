/* ttyhandle_out.c - ttyhandle_out */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandle_out  -  Handle an output on a tty device by sending more
 *		     characters to the device FIFO (interrupts disabled)
 *------------------------------------------------------------------------
 */


void gpiohandle_out(
	 struct	gpiocblk	*gpioptr,		/* Ptr to gpiotab entry		*/
	 struct	gpio_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{
	char port;
	char pin;
	port = 8;
	pin = 12;
	//signed char* PortSet_ptr[2];

	printf("gpio_addr= %x , val = %u" ,csrptr,~(PortIDSet_ptr[port-8][pin-1]));
	csrptr->oe &= ~(PortIDSet_ptr[port-8][pin-1]);
	printf("csrptr->oe = %x , value = %u" ,&csrptr->oe,csrptr->oe);
	csrptr->set_data_out= PortIDSet_ptr[port-8][pin-1];
	printf("csrptr->set_data_out = %x , value = %d",&csrptr->set_data_out, csrptr->set_data_out);
	
	return;
}
