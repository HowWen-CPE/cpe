#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=preip
PKG_RELEASE:=1
include $(INCLUDE_DIR)/package.mk

define Package/preip
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=preip
	SUBMENU:=CPE Application
	DEPENDS:=+nvram +wlan-middleware
endef

define Package/preip/description
	Utilities to set IP or some ID by MAC layer.
endef

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/nvram -I $(BUILD_DIR)/wlan-middleware" \
	NVRAM_LIB="-L $(STAGING_DIR)/usr/lib -lnvram" \
	WLANMW_LIB="-L $(BUILD_DIR)/wlan-middleware -lwlan-middleware" \
	KERNELPATH=$(LINUX_DIR)
endef

define Package/preip/install
	$(INSTALL_DIR) $(1)/usr/sbin $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/preipd $(1)/usr/sbin/
	$(CP) $(PKG_BUILD_DIR)/preip.ko $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/preipd.init $(1)/etc/init.d/preipd
endef

$(eval $(call BuildPackage,preip))
