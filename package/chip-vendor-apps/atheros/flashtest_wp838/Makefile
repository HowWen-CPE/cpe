# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=flashtest_wp838
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/flashtest_wp838
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=flashtest_wp838
  DEPENDS:=@TARGET_atheros_wp838_wp838_QCA10_1_436
  SUBMENU:=Ralink Application
endef

define Package/flashtest_wp838/description
	flashtest_wp838
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS)"		
endef

define Package/flashtest_wp838/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/flashtest_wp838 $(1)/usr/bin/
endef

$(eval $(call BuildPackage,flashtest_wp838))
