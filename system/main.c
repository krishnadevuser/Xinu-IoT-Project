/*  main.c  - main */

#include <xinu.h>



struct	gpiocblk gpiotab[2];
struct	adccblk	adctab[1];

process	main(void)
{

	recvclr();
	
	//////////////////adc //////////////////////////////////
	/*struct	adccblk* adcptr = &adctab[0];
	adcptr->adchead = adcptr->adtctail = 	
	&adcptr->adcbuff[0];		
	adcptr->adcsem = semcreate(0);		
	adcptr->clk_div = 160 ;
	adcptr->chn_ID = 1 ;
	adcptr->open_dly = 0;
	adcptr->sample_dly = 1;*/


	kprintf("ADC init complete\n");
	////////////////////////////////////////////////////////
	//gpioinit();
	////////gpio output to pin 12 and port 8///////////
	/*struct	gpiocblk	*gpioptr = &gpiotab[0];
	gpioptr->port = 8;
	gpioptr->pin = 12;
	gpioptr->gpiohead = gpioptr->gpiotail = &gpioptr->gpiobuf[0];	
	*gpioptr->gpiohead = 1;
	gpiohandle_out(gpioptr,(struct gpio_csreg *) 0x4804C000);*/
	/////////////////////////////////////////////////////
	/////////////////gpio input pin 11 port 8///////////////////
	/*struct	gpiocblk *gptr =&gpiotab[1];
	gptr->port = 8;
	gptr->pin = 11;
	gptr->int_mode = 1;
	gptr->gpiohead = gptr->gpiotail = &gptr->gpiobuf[0];
	set_evec( 98, (uint32)*gpiohandler );
	gpioint_en(gptr,(struct gpio_csreg *) 0x4804C000);
	char value = gpiohandle_in(gptr,(struct gpio_csreg *) 0x4804C000);
	kprintf("value = %u",value);*/
	/////////////////////////////////////////////////////////////////////
	putc(LED,1);

	kprintf("Start main function\r\n");
	char data[4];
	unsigned int integer = 0;
	unsigned int decimal = 0;
	float voltage;
	while(1)
	{
		read(TEMP,(char*)data,4);
		unsigned int vol = *(unsigned int*)data;
		voltage = (vol*1.8/4095)*1000;

		integer = (unsigned int)voltage;
		decimal = (unsigned int)((voltage - integer)*10);
		kprintf("voltage = %d.%d  mv\r\n",integer,decimal);
		sleep(1);
	}

	//getc(TEMP);
	while(1){
		char value = getc(INPUT);
			kprintf("value = %u\n", value);
		//kprintf("data value = %f",((float)analogRead() / 4095.0f) * 1.8f);
	}

	/* Create the shared circular buffer and semaphores here */
	/* */
	
	
	//timer_id = create(time_and_end, 4096, 50, "timer", 0);

	//resched_cntl(DEFER_START);

	/* Uncomment the following line for part 3 to see timing results */
	//resume(timer_id);
	//resched_cntl(DEFER_STOP);

	return OK;
}
