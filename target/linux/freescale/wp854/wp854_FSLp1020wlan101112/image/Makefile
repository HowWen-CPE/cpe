#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

TARGET_IMAGE_VERSION:=$(call qstrip,$(CONFIG_TARGET_IMAGE_VERSION))

IMGNAME:=$(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)

VMLINUX:=$(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)-vmlinux
UIMAGE:=$(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)-vmlinux-uImage

IMGNAME_INFO:=$(lastword $(subst /, ,$(UIMAGE)))

define Image/Prepare
	#$(STAGING_DIR_HOST)/bin/dtc -f -b 0 -I dts -O dtb -R 8 -S 0x3000 -o $(BIN_DIR)/WP854.dtb $(LINUX_DIR)/arch/powerpc/boot/dts/WP854.dts
	$(STAGING_DIR_HOST)/bin/dtc -f -b 0 -I dts -O dtb -R 8 -S 0x3000 -o $(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION).dtb $(LINUX_DIR)/arch/powerpc/boot/dts/WP854.dts
endef

define Image/BuildKernel
	cp $(LINUX_DIR)/arch/powerpc/boot/uImage $(UIMAGE)
	cp $(KDIR)/vmlinux $(VMLINUX).bin
	cp $(VMLINUX).bin $(VMLINUX)	
	gzip -f -9  $(VMLINUX)
endef

define Image/Build/squashfs
    $(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
endef

# Add deadc0de after mkimage is done.
# The firmware is also round-up of 64KB in terms of size.
define Image/Build
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(IMGNAME)-root.$(1) bs=64k conv=sync
	
	mkimage -A ppc -O linux -T kernel -C gzip \
	-a $(shell $(TARGET_CROSS)objdump -p $(LINUX_DIR)/vmlinux 2>/dev/null | grep -m 1 LOAD | awk '{print $$7}')\
	-e $(shell $(TARGET_CROSS)objdump -p $(LINUX_DIR)/vmlinux 2>/dev/null | grep -m 1 LOAD | awk '{print $$7}')\
	-n '$(IMGNAME_INFO)-$(1)-gz.bix' \
	-d $(VMLINUX).gz \
	-o 0x300000 -f $(KDIR)/root.squashfs $(IMGNAME)-uImage-$(1)-gz.bix

	$(call add_jffs2_mark,$(IMGNAME)-uImage-$(1)-gz.bix)
	mv $(IMGNAME)-uImage-$(1)-gz.bix $(IMGNAME)-uImage-$(1)-gz-deadc0de.bix
	dd if=$(IMGNAME)-uImage-$(1)-gz-deadc0de.bix of=$(IMGNAME)-uImage-$(1)-gz.bix bs=64k conv=sync
	rm -rf $(IMGNAME)-uImage-$(1)-gz-deadc0de.bix
endef



#define Image/Build
#	$(call Image/Build/$(1),$(1))
#endef

define CheckFilesSize
	if [ `stat -c%s "$(1)"` -gt $(2) ]; then \
		echo "Warning: $(1) is too big"; \
	fi
endef

define Image/Build/cramfs
	$(call prepare_generic_cramfs,$(KDIR)/root.cramfs)
	cp $(KDIR)/root.$(1) $(BIN_DIR)/$(IMG_PREFIX)-root.cramfs
	$(call CheckFileSize, $(LINUX_DIR)/arch/powerpc/boot/uImage, 3145728)
	( \
		dd if=$(LINUX_DIR)/arch/powerpc/boot/uImage bs=3072k conv=sync; \
		dd if=$(KDIR)/root.$(1) bs=256k conv=sync; \
	) > $(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)-system-$(1).img
endef

#define Image/Build/squashfs
#	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
#	cp $(KDIR)/root.$(1) $(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)-root.squashfs
#	$(call CheckFileSize, $(LINUX_DIR)/arch/powerpc/boot/uImage, 3145728)
#	( \
#		dd if=$(LINUX_DIR)/arch/powerpc/boot/uImage bs=3072k conv=sync; \
#		dd if=$(KDIR)/root.$(1) bs=256k conv=sync; \
#	) > $(BIN_DIR)/$(IMG_PREFIX)-$(TARGET_IMAGE_VERSION)-system-$(1).img
#endef

$(eval $(call BuildImage))
