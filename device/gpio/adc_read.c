
#include <xinu.h>

devcall	adcread(struct dentry *devptr, char *buff, int32 count)
{
	struct	adcblk	*adcptr;		/* Pointer to tadc control block	*/
	int32	avail;			/* Characters available in buff.*/
	int32	nread;			/* Number of characters read	*/
	int32	firstch;		/* First input character on line*/
	char	ch;			/* Next input character		*/

	if (count < 0) {
		return SYSERR;
	}
	adcptr= &adctab[devptr->dvminor];

	
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
	

	/* Block until input arrives */

	firstch = adc_pull(devptr);

	/* Check for End-Of-File */

	if (firstch == EOF) {
		return EOF;
	}

	/* Read up to a line */

	ch = (char) firstch;
	*buff++ = ch;
	nread = 1;
	while ( (nread < count) && (ch != '\n') &&
			(ch != '\r') ) {
		ch = adc_pull(devptr);
		*buff++ = ch;
		nread++;
	}
	return nread;
}
