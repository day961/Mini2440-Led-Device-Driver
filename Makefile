KERNELDIR = /usr/src/linux-2.6.32.2/
#KERNELDIR = /usr/src/linux-headers-3.2.0-29-generic-pae/
PWD := $(shell pwd)
SUBDIRS = usertest
CC = arm-linux-gcc

obj-m := led.o

.PHONY:default all $(SUBDIRS) modules modules_install clean

default: all

all clean:
	$(MAKE) $(SUBDIRS) TARGET=$@

$(SUBDIRS):
	$(MAKE) -C $@ $(TARGET)

all:modules

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
