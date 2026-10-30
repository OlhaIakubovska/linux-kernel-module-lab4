# Usage:
#   export KDIR=$HOME/repos/linux-stable   (kernel tree already built)
#   make                      # native build / QEMU x86
#   make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-   # BeagleBone Black
obj-m := hello.o

KDIR ?= $(HOME)/repos/linux-stable

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

check:
	$(KDIR)/scripts/checkpatch.pl --no-tree -f hello.c
