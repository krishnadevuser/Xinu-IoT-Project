
#include <xinu.h>

devcall	adcread(struct dentry *devptr, char *buff, int32 count)
{
	struct adccblk *adcptr;		
	int32	avail;			
	int32	nread;			
	int32	firstch;		
	char	ch;			

	/*if (count < 0) {
		return SYSERR;
	}
	adcptr = &adctab[devptr->dvminor];

	
		if (count == 0) {
			avail = semcount(adcptr->sem);
			if (avail == 0) {
				return 0;
			} else {
				count = avail;
			}
		}
		for (nread = 0; nread < count; nread++) {
			*buff++ = (char) adcgetc(devptr);
		}
		return nread;
	


	firstch = adc_pull(devptr);



	if (firstch == EOF) {
		return EOF;
	}



	ch = (char) firstch;
	*buff++ = ch;
	nread = 1;
	while ( (nread < count) && (ch != '\n') &&
			(ch != '\r') ) {
		ch = adc_pull(devptr);
		*buff++ = ch;
		nread++;
	}*/
	return nread;
}
