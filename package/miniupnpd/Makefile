#
# Copyright (C) 2007 EZPacket Technologies
#

include $(TOPDIR)/rules.mk
 
PKG_NAME:=miniupnpd
PKG_VERSION:=1.7
PKG_RELEASE:=0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=http://miniupnp.free.fr/files
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/miniupnpd
    SECTION:=uitls
    CATEGORY:=Network
    TITLE:=upnpd - mini upnp server
    DEPENDS:=+iptables +libip4tc +nvram
endef

define Package/miniupnpd/description
	A very tiny miniupnpd server.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)	-f Makefile.linux \
	$(TARGET_CONFIGURE_OPTS) \
	CFLAGS="-DENABLE_NATPMP -DIPTABLES_143 -DEZP_DEBUG -DENABLE_LIGD_ICON -DENABLE_PNPX \
		$(TARGET_CFLAGS) -I $(STAGING_DIR)/usr/include -I $(LINUX_DIR)/include -I $(BUILD_DIR)/nvram" \
	LIBS="$(STAGING_DIR)/usr/lib/libiptc.a" \
	LDFLAGS="-L../nvram -lnvram" $(PKG_NAME) 
endef

define Package/miniupnpd/install
	$(INSTALL_DIR) $(1)/usr/bin 
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/ 
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/miniupnpd.script $(1)/etc/init.d/upnpd
endef

$(eval $(call BuildPackage,miniupnpd))
