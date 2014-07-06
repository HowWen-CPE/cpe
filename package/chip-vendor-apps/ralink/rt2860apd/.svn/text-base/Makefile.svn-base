# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=rt2860apd
PKG_VERSION:=2.3.0.0
PKG_RELEASE:=1

PKG_SOURCE:=rt2860ap-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/rt2860ap-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/rt2860apd
	SECTION:=uitls
	CATEGORY:=Network
	DEPENDS:=@TARGET_ralink
	TITLE:=Ralink wireless management daemon
endef

define Package/rt2860apd/description
	  A wireless management daemon for Ralink RT2860 11n wireless card.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/DOT1X \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR)"
endef

define Package/rt2860apd/install
	$(INSTALL_DIR) $(1)/etc/wl
	#$(INSTALL_BIN) ./files/rt2860ap-init $(1)/etc/wl
	#$(INSTALL_BIN) ./files/RT3883_AP_3T3R_V0_1.bin $(1)/etc/wl
	#$(INSTALL_BIN) ./files/MT7620_AP_2T2R-4L_V11.BIN $(1)/etc/wl
	#$(INSTALL_BIN) ./files/MT7620_AP_2T2R-4L_external_LNA_external_PA_V15.BIN $(1)/etc/wl
	#$(INSTALL_BIN) ./files/MT7620_AP_2T2R-4L_external_LNA_internal_PA_V15.BIN $(1)/etc/wl
	#$(INSTALL_BIN) ./files/MT7620_AP_2T2R-4L_internal_LNA_internal_PA_V15.BIN $(1)/etc/wl	
	#$(INSTALL_BIN) ./files/RT2860AP.dat $(1)/etc/wl
	# $(INSTALL_DIR) $(1)/etc/Wireless
	# ln -sf /etc/wl $(1)/etc/Wireless/RT2860
	#ln -sf /var/RT2860AP.dat $(1)/etc/wl/RT2860AP.dat
	#-find $(1) -type d -name CVS | xargs rm -rf
	#-find $(1) -type d -name .svn | xargs rm -rf
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/DOT1X/rt2860apd $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/sbin
	#$(INSTALL_BIN) ./files/ezp-wl0-mode $(1)/sbin
	#$(INSTALL_BIN) ./files/ezp-wl0-ctrl $(1)/sbin
	#$(INSTALL_BIN) ./files/ezp-wl0-ant $(1)/sbin
	#$(INSTALL_BIN) ./files/ezp-auto-bridge $(1)/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	#$(INSTALL_BIN) ./files/autobridge $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/rt2860apd.init $(1)/etc/init.d/rt2860apd
	#$(INSTALL_BIN) ./files/wps0-write-conf $(1)/sbin/wps0-write-conf
endef

$(eval $(call BuildPackage,rt2860apd))
