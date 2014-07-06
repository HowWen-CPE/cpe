#
# Copyright (C) 2007-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk


PKG_NAME:=dhcpcd
PKG_VERSION:=3.2.3.orig
PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://ftp.de.debian.org/debian/pool/main/d/dhcpcd/
PKG_MD5SUM:=630ec5c9f31b4b4f02972899ea955a37
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk


define Package/dhcpcd
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=IP Addresses and Names
  TITLE:=DHCP client for automatically configuring IPv4 networking
  URL:=http://roy.marples.name/projects/dhcpcd
endef

define Package/dhcpcd/description
	Simple configuration: supports executions of a script when the
	IP address changes.
endef

define Package/dhcpcd/install
	$(INSTALL_DIR) $(1)/sbin
	$(CP) $(PKG_INSTALL_DIR)/sbin/* $(1)/sbin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) ./files/dhcpcd.sh $(1)/etc/
	$(INSTALL_BIN) ./files/netmask.sh $(1)/etc/
	$(INSTALL_BIN) ./files/dhcpcdstart $(1)/sbin/
endef

$(eval $(call BuildPackage,dhcpcd))

