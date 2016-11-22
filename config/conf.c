/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <xinu.h>


extern	devcall	ioerr(void);
extern	devcall	ionull(void);

/* Device independent I/O switch */

struct	dentry	devtab[NDEVS] =
{
/**
 * Format of entries is:
 * dev-number, minor-number, dev-name,
 * init, open, close,
 * read, write, seek,
 * getc, putc, control,
 * dev-csr-address, intr-handler, irq
 */

/* CONSOLE is tty */
	{ 0, 0, "CONSOLE",
	  (void *)ttyinit, (void *)ionull, (void *)ionull,
	  (void *)ttyread, (void *)ttywrite, (void *)ioerr,
	  (void *)ttygetc, (void *)ttyputc, (void *)ttycontrol,
	  (void *)0x44e09000, (void *)ttyhandler, 72 },

/* TEMP is adc */
	{ 1, 0, "TEMP",
	  (void *)adcinit, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)adcpoll, (void *)ionull, (void *)ionull,
	  (void *)0x44e0d000, (void *)adchandler, 16 },

/* NULLDEV is null */
	{ 2, 0, "NULLDEV",
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)ionull, (void *)ionull, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* ETHER0 is eth */
	{ 3, 0, "ETHER0",
	  (void *)ethinit, (void *)ioerr, (void *)ioerr,
	  (void *)ethread, (void *)ethwrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ethcontrol,
	  (void *)0x0, (void *)ethhandler, 0 },

/* NAMESPACE is nam */
	{ 4, 0, "NAMESPACE",
	  (void *)naminit, (void *)namopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ioerr, 0 },

/* RDISK is rds */
	{ 5, 0, "RDISK",
	  (void *)rdsinit, (void *)rdsopen, (void *)rdsclose,
	  (void *)rdsread, (void *)rdswrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)rdscontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* RAM0 is ram */
	{ 6, 0, "RAM0",
	  (void *)raminit, (void *)ramopen, (void *)ramclose,
	  (void *)ramread, (void *)ramwrite, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILESYS is rfs */
	{ 7, 0, "RFILESYS",
	  (void *)rfsinit, (void *)rfsopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)rfscontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE0 is rfl */
	{ 8, 0, "RFILE0",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE1 is rfl */
	{ 9, 1, "RFILE1",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE2 is rfl */
	{ 10, 2, "RFILE2",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE3 is rfl */
	{ 11, 3, "RFILE3",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE4 is rfl */
	{ 12, 4, "RFILE4",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE5 is rfl */
	{ 13, 5, "RFILE5",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE6 is rfl */
	{ 14, 6, "RFILE6",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE7 is rfl */
	{ 15, 7, "RFILE7",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE8 is rfl */
	{ 16, 8, "RFILE8",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* RFILE9 is rfl */
	{ 17, 9, "RFILE9",
	  (void *)rflinit, (void *)ioerr, (void *)rflclose,
	  (void *)rflread, (void *)rflwrite, (void *)rflseek,
	  (void *)rflgetc, (void *)rflputc, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILESYS is lfs */
	{ 18, 0, "LFILESYS",
	  (void *)lfsinit, (void *)lfsopen, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)ioerr, (void *)ioerr, (void *)ioerr,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE0 is lfl */
	{ 19, 0, "LFILE0",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE1 is lfl */
	{ 20, 1, "LFILE1",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE2 is lfl */
	{ 21, 2, "LFILE2",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE3 is lfl */
	{ 22, 3, "LFILE3",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE4 is lfl */
	{ 23, 4, "LFILE4",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* LFILE5 is lfl */
	{ 24, 5, "LFILE5",
	  (void *)lflinit, (void *)ioerr, (void *)lflclose,
	  (void *)lflread, (void *)lflwrite, (void *)lflseek,
	  (void *)lflgetc, (void *)lflputc, (void *)lflcontrol,
	  (void *)0x0, (void *)ionull, 0 },

/* SPI0 is spi */
	{ 25, 0, "SPI0",
	  (void *)spiinit, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)spicontrol,
	  (void *)0x48030000, (void *)ionull, 0 },

/* SPI1 is spi */
	{ 26, 1, "SPI1",
	  (void *)spiinit, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)spicontrol,
	  (void *)0x481a0000, (void *)ionull, 0 },

/* LED is gpio */
	{ 27, 0, "LED",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)gpio_pull, (void *)gpiopush, (void *)ionull,
	  (void *)0x4804c000, (void *)gpiohandler, 98 },

/* INPUT is gpio */
	{ 28, 1, "INPUT",
	  (void *)gpioinit, (void *)ionull, (void *)ionull,
	  (void *)ionull, (void *)ionull, (void *)ionull,
	  (void *)gpio_pull, (void *)gpiopush, (void *)ionull,
	  (void *)0x4804c000, (void *)gpiohandler, 98 }
};
