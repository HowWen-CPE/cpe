#
# Copyright (C) 2013 SeanXKong@Liteon.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=athr-hostap
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_MIRROR_MD5SUM:=944337b455c2f338abf8d41f0c8db2f7

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

ATH_HOSTAP:=hostapd hostapd_cli
ATH_WPASUP:=wpa_supplicant wpa_cli

TARGET_CONFIGURE_OPTS+= \
	ATHEROSPATH=$(KERNEL_BUILD_DIR)/wifi_QCA_10.1.436/drivers/wlan_modules

include $(INCLUDE_DIR)/package.mk

define Package/athr-hostap
        SECTION:=net
        CATEGORY:=Network
        TITLE:=IEEE 802.1x Authenticator(Atheros)
        MAINTAINER:=Liteon NABU BJRD PT <SeanXKong@liteon.com>
        DEPENDS:=@TARGET_atheros +wifi_wp838_QCA10.1.436 
endef

define Package/athr-hostap/description
	This package contains a full featured IEEE 802.1x/WPA/EAP/RADIUS
	Authenticator.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/hostapd clean
	$(MAKE) -C $(PKG_BUILD_DIR)/hostapd $(TARGET_CONFIGURE_OPTS)

	$(MAKE) -C $(PKG_BUILD_DIR)/wpa_supplicant clean
	$(MAKE) -C $(PKG_BUILD_DIR)/wpa_supplicant $(TARGET_CONFIGURE_OPTS)
	
endef

define Package/athr-hostap/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(foreach app,$(ATH_HOSTAP),$(PKG_BUILD_DIR)/hostapd/$(app)) $(1)/usr/sbin
	$(CP) $(foreach app,$(ATH_WPASUP),$(PKG_BUILD_DIR)/wpa_supplicant/$(app)) $(1)/usr/sbin
endef

$(eval $(call BuildPackage,athr-hostap))
