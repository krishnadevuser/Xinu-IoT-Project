#include <xinu.h>

devcall gpiopush(struct dentry *devptr, bool8 ch)
{
	
	//struct gpio_csreg * csrptr = (struct uart_csreg *) devptr->dvcsr;

	struct gpiocblk* gpioptr =(struct gpiocblk*) &gpiotab[ devptr->dvminor ];
	wait(gpioptr->sem);
	if (ch)
		*gpioptr->gpiotail = 1;
	else 
		*gpioptr->gpiotail = 0;
		gpioptr->gpiotail++;

	if(gpioptr->gpiotail >= gpioptr->gpiobuf[GPIO_BUFLEN])
	{
		gpioptr->gpiotail = gpioptr->gpiobuf;
	}

	gpiohandle_out(gpioptr,(struct gpio_csreg *)devptr->dvcsr); //for testing
	//gpioickout((struct gpio_csreg *)devptr->devcsr);
	return OK;
}