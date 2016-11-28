#
# Make the Xinu configuration program
#

COMPILER_ROOT	=	/usr/bin/
GSL_ROOT = /usr/local/bin/
CC	= ${COMPILER_ROOT}gcc
LEX	= ${COMPILER_ROOT}flex
YACC	= ${COMPILER_ROOT}bison -y	# Flag enables yacc-compatible filenames
GSL	= ${GSL_ROOT}gsl -script:
CFLAGS	= 
LFLAGS	= -ll

#
# Name of the configuration program
#

CONFIG	= config

all:		conf.h conf.c gpioinit.c main.c adcinit.c

${CONFIG}:	lex.yy.c y.tab.c
		$(CC) ${CFLAGS} -o $@ y.tab.c ${LFLAGS}

lex.yy.c:	config.l
		$(LEX) config.l

y.tab.c:	config.y
		$(YACC) config.y

gpioinit.c:	device.xml
		$(GSL)gpio.gsl device.xml

main.c:	device.xml
		$(GSL)main.gsl device.xml

adcinit.c:device.xml
		$(GSL)adc.gsl device.xml
clean:
		rm -f config lex.yy.c y.tab.c gpioinit.c main.c adcinit.c

conf.h:		config Configuration
		./config Configuration conf.c conf.h

conf.c:		config Configuration
		./config Configuration conf.c conf.h

install:	conf.h conf.c gpioinit.c adcinit.c
		cp -p conf.h ../include
		cp -p conf.c ../system
		cp -p gpioinit.c ../device/gpio
		cp -p adcinit.c ../device/adc
		cp -p main.c ../system
