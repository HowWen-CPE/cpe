#
# Copyright (C) Sean@liteon.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#


include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mt7620_wifi2714_all_dpa_20130218
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.bz2
PKG_MD5SUM:=436fbb7846c1465693781888fae0e04b


include $(INCLUDE_DIR)/package.mk

define KernelPackage/wifi_wp688_130218
  SUBMENU:=Wireless Drivers
  TITLE:=Driver for wp688 wireless
  DEPENDS:=@TARGET_ralink_wp688_wp688_MTK4110_130218
  FILES:=$(PKG_BUILD_DIR)/rt2860v2_ap/rt2860v2_ap.$(LINUX_KMOD_SUFFIX)
  MAINTAINER:=Liteon NABU BJRD PT <Sean@liteon.com>
  AUTOLOAD:=$(call AutoLoad,25,$(MADWIFI_AUTOLOAD))
  MENU:=1
endef

define KernelPackage/wifi_wp688_130218/description
 This package contains a driver for wp688 Ralink wifi driver.
endef

MAKE_ARGS:=\
	$(TARGET_CONFIGURE_OPTS) \
        SUBDIRS=$(PKG_BUILD_DIR)/rt2860v2_ap \
	MTK_WIFI_DIR=$(PKG_NAME) 

define Build/Compile
	$(MAKE) -C $(LINUX_DIR) $(MAKE_ARGS) modules
endef

define KernelPackage/wifi_wp688_130218/install
endef

$(eval $(call KernelPackage,wifi_wp688_130218))
