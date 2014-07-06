#
# Copyright (C) 2008-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
#
#
PKG_NAME:=firewall-new

PKG_VERSION:=2
PKG_RELEASE:=55.1

include $(INCLUDE_DIR)/package.mk

define Package/firewall-new
  SECTION:=net
  CATEGORY:=Base system
  URL:=http://openwrt.org/
  TITLE:=OpenWrt firewall
  MAINTAINER:=Jo-Philipp Wich <xm@subsignal.org>
  DEPENDS:=+iptables +kmod-ipt-conntrack +kmod-ipt-nat
  PKGARCH:=all
endef

define Package/firewall-new/description
  firewall for OpenWrt
endef

define Build/Compile
	true
endef

define Package/firewall-new/conffiles
	true
endef

define Package/firewall-new/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/bin/ezp-iptables $(1)/sbin
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/network-acl $(1)/etc/init.d/network-acl
#	$(INSTALL_DIR) $(1)/etc/rc.d/
#	$(INSTALL_BIN) ./files/S60network-acl $(1)/etc/rc.d/S60network-acl
#	$(INSTALL_DIR) $(1)/etc/hotplug.d/iface
#	$(INSTALL_DATA) ./files/firewall.hotplug $(1)/etc/hotplug.d/iface/20-firewall
#	$(INSTALL_DIR) $(1)/etc/hotplug.d/firewall
#	$(INSTALL_DATA) ./files/reflection.hotplug $(1)/etc/hotplug.d/firewall/10-nat-reflection
#	$(INSTALL_DIR) $(1)/etc
#	$(INSTALL_DATA) ./files/firewall.user $(1)/etc
#	$(INSTALL_DIR) $(1)/lib/upgrade/keep.d
#	$(INSTALL_DATA) ./files/firewall.upgrade $(1)/lib/upgrade/keep.d/firewall
endef

$(eval $(call BuildPackage,firewall-new))
