CC = arm-linux-gcc

all:usertest

usertest:leda.o
	$(CC) -o usertest leda.o
leda.o:leda.c
	$(CC) -c leda.c
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.order *.symvers leda
.PHONY: all clean
