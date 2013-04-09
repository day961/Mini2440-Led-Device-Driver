KERNELDIR = /usr/src/linux-2.6.32.2/
#KERNELDIR = /usr/src/linux-headers-3.2.0-29-generic-pae/
PWD := $(shell pwd)

CC = arm-linux-gcc

obj-m := led.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
#.PHONY:modules modules_install clean
