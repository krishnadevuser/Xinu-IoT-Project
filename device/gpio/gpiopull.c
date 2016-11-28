#include <xinu.h>

/**************************************************************************
function name:  gpio_pull
description:    Read one character from a GPIO device
input:          Pointer to the device Switch Table entry
author:         Srikrishna Iyer
**************************************************************************/

devcall gpio_pull(
	struct dentry *devptr	/* Entry in device switch table	*/
	)
{
	char ch;	/* Character to return		*/
	struct gpiocblk *gpioptr;	/* Pointer to ttytab entry	*/

	gpioptr = &gpiotab[devptr->dvminor];

	/* Wait for a character in the buffer and extract one character	*/

	wait(gpioptr->sem);
	
	ch = *gpioptr->gpiohead++;

	/* Wrap around to beginning of buffer, if needed */
	
	if (gpioptr->gpiohead >= &gpioptr->gpiobuf[GPIO_BUFLEN]) 
	{
		gpioptr->gpiohead = gpioptr->gpiobuf;
	}
	return (devcall)ch;
}