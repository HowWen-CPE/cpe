#
# Copyright (C) 2009-2010 Jo-Philipp Wich <xm@subsignal.org>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=spectral_tpe
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/spectral_tpe
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=spectral from tpe
  #MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
  DEPENDS:=@PACKAGE_kmod-wifi_wp838_QCA_10.1.436
endef

define Package/spectral_tpe/description
 This package contains an spectral utilities.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)\
	CC="$(TARGET_CC)" \
	AR=$(TARGET_CROSS)ar\
	LD=$(TARGET_CROSS)ld 	
endef
	

define Package/spectral_tpe/install
       $(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ssd $(1)/sbin/	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/athssd $(1)/sbin/
endef

$(eval $(call BuildPackage,spectral_tpe))
