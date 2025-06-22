# character device driver

EXTRA_CFLAGS = -Wall
obj-m = full-app-chdev-N-gpio.o

KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean