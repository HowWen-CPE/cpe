# $Id$

include $(TOPDIR)/rules.mk

include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sw_rst_daemon
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/sw_rst_daemon
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=sw_rst_daemon
  DEPENDS:=@TARGET_atheros_wp838_wp838_QCA10_1_436
  SUBMENU:=Liteon Application
endef

define Package/sw_rst_daemon/description
	sw_rst_daemon	
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -I$(LINUX_DIR)/include"
endef

define Package/sw_rst_daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sw_rst_daemon $(1)/usr/bin/
endef

$(eval $(call BuildPackage,sw_rst_daemon))
