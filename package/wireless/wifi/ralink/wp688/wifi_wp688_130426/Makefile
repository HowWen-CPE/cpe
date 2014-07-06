#
# Copyright (C) Sean@liteon.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#


include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mt7620_wifi2716_all_dpa_20130426
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.bz2
PKG_MD5SUM:=d39299a0acdf5ea83522c8bbdc0d1fde


include $(INCLUDE_DIR)/package.mk

MADWIFI_AUTOLOAD:= rt2860v2_ap

define KernelPackage/wifi_wp688_130426
  SUBMENU:=Wireless Drivers
  TITLE:=Driver for wp688 wireless
  DEPENDS:=@TARGET_ralink_wp688_wp688_MTK4110_130426
  FILES:=$(PKG_BUILD_DIR)/rt2860v2_ap/rt2860v2_ap.$(LINUX_KMOD_SUFFIX)
  MAINTAINER:=Liteon NABU BJRD PT <Sean@liteon.com>
  AUTOLOAD:=$(call AutoLoad,25,$(MADWIFI_AUTOLOAD))
  MENU:=1
endef

define KernelPackage/wifi_wp688_130426/description
 This package contains a driver for wp688 Ralink wifi driver.
endef

MAKE_ARGS:=\
	$(TARGET_CONFIGURE_OPTS) \
        SUBDIRS=$(PKG_BUILD_DIR)/rt2860v2_ap \
	MTK_WIFI_DIR=$(PKG_NAME) 

define Build/Compile
	$(MAKE) -C $(LINUX_DIR) $(MAKE_ARGS) modules
endef

define KernelPackage/wifi_wp688_130426/install
endef

$(eval $(call KernelPackage,wifi_wp688_130426))
