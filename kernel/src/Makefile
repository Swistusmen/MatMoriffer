obj-m:=matmoriffer.o
module_control-objs := network_operations.o message_buffer.o

all:
		make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
		make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean