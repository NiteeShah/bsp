obj-m += first_char_driver.o
obj-m += first_workqueue.o

PWD := $(shell pwd) 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
