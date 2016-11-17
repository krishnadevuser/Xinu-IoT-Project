#include<xinu.h>

devcall gpio_pull(struct dentry *devptr)
{
	char ch;
	struct gpioblk *gpioptr;

	gpioptr = &gpiotab[devptr->dvminor];

	wait(gpioptr->sem);
	ch = *gpioptr->gpiohead++;

	if (gpioptr->gpiohead >= &gpioptr->gpiobuf[GPIO_BUFLEN]) 
	{
		gpioptr->gpiohead = gpioptr->gpiobuf;
	}
	return (devcall)ch;
}