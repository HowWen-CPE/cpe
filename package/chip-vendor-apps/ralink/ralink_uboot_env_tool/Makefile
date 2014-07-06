#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ralink_uboot_env_tool
PKG_VERSION:=0.1
#PKG_RELEASE:=mt7620

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/ralink_uboot_env_tool
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=read/modify U-Boot enviroment 
  DEPENDS:=@TARGET_ralink +zlib
  SUBMENU:=Ralink Application
endef

define Package/ralink_uboot_env_tool/description
 This package includes tools to read and modify Ralink U-Boot enviroment.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

#To port this package to other project using native toolchain, comment compile section below.
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(LINUX_DIR)/include "
endef

define Package/ralink_uboot_env_tool/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/print_env $(1)/usr/sbin/
	ln -sf print_env $(1)/usr/sbin/set_env
	$(CP) $(PKG_BUILD_DIR)/board_data.config $(1)/etc
endef

$(eval $(call BuildPackage,ralink_uboot_env_tool))
