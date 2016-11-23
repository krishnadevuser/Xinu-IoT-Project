/* gpiohandle_in.c - gpiohandle_in */

#include <xinu.h>


 char gpiohandler(uint32 xnum)
{
	struct	dentry	*devptr;	

	devptr = (struct dentry *) &devtab[27];
	struct gpio_csreg * csrptr = (struct uart_csreg *) devptr->dvcsr;
	struct gpiocblk* gpioptr = &gpiotab[ devptr->dvminor ];

	if ((csrptr->data_in & PortIDSet_ptr[gpioptr->port-8][gpioptr->pin-1]))
	{
		*gpioptr->gpiotail = 1;
		*gpioptr->gpiotail++;
		//kprintf("Interrupt called value 1 \n" );
	}
	else
	{
		//kprintf("Interrupt called value 0 \n" );
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
