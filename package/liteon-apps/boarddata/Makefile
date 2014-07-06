#
# Copyright (C) 2008-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=boarddata
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/boarddata
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=boarddata
  	SUBMENU:=Liteon Application
endef

define Package/boarddata/description
	Boarddata API for boarddata partition
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -D $(CONFIG_PRODUCT_NAME)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/boarddata.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libboarddata.so $(1)/usr/lib
endef

define Package/boarddata/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libboarddata.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/boarddata $(1)/usr/sbin/
endef


$(eval $(call BuildPackage,boarddata))
