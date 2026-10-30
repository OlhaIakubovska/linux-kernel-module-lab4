obj-m := hello.o

KDIR ?= $(HOME)/repos/linux-stable

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
