#include<xinu.h>

devcall adcpoll(struct dentry *devptr)
{
	char ch;
	struct adccblk *adcptr;

	adcptr = &adctab[devptr->dvminor];
	gpio_adc_in(adcptr,(struct gpio_csreg *)devptr->dvcsr);
	return 0;
	while(1)
	{
		wait(adcptr->sem);
		ch = *adcptr->adchead++;

		if (adcptr->adchead >= &adcptr->adcbuf[ADC_BUFLEN])
		{
			adcptr->adchead = adcptr->adcbuf;
		}
	}
	return (devcall)ch;
}