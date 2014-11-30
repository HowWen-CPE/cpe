#
# Makefile for tracing module
#

ifeq ($(obj),)
obj=	.
endif

#include $(SOFT_BUILD_ROOT)/Make.rule
#include $(LITEON_APP_PATH)/rules.gcc

DEPTH=	..
ifndef TOPDIR
TOPDIR=$(LINUX_SRC)
endif

#CFLAGS  += -L $(LITEON_APP_PATH)/wrap -lwrap  

obj-m		+= preip.o
#preip-objs	:= preip.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= preip.o
list-multi	:= preip.o
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
	$(CC) $(CFLAGS) -o preipd preipd.c
	$(CC) $(CFLAGS) -o testapi testapi.c
	$(MAKE)	-C ${KERNELPATH} SUBDIRS=$(shell pwd) modules

#install:
#		$(call install_module,preip.ko)
#		$(call install_program,preipd,$(APP_INSTALL_ROOT))
#		$(call install_program,testapi,$(APP_INSTALL_ROOT))

clean:
	$(call clean_module,$(shell pwd))
	rm -rf preipd
		

