#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
PKG_NAME:=hcd
PKG_RELEASE:=1
include $(INCLUDE_DIR)/package.mk

define Package/hcd
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Hcd
	SUBMENU:=Liteon Application
        DEPENDS:=+rtk_inbandLib
endef

define Package/hcd/description
	Command line setting support.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/*.c $(PKG_BUILD_DIR)/
	$(CP) ./src/Makefile $(PKG_BUILD_DIR)/
	$(CP) ./src/*.h $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) -Wall -I. \
	-I $(STAGING_DIR)/usr/include \
	-I $(PKG_BUILD_DIR) " \
	INBAND_LIB="-L $(BUILD_DIR)/rtk_inbandLib -lrtk_inbandLib"
endef

define Package/hcd/install
	$(INSTALL_DIR) $(1)/usr/sbin $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/hcd $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,hcd))
