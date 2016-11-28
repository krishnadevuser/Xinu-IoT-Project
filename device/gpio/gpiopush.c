#include <xinu.h>

/**************************************************************************
function name:  gpio_push
description:    Write one character from a GPIO device
input:          Pointer to the device Switch Table entry and a boolean character
author:         Srikrishna Iyer
**************************************************************************/

devcall gpiopush(
	struct dentry *devptr,	/* Entry in device switch table	*/
	bool8 ch 				/* Character to write		*/
	)
{
	
	//struct gpio_csreg * csrptr = (struct uart_csreg *) devptr->dvcsr;

	struct gpiocblk* gpioptr =(struct gpiocblk*) &gpiotab[ devptr->dvminor ];	
	
	wait(gpioptr->sem);
	
	if (ch)
		*gpioptr->gpiotail = 1;
	else 
		*gpioptr->gpiotail = 0;
	
	gpioptr->gpiotail++;

	/* Wrap around to beginning of buffer, if needed */

	if(gpioptr->gpiotail >= gpioptr->gpiobuf[GPIO_BUFLEN])
	{
		gpioptr->gpiotail = gpioptr->gpiobuf;
	}

	gpiohandle_out(gpioptr,(struct gpio_csreg *)devptr->dvcsr); //for testing
	//gpioickout((struct gpio_csreg *)devptr->devcsr);
	return OK;
}