#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

TARGET_IMAGE_VERSION:=$(call qstrip,$(CONFIG_TARGET_IMAGE_VERSION))

UBOOT_NAME:=$(IMG_PREFIX)-v0.1.3
PRODUCT_NAME:=$(call qstrip,$(CONFIG_PRODUCT_NAME))
PRODUCT_VERSION:=$(call qstrip,$(CONFIG_VERSION_CONFIG_VERSIONID))

IMGNAME:=$(BIN_DIR)/$(PRODUCT_NAME)-$(PRODUCT_VERSION)

FLASHNAME:=$(BIN_DIR)/$(PRODUCT_NAME)_$(PRODUCT_VERSION)_Flash.bin

IMGNAME_INFO:=$(lastword $(subst /, ,$(IMGNAME)))

export BIN_DIR
export UBOOT_NAME
export PRODUCT_NAME
export IMGNAME
export FLASHNAME

define imgname
$(IMGNAME)-$(2)-$(patsubst jffs2-%,jffs2,$(1))
endef

define add_jffs2_mark
	echo -ne '\xde\xad\xc0\xde' >> $(1)
endef

VMLINUX:=$(IMGNAME)-vmlinux
ifeq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),y)
	VMLINUX:=$(IMGNAME)-vmlinux-initramfs
endif

define Image/BuildKernel
	cp $(KDIR)/vmlinux $(VMLINUX).bin
	$(STAGING_DIR_HOST)/bin/lzma e $(KDIR)/vmlinux -lc1 -lp2 -pb2 $(KDIR)/vmlinux.bin.l7
	dd if=$(KDIR)/vmlinux.bin.l7 of=$(VMLINUX).lzma conv=sync
endef

define Image/Build/squashfs
    $(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
endef


# Add deadc0de after mkimage is done.
# The firmware is also round-up of 64KB in terms of size.
define Image/Build
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(IMGNAME)-root.$(1) bs=64k conv=sync

	mkimage -A mips -O linux -T kernel -a 0x80000000 -C lzma -e \
		$(shell readelf -h $(LINUX_DIR)/vmlinux 2>/dev/null | grep "Entry" | awk '{print $$4}') \
		-n '$(IMGNAME_INFO)-uImage-$(1)-lzma.bix' \
		-d $(VMLINUX).lzma \
		-o $(CONFIG_MTD_KERNEL_PART_SIZ) -f $(KDIR)/root.squashfs $(IMGNAME)-uImage-$(1)-lzma.bix
	mv $(IMGNAME)-uImage-$(1)-lzma.bix $(IMGNAME)-uImage-$(1)-lzma-deadc0de.bix
	dd if=$(IMGNAME)-uImage-$(1)-lzma-deadc0de.bix of=$(IMGNAME)-uImage-$(1)-lzma.bix bs=64k conv=sync	
	rm -rf $(IMGNAME)-uImage-$(1)-lzma-deadc0de.bix    

	mkimage -A mips -O linux -T kernel -a 0x80000000 -C lzma -e \
			$(shell readelf -h $(LINUX_DIR)/vmlinux 2>/dev/null | grep "Entry" | awk '{print $$4}') \
			-n '$(IMGNAME_INFO)-uImage-$(1)-test.bix' \
			-d $(VMLINUX).lzma \
			-f $(KDIR)/root.squashfs $(IMGNAME)-uImage-$(1)-test.bix
	liteonimage -d $(IMGNAME)-uImage-$(1)-test.bix $(IMGNAME)-uImage-$(1)-lzma-web.bin
	rm -rf $(IMGNAME)-uImage-$(1)-test.bix

#	@sh $(PWD)/image/gen_flash.sh
	@sh $(PWD)/$(SUBTARGETS)/image/gen_flash.sh
endef

define Image/Build-initramfs
	mkimage -A mips -O linux -T kernel -a 0x80000000 -C lzma -e \
		$(shell readelf -h $(LINUX_DIR)/vmlinux 2>/dev/null | grep "Entry" | awk '{print $$4}') -p \
		-n 'EZPacket MIPS Linux-$(LINUX_VERSION)' \
		-d $(VMLINUX).lzma \
		-c $(CONFIG_EZP_PRODUCT_CATEGORY) -s $(CONFIG_EZP_PRODUCT_SUBCATEGORY) -w $(EZP_VERSION_MAJOR) -y $(EZP_VERSION_MINOR) -z $(EZP_VERSION_SUBMINOR)  $(IMGNAME)-$(KERNEL)-initramfs.bix
	$(call add_jffs2_mark,$(IMGNAME)-$(KERNEL)-initramfs.bix)
	mv $(IMGNAME)-$(KERNEL)-initramfs.bix $(IMGNAME)-$(KERNEL)-initramfs-deadc0de.bix
	dd if=$(IMGNAME)-$(KERNEL)-initramfs-deadc0de.bix of=$(IMGNAME)-$(KERNEL)-initramfs.bix bs=64k conv=sync
	rm -rf $(IMGNAME)-$(KERNEL)-initramfs-deadc0de.bix
endef

$(eval $(call BuildImage))
