obj-m += helloworld_rpi.o helloworld_with_parameters.o helloworld_with_timing.o

KERNEL_DIR ?= $(PWD)/../linux
TOOLS_DIR ?= $(PWD)/../tools

all: env
	make -C $(KERNEL_DIR) \
			SUBDIRS=$(PWD) modules

clean: env
	make -C $(KERNEL_DIR) \
			SUBDIRS=$(PWD) clean

deploy:
	scp *.ko root@192.168.2.28:/data

env:
	export PATH=$(TOOLS_DIR)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin:$(PATH)
	export TOOLCHAIN=$(TOOLS_DIR)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/
	export CROSS_COMPILE=arm-linux-gnueabihf-
	export ARCH=arm
