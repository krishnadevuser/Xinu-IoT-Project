
#include <xinu.h>
#define ADCTSC_CTRL	0x40
#define ADCTSC_MMAP_ADDR	0x44E0D000

void gpio_adc_in(
	 struct	adccblk	*adcptr,		/* Ptr to gpiotab entry		*/
	 struct	adc_csreg *csrptr	/* Address of GPIO's CSRs	*/
	)
{

	int chn_ID =0;
	int32	avail;			
	int i ;
	unsigned int *reg_ctrl = NULL;

//	avail = semcount(adcptr->sem);
	//if (avail < 0) {	
	//	avail = 0;
	//}
	//if (avail >= ADC_BUFLEN) { 
	//	return;
	//}

	unsigned int buf_data = 0;
	kprintf("ADC Handler  called  data = %u \n",buf_data );
	ADCTSC_module_ctrl(adcptr,csrptr);
	adc_channel_status(	csrptr, adcptr->chn_ID , 1); //enable channel
	reg_ctrl = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;
	*reg_ctrl |= (CTRL_ENABLE | CTRL_STEP_ID_TAG);
	kprintf("ADC Handler  called  data = %u \n",csrptr->fifo_0_data );
	while(csrptr->st_en !=0) {
		kprintf("ADC init 7 = %u aptr->fifo_cnt=%x\n",csrptr->fifo_cnt, &csrptr->fifo_cnt);
		if(csrptr->fifo_cnt  > 0) {
			

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
					adc_channel_status(	csrptr, adcptr->chn_ID , 0); //disable channel
				}
			}
		} else {
			adc_channel_status(	csrptr, adcptr->chn_ID , 0);
			break;
		}
	}
	reg_ctrl = (void *)ADCTSC_MMAP_ADDR+ADCTSC_CTRL;
     *reg_ctrl &= ~(CTRL_ENABLE | CTRL_STEP_ID_TAG);
	return;
}