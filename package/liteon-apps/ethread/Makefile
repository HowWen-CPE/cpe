#
# Copyright (C) 2008-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ethread
PKG_VERSION:=1.0
PKG_RELEASE:=1


PKG_MAINTAINER:=Roger Zhang(roger.zhang@liteon.com)


include $(INCLUDE_DIR)/package.mk

define Package/ethread
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=C library for  ethernet port status
  SUBMENU:=Liteon Application
endef


TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include -D ${CONFIG_PRODUCT_NAME}
TARGET_LDFLAGS += -L$(STAGING_DIR)/usr/lib

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -I$(LINUX_DIR)/include"
endef

define Package/ethread/install
	$(INSTALL_DIR) $(1)/lib
	$(CP) $(PKG_BUILD_DIR)/libethread.so* $(1)/lib/
#	$(INSTALL_DIR) $(1)/usr/sbin
#	$(CP) $(PKG_BUILD_DIR)/ethread $(1)/usr/sbin/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/ethread.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libethread.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,ethread))
