# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=flash
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/flash
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=flash
  SUBMENU:=Liteon Application
endef

define Package/flash/description
	flash utility
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -I$(LINUX_DIR)/include"
endef

define Package/flash/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin
endef

$(eval $(call BuildPackage,flash))
