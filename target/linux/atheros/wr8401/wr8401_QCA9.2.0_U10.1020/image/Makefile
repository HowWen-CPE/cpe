#
# Copyright (C) 2013 Sean@liteon.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

TARGET_IMAGE_VERSION:=$(call qstrip,$(CONFIG_TARGET_IMAGE_VERSION))

IMGNAME:=$(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)


define imgname
$(IMGNAME)-$(2)-$(patsubst jffs2-%,jffs2,$(1))
endef

VMLINUX:=$(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)-vmlinux
UIMAGE:=$(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)-uImage

IMGNAME_INFO:=$(lastword $(subst /, ,$(UIMAGE)))

define Image/BuildKernel
	cp $(KDIR)/vmlinux $(VMLINUX).bin
	$(STAGING_DIR_HOST)/bin/lzma e $(KDIR)/vmlinux -lc1 -lp2 -pb2 $(KDIR)/vmlinux.bin.l7
	dd if=$(KDIR)/vmlinux.bin.l7 of=$(VMLINUX).lzma conv=sync
endef

define Image/Build
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(IMGNAME)-root.$(1) bs=64k conv=sync

	mkimage -A mips -O linux -T kernel -a 0x80002000 -C lzma -e \
		$(shell readelf -h $(LINUX_DIR)/vmlinux 2>/dev/null | grep "Entry" | awk '{print
 $$4}')\
		-n '$(IMGNAME_INFO)-lzma.bin' \
		-d $(VMLINUX).lzma $(UIMAGE)-lzma.bin
endef


$(eval $(call BuildImage))
