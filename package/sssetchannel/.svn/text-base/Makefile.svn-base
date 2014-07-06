#
# Copyright (C) 2009-2010 Jo-Philipp Wich <xm@subsignal.org>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sssetchannel
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/sssetchannel
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=sssetchannel for spectral
  #MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
  #DEPENDS:=@TARGET_brcm47xx||@TARGET_ar71xx
endef

define Package/sssetchannel/description
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
	

define Package/sssetchannel/install
       $(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sssetchannel $(1)/sbin/		
endef

$(eval $(call BuildPackage,sssetchannel))
