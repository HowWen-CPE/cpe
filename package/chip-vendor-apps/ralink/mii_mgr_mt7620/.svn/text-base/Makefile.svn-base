# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk


PKG_NAME:=mii_mgr
EXEC:=mii_mgr
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/mii_mgr_mt7620
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=mii_mgr 
  DEPENDS:=@TARGET_ralink_wp688_wp688_MTK4110_130426
  SUBMENU:=Ralink Application
endef

define Package/mii_mgr_mt7620/description
	mii_mgr for mt7620
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -I$(LINUX_DIR)/include -I$(LINUX_DIR)/drivers/net/raeth"
endef

define Package/mii_mgr_mt7620/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(EXEC) $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,mii_mgr_mt7620))
