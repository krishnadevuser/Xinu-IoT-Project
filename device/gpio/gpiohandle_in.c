/* gpiohandle_in.c - gpiohandle_in */

#include <xinu.h>

/**************************************************************************
function name:  gpiohandle_in
description:    Handle an output on a tty device by sending more characters to the device FIFO (interrupts disabled)
input:          Pointer to GPIO Structure and Address of GPIO's CSR
author:         Madhav Agrawal   
**************************************************************************/

char gpiohandle_in(
	 struct	gpiocblk *gpioptr,	/* Ptr to gpiotab entry		*/
	 struct	gpio_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{

	//printf("gpio_addr= %x , val = %u" , csrptr, ~(PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1]));
	csrptr->oe |= (PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1]);
	//printf("csrptr->oe = %x , value = %u", &csrptr->oe, csrptr->oe);
	if ((csrptr->data_in & PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1]))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
