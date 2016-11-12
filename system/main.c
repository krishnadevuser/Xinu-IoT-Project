/*  main.c  - main */

#include <xinu.h>


/* Timing utility function - please ignore */
/*process time_and_end(void)
{
	int32 times[5];
	int32 i;

	for (i = 0; i < 5; ++i)
	{
		times[i] = clktime_ms;
		yield();

		consumed_count = 0;
		while (consumed_count < CONSUMED_MAX * (i+1))
		{
			yield();
		}

		times[i] = clktime_ms - times[i];
	}

	//kill(producer_id);
	//kill(consumer_id);

	for (i = 0; i < 5; ++i)
	{
		kprintf("TIME ELAPSED (%d): %d\n", (i+1) * CONSUMED_MAX, times[i]);
	}
}*/

process	main(void)
{
	recvclr();
	struct	gpiocblk	*gpioptr;

	gpiohandle_out(gpioptr,(struct uart_csreg *) 0x4804C000);

	/* Create the shared circular buffer and semaphores here */
	/* */
	
	
	//timer_id = create(time_and_end, 4096, 50, "timer", 0);

	//resched_cntl(DEFER_START);

	/* Uncomment the following line for part 3 to see timing results */
	//resume(timer_id);
	//resched_cntl(DEFER_STOP);

	return OK;
}
