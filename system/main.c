/*  main.c  - main */

#include <xinu.h>
#define BUF_SIZE  10
pid32 producer_id;
pid32 consumer_id;
pid32 timer_id;

int32 consumed_count = 0;
const int32 CONSUMED_MAX = 100;

/* Define your circular buffer structure and semaphore variables here */
/* */

struct cir_queue{
	int32 buffer[BUF_SIZE];
	int32 in;
	int32 out;
	int32 len;

};
struct cir_queue cir_buf;
sid32 full, empty;
sid32 mutex;

void create_queue(void)
{
	/* */
	cir_buf.max_len = BUF_SIZE;
	cir_buf.in = 0;
	cir_buf.out = 0 ;

}
/* Place your code for entering a critical section here */
void mutex_acquire(sid32 mutex)
{
	/* */
	wait(mutex);
}

/* Place your code for leaving a critical section here */
void mutex_release(sid32 mutex)
{
	/* */
	signal(mutex);
}


/* Place the code for the buffer producer here */
process producer(void)
{
	/* */
	while (1) {
	  	wait(empty);
	  	int32 item = rand() % 10;
	  	mutex_acquire(mutex);
	  	if (cir_buf.in == cir_buf.len)
	  		cir_buf.in =0;
	  	cir_buf.buffer[cir_buf.in++] = item;
	  	mutex_release(mutex);
	  	kprintf("Item Produced: %d\n", item);
	  	signal(full);
	  	
	  }
	return OK;
}

/* Place the code for the buffer consumer here */
process consumer(void)
{
	while (1) {
	  wait(full);
	  mutex_acquire(mutex);
	  if (cir_buf.out == cir_buf.len)
	  		cir_buf.out =0;
	  int32 w = cir_buf.buffer[cir_buf.out++];
	  kprintf("Item Consumed: %d\n", w);
	  consumed_count += 1;
	  mutex_release(mutex);
	  signal(empty);
	  /*consume item w */
	}
	/* Every time your consumer consumes another buffer element,
	 * make sure to include the statement:
	 *   consumed_count += 1;
	 * this will allow the timing function to record performance */
	/* */

	return OK;
}


/* Timing utility function - please ignore */
process time_and_end(void)
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

	kill(producer_id);
	kill(consumer_id);

	for (i = 0; i < 5; ++i)
	{
		kprintf("TIME ELAPSED (%d): %d\n", (i+1) * CONSUMED_MAX, times[i]);
	}
}

process	main(void)
{
	recvclr();

	/* Create the shared circular buffer and semaphores here */
	/* */
	
	create_queue();
	empty = semcreate(cir_buf.len);
	full = semcreate(0);
	mutex = semcreate(1);
	producer_id = create(producer, 4096, 50, "producer", 0);
	consumer_id = create(consumer, 4096, 50, "consumer", 0);
	timer_id = create(time_and_end, 4096, 50, "timer", 0);

	resched_cntl(DEFER_START);
	resume(producer_id);
	resume(consumer_id);
	/* Uncomment the following line for part 3 to see timing results */
	resume(timer_id);
	resched_cntl(DEFER_STOP);

	return OK;
}
