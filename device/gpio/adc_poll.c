#include<xinu.h>

devcall adcpoll(struct dentry *devptr)
{
	char ch;
	struct adccblk *adcptr;

	adcptr = &adcctab[devptr->dvminor];

	while(1)
	{
		wait(adcptr->sem);
		ch = *typtr->tyihead++;

		if (adcptr->adchead >= &adcptr->adcbuf[ADC_BUFLEN])
		{
			adcptr->adchead = adcptr->adcbuf;
		}
	}
	return (devcall)ch;
}