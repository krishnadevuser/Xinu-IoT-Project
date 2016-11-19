#include<xinu.h>

devcall adc_pull(struct dentry *devptr)
{
	char ch;
	struct adcblk *adcptr;

	adcptr = &adctab[devptr->dvminor];

	wait(adcptr->sem);
	ch = *adcptr->adchead++;

	if (adcptr->adchead >= &adcptr->adcbuf[ADC_BUFLEN]) 
	{
		adcptr->adchead = adcptr->adcbuf;
	}
	return (devcall)ch;
}