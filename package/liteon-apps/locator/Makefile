#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
PKG_NAME:=locator
PKG_RELEASE:=1
include $(INCLUDE_DIR)/package.mk

define Package/locator
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Locator
	SUBMENU:=Liteon Application
	DEPENDS:=+nvram +wlan-middleware
endef

define Package/locator/description
	Utilities to set IP or some ID by MAC layer.
endef

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/nvram -I $(BUILD_DIR)/wlan-middleware" \
	NVRAM_LIB="-L $(STAGING_DIR)/usr/lib -lnvram" \
	WLANMW_LIB="-L $(BUILD_DIR)/wlan-middleware -lwlan-middleware"
endef

define Package/locator/install
	$(INSTALL_DIR) $(1)/usr/sbin $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/locator $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/locator.init $(1)/etc/init.d/locator
endef

$(eval $(call BuildPackage,locator))
