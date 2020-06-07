ifneq ($(KERNELRELEASE),)
	obj-m := scull.o
else
	PWD :=$(shell pwd)
	KDIR := /home/lipu/Raspberry/linux-rpi-4.19.y/
	#KDIR :=/lib/modules/$(shell uname -r)/build
all:
	make  -C  $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*
endif

