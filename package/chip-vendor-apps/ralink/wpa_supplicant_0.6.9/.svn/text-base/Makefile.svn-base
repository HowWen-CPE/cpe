#
# Copyright (C) 2008-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wpa_supplicant
PKG_VERSION:=0.6.9
PKG_RELEASE:=2
#PKG_MD5SUM:=0efb8fcedf0a8acf6f423dfdb0658fdd

PKG_SOURCE_URL:=http://hostap.epitest.fi/releases
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2


include $(INCLUDE_DIR)/package.mk

define Package/wpa-supplicant-0.6.9
  SECTION:=net
  CATEGORY:=Network
  TITLE:=WPA Supplicant
  #DEPENDS:=$(if $(CONFIG_WPA_SUPPLICANT_OPENSSL),+libopenssl) 
  DEPENDS:=+libopenssl @TARGET_ralink
  URL:=http://hostap.epitest.fi/wpa_supplicant/
endef

define Package/wpa-supplicant-0.6.9/Description
 WPA Supplicant
endef

define Package/wpa-supplicant-0.6.9/config
	source "$(SOURCE)/Config.in"	
endef

define Package/wpa-cli-0.6.9
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=wpa-supplicant-0.6.9
  TITLE:=WPA Supplicant command line interface
endef

define Package/wpa-cli-0.6.9/Description
 WPA Supplicant control utility
endef

CONFIG=$(firstword $(wildcard ./files/config.$(BOARD) ./files/config.$(ARCH) ./config))

define Build/Configure
	cp $(CONFIG) $(PKG_BUILD_DIR)/wpa_supplicant/.config
	$(if $(CONFIG_WPA_SUPPLICANT_OPENSSL),$(SED) 's|^CONFIG_TLS.*|CONFIG_TLS=openssl|g' $(PKG_BUILD_DIR)/wpa_supplicant/.config)
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	CFLAGS="$(TARGET_CFLAGS) -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR)" \
	$(MAKE) -C $(PKG_BUILD_DIR)/wpa_supplicant \
		CC="$(TARGET_CC)" \
		KERNEL=$(LINUX_DIR) \
		CPPFLAGS="$(TARGET_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		all
endef

define Package/wpa-supplicant-0.6.9/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) \
		$(PKG_BUILD_DIR)/wpa_supplicant/wpa_supplicant \
		$(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_DATA) ./files/wpa_supplicant.sh $(1)/lib/wifi/wpa_supplicant.sh
endef

define Package/wpa-cli-0.6.9/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/wpa_supplicant/wpa_cli $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,wpa-supplicant-0.6.9))
$(eval $(call BuildPackage,wpa-cli-0.6.9))
