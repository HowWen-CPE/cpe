#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xl2tpd
PKG_VERSION:=1.3.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
# Host seems to be down.
#PKG_SOURCE_URL:=ftp://ftp.xelerance.com/xl2tpd/
PKG_MD5SUM:=2f526cc0c36cf6d8a74f1fb2e08c18ec
PKG_SOURCE_URL:=http://pkgs.fedoraproject.org/lookaside/pkgs/xl2tpd/xl2tpd-$(PKG_VERSION).tar.gz/$(PKG_MD5SUM)/

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/xl2tpd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=An L2TP (Layer 2 Tunneling Protocol) daemon
  URL:=http://www.xelerance.com/software/xl2tpd/
  SUBMENU:=VPN
  DEPENDS:=+ppp-mod-pppol2tp +ip +resolveip
endef

define Package/xl2tpd/description
l2tpd is the open source implementation of the L2TP tunneling protocol (RFC2661).
It does implement both LAC and LNS role in a L2TP networking architecture. The
main goal of this protocol is to tunnel PPP frame trough an IP network.
endef

# XXX: CFLAGS are already set by Build/Compile/Default
MAKE_FLAGS+= \
	OFLAGS=""

define Package/xl2tpd/conffiles
/etc/xl2tpd/xl2tpd.conf
/etc/xl2tpd/xl2tp-secrets
/etc/ppp/options.xl2tpd
/etc/ipsec.conf
/etc/ipsec.secrets
endef

define Package/xl2tpd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/local/sbin/xl2tpd $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/local/sbin/xl2tpd-control $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/l2tpClient/ifup.l2tp $(1)/sbin/
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/l2tpServer/xl2tpd.init $(1)/etc/init.d/xl2tpd

	$(INSTALL_DIR) $(1)/etc/xl2tpd/l2tpClient/
	$(INSTALL_DATA) ./files/l2tpClient/xl2tpd_client.conf_template $(1)/etc/xl2tpd/l2tpClient/
	$(INSTALL_CONF) ./files/xl2tp-secrets $(1)/etc/xl2tpd/
	$(INSTALL_CONF) ./files/l2tpClient/xl2tpd_client.conf $(1)/etc/xl2tpd/l2tpClient/

	$(INSTALL_DIR) $(1)/etc/xl2tpd/l2tpServer/
	$(INSTALL_CONF) ./files/xl2tp-secrets $(1)/etc/xl2tpd/
	$(INSTALL_CONF) ./files/l2tpServer/xl2tpd_server.conf $(1)/etc/xl2tpd/l2tpServer/
	
	$(INSTALL_DIR) $(1)/etc/ppp
	$(INSTALL_DATA) ./files/chap-secrets $(1)/etc/ppp/
	$(INSTALL_DATA) ./files/pap-secrets $(1)/etc/ppp/
	
	$(INSTALL_DIR) $(1)/etc/ppp/l2tpClient/
	$(INSTALL_DATA) ./files/l2tpClient/options.xl2tpd_client_template $(1)/etc/ppp/l2tpClient/
	$(INSTALL_DATA) ./files/l2tpClient/options.xl2tpd_client $(1)/etc/ppp/l2tpClient/
	
	$(INSTALL_DIR) $(1)/etc/ppp/l2tpServer/
	$(INSTALL_DATA) ./files/l2tpServer/options.xl2tpd_server $(1)/etc/ppp/l2tpServer/

	#$(INSTALL_DIR) $(1)/lib/netifd/proto
	#$(INSTALL_BIN) ./files/l2tp.sh $(1)/lib/netifd/proto
	ln -sf /tmp/ipsec.conf $(1)/etc/ipsec.conf
	ln -sf /tmp/ipsec.secrets $(1)/etc/ipsec.secrets
	ln -sf /tmp/chap-secrets $(1)/etc/ppp/chap-secrets
	ln -sf /tmp/pap-secrets $(1)/etc/ppp/pap-secrets
	ln -sf /tmp/xl2tpd_client.conf $(1)/etc/xl2tpd/l2tpClient/xl2tpd_client.conf
	ln -sf /tmp/options.xl2tpd_client $(1)/etc/ppp/l2tpClient/options.xl2tpd_client
	ln -sf /tmp/xl2tpd_server.conf $(1)/etc/xl2tpd/l2tpServer/xl2tpd_server.conf
	ln -sf /tmp/options.xl2tpd_server $(1)/etc/ppp/l2tpServer/options.xl2tpd_server
endef

$(eval $(call BuildPackage,xl2tpd))
