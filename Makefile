obj-m += ofd.o
obj-m += myfirst.o
obj-m += first_char_driver.o

PWD := $(shell pwd) 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
