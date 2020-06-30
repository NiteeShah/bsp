obj-m += first_char_driver.o
obj-m += first_workqueue.o
obj-m += first_delayed_work.o
obj-m += first_delayed_workqueue.o
obj-m += first_mutex.o
PWD := $(shell pwd) 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
