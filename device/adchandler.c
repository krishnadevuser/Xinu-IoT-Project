#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandler  -  Handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void adchandler(uint32 xnum) {
	struct	dentry	*devptr;	
	struct	adccblk	*adcptr;		
	struct	adc_csreg *csrptr;	
	uint32	iir = 0;		
	uint32	lsr = 0;		
	int chn_ID =0;
	int32	avail;			
	int i ;
	unsigned int buf_data = 0;

	kprintf("ADC Handler  called  \n" );
	return ;
	devptr = (struct dentry *) &devtab[CONSOLE];
	csrptr = (struct adc_csreg *) devptr->dvcsr;


	
	iir = csrptr->irq_s;

	/*if (iir & ADC_IIR_IRQ == 0) {
		return;
    }
	//iir &= ADC_IIR_IDMASK;		

	avail = semcount(adcptr->sem);
	if (avail < 0) {		
		avail = 0;
	}
	if (avail >= ADC_BUFLEN) { 
		return;
	}

	

	adcptr = &adctab[ devptr->dvminor ];
		if(csrptr->fifo_cnt  > 0) {
		

			for(i = 0 ; i < csrptr->fifo_cnt ; i++) {
				buf_data = csrptr->fifo_0_data;
				kprintf("ADC Handler  called  data = %u \n",buf_data );
				chn_ID = (buf_data >> 16) & 0xF;

				if(adcptr->adctail == adcptr->adctail && semcount(adcptr->sem) <=ADC_BUFLEN ) {
					*adcptr->adctail++ = buf_data & 0xFFF;
					if (adcptr->adctail>=&adcptr->adcbuf[ADC_BUFLEN]) {
						adcptr->adctail = adcptr->adcbuf;
					}
					signal(adcptr->sem);
				}
				else {
					//adc_channel_status(	&csrptr->st_en, adcptr->chn_ID , 0); //disable channel
				}
			}
		}*/

	
}