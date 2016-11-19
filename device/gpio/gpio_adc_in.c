
#include <xinu.h>

void gpio_adc_in(
	 struct	adccblk	*adcptr,		/* Ptr to gpiotab entry		*/
	 struct	adc_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{

	int chn_ID =0;
	int32	avail;			/* Chars available in buffer	*/
	int i ;

	avail = semcount(adcptr->sem);
	if (avail < 0) {		/* One or more processes waiting*/
		avail = 0;
	}
	if (avail >= ADC_BUFLEN) { /* No space => ignore input	*/
		return;
	}
	unsigned int buf_data = 0;
	adc_channel_status(	&csrptr->st_en, adcptr->chn_ID , 1); //enable channel
	while(csrptr->st_en !=0) {
		if(csrptr->fifo_cnt  > 0) {
			/* fetch data from FIFO */

			for(i = 0 ; i < csrptr->fifo_cnt ; i++) {
				buf_data = csrptr->fifo_0_data;
				chn_ID = (buf_data >> 16) & 0xF;
				kprintf("ADC Handler  called  data = %u \n",buf_data );
				if(adcptr->adctail == adcptr->adctail && semcount(adcptr->sem) <=ADC_BUFLEN ) {
					*adcptr->adctail++ = buf_data & 0xFFF;
					if (adcptr->adctail>=&adcptr->adcbuf[ADC_BUFLEN]) {
						adcptr->adctail = adcptr->adcbuf;
					}
					signal(adcptr->sem);
				}
				else {
					adc_channel_status(	&csrptr->st_en, adcptr->chn_ID , 0); //disable channel
				}
			}
		}
	}
	
	return;
}