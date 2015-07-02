#
# Copyright (C) 2009-2010 Jo-Philipp Wich <xm@subsignal.org>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wlan-middleware
PKG_RELEASE:=9

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/wlan-middleware
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=wlan middleware lib
	DEPENDS:=+nvram
	SUBMENU:=Liteon Application
endef

define Package/wlan-middleware/description
 This package contains an utility to manipulate NVRAM on Broadcom based devices.
 It works on bcm47xx (Linux 2.6) without using the kernel api.
endef

define Package/wlan-middleware/config
	source "$(SOURCE)/Config.in"
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/*.c $(PKG_BUILD_DIR)/
	$(CP) ./src/$(CONFIG_WLAN_CHIP_VENDOR)/*.c $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/include
	$(CP) ./include/*.h $(PKG_BUILD_DIR)/include
	$(CP) ./include/$(CONFIG_WLAN_CHIP_VENDOR)/*.h $(PKG_BUILD_DIR)/include

	$(CP) ./src/Makefile $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. \
		-I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) \
		-I $(BUILD_DIR)/nvram \
		$(EZP_CFLAGS) \
		-D $(CONFIG_PRODUCT_NAME) \
		-D $(CONFIG_WLAN_CHIP_VENDOR)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/* $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libwlan-middleware.so $(1)/usr/lib
endef

define Package/wlan-middleware/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwlan-middleware.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/etc/wl
	$(INSTALL_BIN) ./files/$(CONFIG_WLAN_CHIP_VENDOR)/* $(1)/etc/wl
	$(LN) /tmp/RT2860AP.dat $(1)/etc/wl/RT2860AP.dat
endef

$(eval $(call BuildPackage,wlan-middleware))
