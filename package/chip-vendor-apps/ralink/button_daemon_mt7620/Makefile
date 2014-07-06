# $Id$

include $(TOPDIR)/rules.mk

#Add for rt_mmap.h, Sean@liteon, 2012-12-28----->
include $(INCLUDE_DIR)/kernel.mk
#<-----Sean@liteon, 2012-12-28

PKG_NAME:=button_daemon
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/button_daemon_mt7620
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=button_daemon_mt7620
  DEPENDS:=@TARGET_ralink
  SUBMENU:=Ralink Application
endef

define Package/button_daemon_mt7620/description
	button_daemon_mt7620	
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -I$(LINUX_DIR)/include"
endef

define Package/button_daemon_mt7620/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/button_daemon.init $(1)/etc/init.d/button_daemon
endef

$(eval $(call BuildPackage,button_daemon_mt7620))
