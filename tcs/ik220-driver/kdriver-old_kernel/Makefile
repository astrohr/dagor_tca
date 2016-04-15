
ifneq ($(KERNELRELEASE),)
obj-m	:= ik220.o

else
KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR    := /lib/modules/2.6.17.11/build
PWD		:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
endif

clean:
	rm -rf *.o *.ko
