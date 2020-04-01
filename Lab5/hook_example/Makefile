ifneq ($(KERNELRELEASE),)
obj-m :=hello.o
else
KDIR :=/lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules
	gcc test.c -o test
clean:
	rm -rf *.ko *.o *.mod.o *.mod.c *.symvers *.order .hello.* .tmp_versions test
endif
