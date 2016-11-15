#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandler  -  Handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void adchandler(uint32 xnum) {
	struct	dentry	*devptr;	/* Address of device control blk*/
	struct	adccblk	*adcptr;		/* Pointer to ttytab entry	*/	
	struct	adc_csreg *csrptr;	/* Address of UART's CSR	*/
	uint32	iir = 0;		/* Interrupt identification	*/
	uint32	lsr = 0;		/* Line status			*/
	int chn_ID =0;
	int32	avail;			/* Chars available in buffer	*/
	int i ;
	unsigned int buf_data = 0;


	devptr = (struct dentry *) &devtab[CONSOLE];
	csrptr = (struct adc_csreg *) devptr->dvcsr;


	/* Get CSR address of the device (assume console for now) */
	iir = csrptr->irq_s;

	if (iir & ADC_IIR_IRQ == 0) {
		return;
    }
	//iir &= ADC_IIR_IDMASK;		/* Mask off the interrupt ID */ADC_IIR_IDMASK 

	avail = semcount(adcptr->sem);
	if (avail < 0) {		/* One or more processes waiting*/
		avail = 0;
	}
	if (avail >= ADC_BUFLEN) { /* No space => ignore input	*/
		return;
	}

	/* Obtain a pointer to the tty control block */

	adcptr = &adctab[ devptr->dvminor ];
		if(csrptr->fifo_cnt  > 0) {
			/* fetch data from FIFO */

			for(i = 0 ; i < csrptr->fifo_cnt ; i++) {
				buf_data = csrptr->fifo_0_data;
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
		}

	
}