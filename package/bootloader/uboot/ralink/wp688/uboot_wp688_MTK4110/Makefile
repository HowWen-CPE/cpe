#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot_MTK4110
PKG_RELEASE:=v0.1.3

LITEON_SRC:=0
RALINK_SDK:=0

PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_SOURCE_URL:=http://ptnas/sources
PKG_MD5SUM:=1ebb55b03ec8b863e6260852a42f8772


PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/uboot_wp688_MTK4110
  SECTION:=boot
  CATEGORY:=Boot Loaders
  TITLE:=U-Boot in SDK4110 for target MT7620
  SUBMENU:=uboot
  DEPENDS:=@TARGET_ralink
endef

define Package/uboot_wp688_MTK4110/description
 This package contains an u-boot version 4110 for all platform
endef

UBOOT_MAKE_OPTS:=\
    CROSS_COMPILE=$(TARGET_CROSS) \
    PLATFORM_CPU=mips32r2 \
    LITEON_FLAGS=$(LITEON_SRC_FLAG) 

ifneq ($(findstring LITEON_SRC_VALID,$(TARGET_CC)),)
LITEON_SRC:=1
else
RALINK_SDK:=1
endif

define Build/Compile
	if [ $(LITEON_SRC) -eq 1 ] ;\
	then \
		echo "copy WP688_AP.config as .config."; \
		$(CP) $(PKG_BUILD_DIR)/config/WP777_AP.config $(PKG_BUILD_DIR)/.config; \
		$(CP) $(PKG_BUILD_DIR)/config/WP777_autoconf.h $(PKG_BUILD_DIR)/autoconf.h; \
		echo "#define LITEON_UBOOT_VERSION \"$(PKG_RELEASE)\"" >> $(PKG_BUILD_DIR)/uboot_version.h; \
	fi
	if [ $(RALINK_SDK) -eq 1 ] ;\
	then \
		echo "copy MT7620_AP.config as .config."; \
		$(CP) $(PKG_BUILD_DIR)/config/MT7620_AP.config $(PKG_BUILD_DIR)/.config; \
		$(CP) $(PKG_BUILD_DIR)/config/MT7620_autoconf.h $(PKG_BUILD_DIR)/autoconf.h; \
	fi
	
	$(MAKE) -f Makefile -C $(PKG_BUILD_DIR) $(UBOOT_MAKE_OPTS) all
	if [ `stat -c%s $(PKG_BUILD_DIR)/uboot.bin` -gt 524288 ] ;\
	then \
		echo "u-boot.bin file has exceeded 512KB in size."; exit 1; \
	fi
endef

define Package/uboot_wp688_MTK4110/install
	$(CP) $(PKG_BUILD_DIR)/uboot.bin $(BIN_DIR)/$(BOARDNAME)-$(PKG_RELEASE)-uboot.bin
endef

$(eval $(call BuildPackage,uboot_wp688_MTK4110))
