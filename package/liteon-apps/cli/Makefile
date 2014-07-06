#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
PKG_NAME:=cli
PKG_RELEASE:=1
include $(INCLUDE_DIR)/package.mk

define Package/cli
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Cli
	SUBMENU:=Liteon Application
	DEPENDS:=+nvram +wlan-middleware +libreadline +libncurses +ethread
endef

define Package/cli/description
	Command line setting support.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/*.c $(PKG_BUILD_DIR)/
	$(CP) ./src/Makefile $(PKG_BUILD_DIR)/
	mkdir -p $(PKG_BUILD_DIR)/clicbk
	$(CP) ./src/clicbk/* $(PKG_BUILD_DIR)/clicbk
	mkdir -p $(PKG_BUILD_DIR)/include
	$(CP) ./src/include/* $(PKG_BUILD_DIR)/include
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) -Wall -I. \
	-I $(STAGING_DIR)/usr/include \
	-I $(PKG_BUILD_DIR) \
	-I $(PKG_BUILD_DIR)/clicbk \
	-I $(BUILD_DIR)/nvram -I $(BUILD_DIR)/wlan-middleware \
	-I $(BUILD_DIR)/readline-5.2 -D $(CONFIG_PRODUCT_NAME)" \
	NVRAM_LIB="-L $(STAGING_DIR)/usr/lib -lnvram" \
	ETHREAD_LIB="-L $(STAGING_DIR)/usr/lib -lethread" \
	WLANMW_LIB="-L $(BUILD_DIR)/wlan-middleware -lwlan-middleware"\
	READLINE_LIB="-L $(BUILD_DIR)/readline-5.2/shlib -lreadline \
	-L $(STAGING_DIR)/usr/lib -lncurses"
endef

define Package/cli/install
	$(INSTALL_DIR) $(1)/usr/sbin $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/cli $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,cli))
