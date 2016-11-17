#include<xinu.h>

devcall gpiopush(struct dentry *devptr, char ch)
{
	struct gpiocblk *gpioptr;

	gpioptr = &gpiotab[devptr->dvminor];

	wait(gpioptr->sem);
	*gpioptr->gpiotail++ = ch;

	if(gpioptr->gpiotail >= gpioptr->gpiobuf[GPIO_BUFLEN])
	{
		gpio->gpiotail = gpiobuf;
	}

	gpioickout((struct gpio_csreg *)devptr->devcsr);
	return OK;
}