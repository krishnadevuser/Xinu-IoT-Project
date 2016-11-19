#include <xinu.h>

devcall gpiopush(struct dentry *devptr, char ch)
{
	struct gpiocblk *gpioptr;

	gpioptr = &gpiotab[devptr->dvminor];

	wait(gpioptr->sem);
	*gpioptr->gpiotail++ = ch;

	if(gpioptr->gpiotail >= gpioptr->gpiobuf[GPIO_BUFLEN])
	{
		gpioptr->gpiotail = gpioptr->gpiobuf;
	}
	gpiohandle_out(gpioptr,(struct gpio_csreg *)devptr->dvcsr); //for testing
	//gpioickout((struct gpio_csreg *)devptr->devcsr);
	return OK;
}