#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=nvram-driver
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/nvram-driver
  SUBMENU:=Other modules
  TITLE:=Driver for nvram
  AUTOLOAD:=$(call AutoLoad,124,nvramdrv)
  FILES:=$(PKG_BUILD_DIR)/nvramdrv.ko
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(BUILDFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,nvram-driver))
