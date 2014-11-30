#
# Copyright (C) 2009-2010 Jo-Philipp Wich <xm@subsignal.org>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rtk_inbandLib
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/rtk_inbandLib
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=rtk_inbandLib lib
	SUBMENU:=Liteon Application
endef

define Package/rtk_inbandLib/description
 This package contains an utility to communicate network 2 layer.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/*.c $(PKG_BUILD_DIR)/
	$(CP) ./src/Makefile $(PKG_BUILD_DIR)/
	$(CP) ./src/*.h $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -D $(CONFIG_PRODUCT_NAME)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/*.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/librtk_inbandLib.so $(1)/usr/lib
endef

define Package/rtk_inbandLib/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/librtk_inbandLib.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,rtk_inbandLib))
