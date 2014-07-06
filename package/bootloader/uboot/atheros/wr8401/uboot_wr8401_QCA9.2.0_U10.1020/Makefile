#
# Copyright (C) Sean@liteon.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot_QCA9.2.0_U10.1020
PKG_RELEASE:=v0.1.1

PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_MD5SUM:=2a732ebd37856ead97bd25332f71c0a0


PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)

UBOOT_BIN:=uboot

include $(INCLUDE_DIR)/package.mk

define Package/uboot_wr8401_QCA9.2.0_U10.1020
  SECTION:=boot
  CATEGORY:=Boot Loaders
  TITLE:=U-Boot for target Atheros
  SUBMENU:=uboot
  DEPENDS:=@TARGET_atheros_wr8401_wr8401_QCA9_2_0_U10_1020
  MAINTAINER:=Liteon NABU BJRD PT <Sean@liteon.com>
endef

define Package/uboot_wr8401_QCA9.2.0_U10.1020/description
 This package contains an u-boot for wr8401.
endef

# UBOOT_GCC_4_3_3_EXTRA_CFLAGS is important.
UBOOT_PARA:=\
    UBOOT_GCC_4_3_3_EXTRA_CFLAGS=-fPIC \
    ETH_CONFIG=_s17 \
    BOOT_FROM_NAND=1 \
    ATH_CAL_NAND_PARTITION="caldata" \
    ATH_DUAL_FLASH=1

UBOOT_COMMON_OPTS:=\
    ARCH=mips \
    $(UBOOT_PARA)

UBOOT_MAKE_OPTS:=\
    $(UBOOT_COMMON_OPTS)\
    CROSS_COMPILE=$(TARGET_CROSS)

ROM_BOOT_DRV_OPTS:=\
    TOPDIR=$(PKG_BUILD_DIR)/rom-boot-drv/ \
    TOOLPATH=/opt/atheros_gcc-4.3.3/staging_dir/usr \
    KERNELPATH=$(LINUX_DIR) \
    UTILPATH=$(PKG_BUILD_DIR)/utility/bin \
    PATH:=$(TOPDIR)/tools:$(TOOLPATH)/bin:$(TOPDIR)/src:${PATH}

define Build/Compile/u-boot/mrproper
	$(MAKE) -C $(PKG_BUILD_DIR)/u-boot $(UBOOT_MAKE_OPTS) mrproper
endef

define Build/Compile/u-boot
        $(MAKE) -C $(PKG_BUILD_DIR)/u-boot $(UBOOT_PARA) $(1)
endef

define Build/Compile/utility
	$(MAKE) -C $(PKG_BUILD_DIR)/utility/$(1)
endef

define Build/Compile/rom-boot-drv
	$(MAKE) -C $(PKG_BUILD_DIR)/rom-boot-drv $(UBOOT_PARA) $(ROM_BOOT_DRV_OPTS) clean
	$(MAKE) -C $(PKG_BUILD_DIR)/rom-boot-drv $(UBOOT_PARA) $(ROM_BOOT_DRV_OPTS) all
endef

define Build/Compile/uboot-tool
	@echo "Making u-boot tool"
	$(call Build/Compile/u-boot/mrproper)
	$(call Build/Compile/u-boot,db12x_config)
	$(call Build/Compile/u-boot,tools)
endef

define Build/Compile/uboot-mainline
	@echo "Making u-boot image"
	$(MAKE) -C $(PKG_BUILD_DIR)/u-boot $(UBOOT_COMMON_OPTS) db12x_config
	$(MAKE) -C $(PKG_BUILD_DIR)/u-boot $(UBOOT_COMMON_OPTS) clean
	$(MAKE) -C $(PKG_BUILD_DIR)/u-boot $(UBOOT_COMMON_OPTS)
endef	

define Build/Compile/stage1
	@echo "Making 1st stage f/w ..."
	$(call Build/Compile/rom-boot-drv)
endef

define Build/Compile/stage2
	$(call Build/Compile/utility,checksum)
	$(call Build/Compile/utility,bin2hex)
	$(call Build/Compile/utility,binfile)
	$(call Build/Compile/utility,imagealign)
	$(call Build/Compile/stage1)
	@echo "Combining 1st and 2nd stage f/w ..."
	cd $(PKG_BUILD_DIR)/rom-boot-drv && \
	./mk2stage -2 $(PKG_BUILD_DIR)/u-boot/u-boot.bin 
endef

define Build/Compile
	$(call Build/Compile/uboot-tool)
	$(call Build/Compile/uboot-mainline)
	$(call Build/Compile/stage2)

	if [ `stat -c%s $(PKG_BUILD_DIR)/rom-boot-drv/2fw.bin` -gt 524288 ] ;\
	then \
		echo "u-boot.bin file has exceeded 512KB in size."; exit 1; \
	fi
endef

define Package/uboot_wr8401_QCA9.2.0_U10.1020/install
	$(CP) $(PKG_BUILD_DIR)/rom-boot-drv/2fw.bin $(BIN_DIR)/$(BOARDNAME)-$(PKG_RELEASE)-$(UBOOT_BIN)-2fw.bin
endef

$(eval $(call BuildPackage,uboot_wr8401_QCA9.2.0_U10.1020))

