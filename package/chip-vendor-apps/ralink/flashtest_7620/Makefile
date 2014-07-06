# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=flashtest_7620
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/flashtest_7620
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=flashtest_7620
  DEPENDS:=@TARGET_ralink_wp688_wp688_MTK4110_130426
  SUBMENU:=Ralink Application
endef

define Package/flashtest_7620/description
	flashtest_7620
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS)"		
endef

define Package/flashtest_7620/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/flashtest_7620 $(1)/usr/bin/
endef

$(eval $(call BuildPackage,flashtest_7620))
