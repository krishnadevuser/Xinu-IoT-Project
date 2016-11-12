
#ifndef	Ngpio
#define	Ngpio		4		/* Number of serial tty lines	*/
#endif



struct	gpiocblk	{			/* Tty line control block	*/
	char	port;		/* Next input char to read	*/
	char	pin;		/* Next slot for arriving char	*/
};

extern	struct	gpiocblk	gpiotab[];
extern unsigned int* PortIDSet_ptr[2];

