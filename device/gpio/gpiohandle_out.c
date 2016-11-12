/* ttyhandle_out.c - ttyhandle_out */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandle_out  -  Handle an output on a tty device by sending more
 *		     characters to the device FIFO (interrupts disabled)
 *------------------------------------------------------------------------
 */


 /* GPIO Port number set of Beaglebone Black P8 .
 * -1 as GND or VCC , Number 0/1/2 as GPIO 0/1/2
 */
/*const signed char p8_PortSet[] = {-1, -1, 1, 1, 1, 1, 2, 2,
				2, 2, 1, 1, 0, 0, 1, 1, 
				0, 2, 0, 1, 1, 1, 1, 1, 
				1, 1, 2, 2, 2, 2, 0, 0, 
				0, 2, 0, 2, 2, 2, 2, 2, 
				2, 2, 2, 2, 2, 2};*/

/* GPIO Port ID set of Beaglebone Black P8  ,
 * 0 as GND , offset X as GPIO ID m this value must combine with GPIO number
 */
const unsigned int p8_PortIDSet[] = {0,	0,	1<<6,	1<<7,	1<<2,	1<<3,	1<<2,	1<<3,	
				1<<5,	1<<4,	1<<13,	1<<12,	1<<23,	1<<26,	1<<15,	
				1<<14,	1<<27,	1<<1,	1<<22,	1<<31,	1<<30,	1<<5,	
				1<<4,	1<<1,	1<<0,	1<<29,	1<<22,	1<<24,	1<<23,	
				1<<25,	1<<10,	1<<11,	1<<9,	1<<17,	1<<8,	1<<16,	
				1<<14,	1<<15,	1<<12,	1<<13,	1<<10,	1<<11,	1<<8,	
				1<<9,	1<<6,	1<<7};

/* GPIO Port number set of Beaglebone Black P9  ,
 * -1 as GND or VCC , 0 / 1 / 2 for GPIO 0/1/2
 */
/*const signed char p9_PortSet[] = {-1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, 0, 1, 0, 1, 1, 
				1, 0, 0, 0, 0, 0, 0, 1, 0, 
				3, 0, 3, 3, 3, 3, 3, -1, -1, 
				-1, -1, -1, -1, -1, -1, -1, 
				0, 0, -1, -1, -1, -1};*/

/*  GPIO Port ID set of Beaglebone Black P9  ,
 * 0 as GND , offset X as GPIO ID m this value must combine with GPIO number
 */
const unsigned int p9_PortIDSet[]={0,	0,	0,	0,	0,	0,	0,	0,	
				0,	0,	1<<30,	1<<28,	1<<31,	1<<18,	1<<16,	1<<19,	
				1<<5,	1<<4,	1<<13,	1<<12,	1<<3,	1<<2,	1<<17,	
				1<<15,	1<<21,	1<<14,	1<<19,	1<<17,	1<<15,	1<<16,	
				1<<14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1<<20,	
				1<<7,	0,	0,	0,	0};


void gpiohandle_out(
	 struct	gpiocblk	*gpioptr,		/* Ptr to gpiotab entry		*/
	 struct	gpio_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{
	char port;
	char pin;
	port = 8;
	pin = 12;
	//signed char* PortSet_ptr[2];

	unsigned int* PortIDSet_ptr[2];
	PortIDSet_ptr[0]=(unsigned int*)p8_PortIDSet;
	PortIDSet_ptr[1]=(unsigned int*)p9_PortIDSet;
	printf("gpio_addr= %x , val = %u" ,csrptr,~(PortIDSet_ptr[port-8][pin-1]));
	csrptr->oe &= ~(PortIDSet_ptr[port-8][pin-1]);
	printf("csrptr->oe = %x , value = %u" ,&csrptr->oe,csrptr->oe);
	csrptr->set_data_out= PortIDSet_ptr[port-8][pin-1];
	printf("csrptr->set_data_out = %x , value = %d",&csrptr->set_data_out, csrptr->set_data_out);
	
	return;
}
