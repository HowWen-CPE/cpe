#
# Makefile for tracing module
#

ifeq ($(obj),)
obj=	.
endif


DEPTH=	..
ifndef TOPDIR
TOPDIR=$(LINUX_SRC)
endif


obj-m		+= preip_host_module.o
#preip-objs	:= preip_host_module.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= preip_host_module.o
list-multi	:= preip_host_module.o
endif


INCS += -I.

EXTRA_CFLAGS+=$(INCS) ${COPTS} -g
ifeq ($(DEBUG_BUILD),1)
EXTRA_CFLAGS+=-DDEBUG
endif

#KERNELPATH=$(LINUX_DIR)


APP_INSTALL_ROOT=$(ROOTFS_PATH)/sbin/

#all:
preip:
	$(CC) $(CFLAGS) -o preip_host preip_host.c -lpthread
	$(MAKE)	-C ${KERNELPATH} SUBDIRS=$(shell pwd) modules

#install:
#		$(call install_module,preip.ko)
#		$(call install_program,preipd,$(APP_INSTALL_ROOT))
#		$(call install_program,testapi,$(APP_INSTALL_ROOT))

clean:
	$(call clean_module,$(shell pwd))
	rm -rf preip_host preip_host_module.ko
		

