#
# Copyright (C) Sean@liteon.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=u-boot-2006-04-06-1725
PKG_RELEASE:=v0.2.5

PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_MD5SUM:=a54a91fc21854bffd927975579f77fdb


PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)

UBOOT_BIN:=uboot

include $(INCLUDE_DIR)/package.mk

define Package/uboot_wp838_QCA10.1.436
  SECTION:=boot
  CATEGORY:=Boot Loaders
  TITLE:=U-Boot for target Atheros
  SUBMENU:=uboot
  DEPENDS:=@TARGET_atheros_wp838_wp838_QCA10_1_436
  MAINTAINER:=Liteon NABU BJRD PT <Sean@liteon.com>
endef

define Package/uboot_wp838_QCA10.1.436/description
 This package contains an u-boot for WP838.
endef

UBOOT_CONFIG:=\
	COMPRESSED_UBOOT=0 \
	ATH_RGMII_CAL=1 \
	ETH_CONFIG=_f1e \
	CFG_BOARD_TYPE=db12x \
	ATH_SGMII_FORCED=1 \
	ATH_S17_PHY0_WAN=1 \
	ATH_S17_MAC0_SGMII=1 \
	AP_SPI_CS2_GPIO=16 \
	FLASH_SIZE=16

UBOOT_PARA:=\
	UBOOT_GCC_4_3_3_EXTRA_CFLAGS=-fPIC \
	$(UBOOT_CONFIG)

UBOOT_COMMON_OPTS:=\
	ARCH=mips \
	$(UBOOT_PARA)


define Build/Compile/u-boot
        $(MAKE) -C $(PKG_BUILD_DIR)/u-boot $(UBOOT_PARA) $(1)
endef

define Build/Compile/uboot-mainline
	@echo "Making u-boot image"
	$(MAKE) -C $(PKG_BUILD_DIR) $(UBOOT_COMMON_OPTS) db12x_config
	$(MAKE) -C $(PKG_BUILD_DIR) $(UBOOT_COMMON_OPTS) clean
	$(MAKE) -C $(PKG_BUILD_DIR) $(UBOOT_COMMON_OPTS)
endef	

define Build/Prepare
	@echo "Copy lib_bootstrap to build directory...."
	$(CP) ./files/* $(PKG_BUILD_DIR)/
	$(call Build/Prepare/Default)
endef

define Build/U-Boot/Version
	echo "/*" > $(PKG_BUILD_DIR)/include/liteon_version.h
	echo " * Automatically generated C config: don't edit " \
		>> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo " */" >> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo " " >> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo "#ifndef __LITEON_VERSION_H__" >> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo "#define __LITEON_VERSION_H__" >> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo "" >> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo "#define LITEON_U_BOOT_VERSION \"Liteon U-Boot Version: $(PKG_RELEASE)\"" \
		>> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo "" >> $(PKG_BUILD_DIR)/include/liteon_version.h
	echo "#endif /* __LITEON_VERSION_H__*/" >> $(PKG_BUILD_DIR)/include/liteon_version.h

endef

define Build/Compile
	$(call Build/U-Boot/Version)
	$(call Build/Compile/uboot-mainline)

	if [ `stat -c%s $(PKG_BUILD_DIR)/u-boot/u-boot.bin` -gt 262144 ] ;\
	then \
		echo "u-boot.bin file has exceeded 256KB in size."; exit 1; \
	fi
endef

define Package/uboot_wp838_QCA10.1.436/install
	$(CP) $(PKG_BUILD_DIR)/u-boot.bin $(BIN_DIR)/$(BOARDNAME)-$(PKG_RELEASE)-$(UBOOT_BIN).bin
endef

$(eval $(call BuildPackage,uboot_wp838_QCA10.1.436))

