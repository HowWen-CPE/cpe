#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=net-snmp
PKG_VERSION:=5.7.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/net-snmp
PKG_MD5SUM:=5bddd02e2f82b62daa79f82717737a14

PKG_FIXUP = patch-libtool

PKG_INCLUDE_OPENSSL:=$(BUILD_DIR)/openssl-1.0.1e/

include $(INCLUDE_DIR)/package.mk

define Package/net-snmp/Default
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+nvram
  URL:=http://www.net-snmp.org/
endef

define Package/net-snmp/Default/description
 Simple Network Management Protocol (SNMP) is a widely used protocol for
 monitoring the health and welfare of network equipment (eg. routers),
 computer equipment and even devices like UPSs. Net-SNMP is a suite of
 applications used to implement SNMP v1, SNMP v2c and SNMP v3 using both
 IPv4 and IPv6.
endef


define Package/libnetsnmp
$(call Package/net-snmp/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libelf libopenssl nvram
  TITLE:=Open source SNMP implementation (libraries)
endef

define Package/libnetsnmp/description
$(call Package/net-snmp/Default/description)
 .
 This package contains shared libraries, needed by other programs.
endef


define Package/snmp-utils
$(call Package/net-snmp/Default)
  DEPENDS:=+libnetsnmp libopenssl nvram
  TITLE:=Open source SNMP implementation (utilities)
endef

define Package/snmp-utils/description
$(call Package/net-snmp/Default/description)
 .
 This package contains SNMP client utilities.
endef


define Package/snmpd
$(call Package/net-snmp/Default)
  DEPENDS:=+libnetsnmp libopenssl nvram
  TITLE:=Open source SNMP implementation (daemon)
endef

define Package/snmpd/description
$(call Package/net-snmp/Default/description)
 .
 This package contains the SNMP agent, dynamically linked.
endef


define Package/snmpd-static
$(call Package/net-snmp/Default)
  TITLE:=Open source SNMP implementation (daemon)
  DEPENDS:=+libelf libopenssl nvram 
endef

define Package/snmpd-static/description
$(call Package/net-snmp/Default/description)
 .
 This package contains the SNMP agent, statically linked.
endef


SNMP_MIB_MODULES_INCLUDED = \
	ieee802dot11 \
	mibII/at \
	mibII/icmp \
	mibII/ifTable \
	mibII/ip \
	mibII/snmp_mib \
	mibII/sysORTable \
	mibII/system_mib \
	mibII/tcp \
	mibII/udp \
	mibII/vacm_context \
	mibII/vacm_vars \
	snmpv3/snmpEngine \
	snmpv3/snmpMPDStats \
	snmpv3/usmConf \
	snmpv3/usmStats \
	snmpv3/usmUser \
	tunnel \
	ucd-snmp/disk \
	ucd-snmp/dlmod \
	ucd-snmp/extensible \
	ucd-snmp/loadave \
	ucd-snmp/memory \
	ucd-snmp/pass \
	ucd-snmp/proc \
	ucd-snmp/vmstat \
	util_funcs \
	utilities/execute \

SNMP_MIB_MODULES_EXCLUDED = \
	agent_mibs \
	agentx \
	disman/event \
	disman/schedule \
	hardware \
	host \
	if-mib \
	mibII \
	notification \
	notification-log-mib \
	snmpv3mibs \
	target \
	tcp-mib \
	ucd_snmp \
	udp-mib \
	utilities \

SNMP_TRANSPORTS_INCLUDED = Callback UDP

SNMP_TRANSPORTS_EXCLUDED = TCP TCPIPv6 Unix

TARGET_CFLAGS += $(FPIC)

CONFIGURE_ARGS += \
	--enable-mfd-rewrites \
	--enable-shared \
	--enable-static \
	--with-endianness=little \
	--with-logfile=/var/log/snmpd.log \
	--with-persistent-directory=/usr/share/snmp/ \
	--with-default-snmp-version=3 \
	--with-sys-contact=root@localhost \
	--with-sys-location=Unknown \
	--enable-applications \
	--disable-debugging \
	--disable-manuals \
	--disable-mibs \
	--disable-scripts \
	--with-out-mib-modules="$(SNMP_MIB_MODULES_EXCLUDED)" \
	--with-mib-modules="$(SNMP_MIB_MODULES_INCLUDED)" \
	--with-out-transports="$(SNMP_TRANSPORTS_EXCLUDED)" \
	--with-transports="$(SNMP_TRANSPORTS_INCLUDED)" \
	--with-openssl=$(PKG_INCLUDE_OPENSSL)\
	--without-libwrap \
	--without-rpm \
	--without-zlib \
	 $(call autoconf_bool,CONFIG_IPV6,ipv6) \

ifeq ($(CONFIG_IPV6),y)
SNMP_TRANSPORTS_INCLUDED+= UDPIPv6
endif

TARGET_LDFLAGS += -L$(TOOLCHAIN_DIR)/usr/lib

ifdef CONFIG_PACKAGE_snmpd-static
  define Build/Compile/static
	$(MAKE) -C $(PKG_BUILD_DIR)/agent \
		LDFLAGS="$(TARGET_LDFLAGS) -lm -static" \
		EXEEXT="-static" \
		snmpd-static
  endef
endif

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		LDFLAGS="$(TARGET_LDFLAGS) -lm -lc " \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) \
                -I $(BUILD_DIR)/nvram -L$(STAGING_DIR)/usr/lib/ -lnvram  "\
		all install	
	$(Build/Compile/static)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(2)/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/net-snmp-config $(2)/bin/
	$(SED) 's,=/usr,=$(STAGING_DIR)/usr,g' $(2)/bin/net-snmp-config
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/net-snmp-create-v3-user $(2)/bin/
	$(SED) 's,=/usr,=$(STAGING_DIR)/usr,g' $(2)/bin/net-snmp-create-v3-user
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/net-snmp $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnetsnmp{,agent,helpers,mibs}.{a,so*} $(1)/usr/lib/
endef

define Package/libnetsnmp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnetsnmp{,agent,helpers,mibs}.so.* $(1)/usr/lib/
endef

define Package/snmpd/conffiles
/etc/default/snmpd
/etc/config/snmpd
endef

define Package/snmpd/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/snmpd.conf $(1)/etc/config/snmpd
	$(INSTALL_DIR) $(1)/etc/snmp
	ln -sf /var/run/snmpd.conf $(1)/etc/snmp/
	$(INSTALL_DIR) $(1)/usr/share/snmp/
	ln -sf /tmp/snmpd.conf $(1)/usr/share/snmp/snmpd.conf
	$(INSTALL_DIR) $(1)/etc/default
	$(INSTALL_DATA) ./files/snmpd.default $(1)/etc/default/snmpd
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/snmpd.init $(1)/etc/init.d/snmpd
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/snmpd $(1)/usr/sbin/snmpd
endef

define Package/snmpd-static/conffiles
/etc/default/snmpd
/etc/config/snmpd
endef

define Package/snmpd-static/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/snmpd.conf $(1)/etc/config/snmpd
	$(INSTALL_DIR) $(1)/etc/snmp
	ln -sf /var/run/snmpd.conf $(1)/etc/snmp/
	$(INSTALL_DIR) $(1)/usr/share/snmp/
	ln -sf /tmp/snmpd.conf $(1)/usr/share/snmp/snmpd.conf
	$(INSTALL_DIR) $(1)/etc/default
	$(INSTALL_DATA) ./files/snmpd.default $(1)/etc/default/snmpd
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/snmpd.init $(1)/etc/init.d/snmpd
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/agent/snmpd-static $(1)/usr/sbin/snmpd
endef

define Package/snmp-utils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/snmp{get,set,status,test,trap,walk,usm} $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/net-snmp-config $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/net-snmp-create-v3-user $(1)/usr/bin/
endef

$(eval $(call BuildPackage,libnetsnmp))
$(eval $(call BuildPackage,snmp-utils))
$(eval $(call BuildPackage,snmpd))
$(eval $(call BuildPackage,snmpd-static))
