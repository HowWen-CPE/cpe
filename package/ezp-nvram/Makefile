# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ezp-nvram
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezp-nvram
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=NVRAM application for EZ Packet configuration
endef

define Package/ezp-nvram/description
 This package contains an utility for EZ Packet configuration subsystem.
endef



# EZP: this should depend on menuconfig
# TODO: Remove  WAN_NUM, LAN_NUM, WL_NUM
# TODO:

#
CONFIG_EZP_PRODUCT_VERSION="2.0.6-PLUS"
CONFIG_EZP_NVRAM_DOWNGRADE_VERSION="2.0.6-PLUS"
CONFIG_EZP_PRODUCT_CATEGORY="M"
CONFIG_EZP_PRODUCT_SUBCATEGORY="2"
CONFIG_EZP_PRODUCT_SUBSUBCATEGORY=""
CONFIG_EZP_SECRET="ABOCOM"
CONFIG_WAN_NUM=1
CONFIG_LAN_NUM=2
CONFIG_WL_NUM=1

CONFIG_TARGET_mt7620=y

ifeq ($(CONFIG_TARGET_rt305x),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_RT305X
endif

ifeq ($(CONFIG_TARGET_mt7620),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_RT3883
endif

ifeq ($(CONFIG_TARGET_rt3883),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_RT3883
endif

ifeq ($(CONFIG_TARGET_ox810),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_OX810
endif

ifeq ($(CONFIG_TARGET_as500),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_AS500
endif

ifeq ($(BR2_LINUX_2_4_FV13XX),y)
#BR2_PACKAGE_NVRAM:=y
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_FV13XX
endif

ifeq ($(BR2_LINUX_2_4_BRCM),y)
#BR2_PACKAGE_NVRAM:=y
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_BRCM
endif

# EZP: Pass WAN_NUM, LAN_NUM, WL_NUM while compiling
EZP_CFLAGS+= -DWAN_NUM=$(CONFIG_WAN_NUM) -DLAN_NUM=$(CONFIG_LAN_NUM) -DWL_NUM=$(CONFIG_WL_NUM) -DEZP_PROD_CAT=$(CONFIG_EZP_PRODUCT_CATEGORY) -DEZP_PROD_CAT_$(CONFIG_EZP_PRODUCT_CATEGORY) -DEZP_PROD_SUBCAT=$(CONFIG_EZP_PRODUCT_SUBCATEGORY) -DEZP_PROD_VERSION=$(CONFIG_EZP_PRODUCT_VERSION) -DEZP_PROD_SUBSUBCAT=$(CONFIG_EZP_PRODUCT_SUBSUBCATEGORY) -DEZP_PROD_SUBSUBCAT_$(CONFIG_EZP_PRODUCT_SUBSUBCATEGORY) -DEZP_PUBLIC_TREE

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
	mkdir -p $(STAGING_DIR)/usr/include
	$(CP) ./include/[a-z]* $(STAGING_DIR)/usr/include
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -I $(STAGING_DIR)/usr/include $(EZP_CFLAGS)"
endef

define Build/InstallDev
	mkdir -p $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib
endef

define Package/ezp-nvram/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nvram $(1)/usr/sbin
endef

$(eval $(call BuildPackage,ezp-nvram))
