obj-m += helloworld_rpi.o helloworld_with_parameters.o helloworld_with_timing.o

SHELL=/bin/bash
KERNEL_DIR ?= $(PWD)/../linux
TOOLS_DIR ?= $(PWD)/../tools

TMP_PATH:=$(TOOLS_DIR)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin:$(PATH)

export PATH=$(TMP_PATH)
export TOOLCHAIN=$(TOOLS_DIR)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm

all:
	make -C $(KERNEL_DIR) \
			SUBDIRS=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) \
			SUBDIRS=$(PWD) clean

deploy:
	scp *.ko root@192.168.2.28:/data

