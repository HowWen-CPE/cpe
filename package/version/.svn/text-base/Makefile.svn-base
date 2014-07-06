# 
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=version
PKG_RELEASE:=1
# we need update version number everytime 
PKG_REBUILD:=y

include $(INCLUDE_DIR)/package.mk

define Package/version
	SECTION:=base
	CATEGORY:=Base system
	TITLE:=Version ID
endef

define Package/version/description
	This package contains an utility to control product version id. 
	$$R means current version in subversion.
endef

define Package/version/config
	source "$(SOURCE)/config/Config.in"
endef

define Build/Compile
	# nothing need to do :(
endef

define Package/version/install
	grep 'CONFIG_TARGET_IMAGE_VERSION' $(TOPDIR)/.config | cut -c29- | sed "s/\"//g" | sed "s/^/GP_SDK: /g" > $(1)/gpinfo
	grep 'CONFIG_VERSION_CONFIG_APPVERSIONID' $(TOPDIR)/.config | cut -c36- | sed "s/\"//g" | sed "s/^/GP_APP: /g" >> $(1)/gpinfo
	grep 'CONFIG_VERSION_CONFIG_VERSIONID' $(TOPDIR)/.config | cut -c33- | sed "s/\"//g" | sed "s/^/GP:     /g" >> $(1)/gpinfo
	grep 'CONFIG_VERSION_CONFIG_VERSIONID' $(TOPDIR)/.config | cut -c33- | sed "s/\"//g" > $(1)/version
	$(SED) 's,$$$$R,$(REVISION),g' $(1)/version
	rm -rf $(PKG_BUILD_DIR)/.version*
endef

$(eval $(call BuildPackage,version))
