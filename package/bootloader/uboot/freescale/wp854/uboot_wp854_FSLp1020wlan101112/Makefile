#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot_FSLP1020wlan_20101112
PKG_RELEASE:=v0.1.1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_MD5SUM:=2c20b972596bb0267dbacceab0799bfb
#PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define uboot/Default
  TITLE:=
  CONFIG:=
  IMAGE:=
endef

define uboot/wp854_FSLp1020wlan101112
  TITLE:=U-boot for the p1020wlan or p1011wlan
endef

UBOOTS:=wp854_FSLp1020wlan101112

define Package/uboot/template
define Package/uboot_$(1)
  SECTION:=boot
  CATEGORY:=Boot Loaders
  TITLE:=$(2)
  SUBMENU:=uboot
  DEPENDS:=@TARGET_freescale_wp854_wp854_FSLp1020wlan101112
  URL:=http://www.denx.de/wiki/U-Boot
  DEFAULT:=y if (CONFIG_TARGET_freescale_Default)
  VARIANT:=P1020SKU
endef
endef

define BuildUbootPackage	
	$(eval $(uboot/Default))
	$(eval $(uboot/$(1)))
	$(call Package/uboot/template,$(1),$(TITLE))
endef


ifdef BUILD_VARIANT
$(eval $(call uboot/$(BUILD_VARIANT)))
UBOOT_CONFIG:=$(if $(CONFIG),$(CONFIG),$(BUILD_VARIANT))
UBOOT_IMAGE:=$(if $(IMAGE),$(IMAGE),openwrt-$(BOARD)-$(BUILD_VARIANT)-u-boot.bin)
endif

UBOOT_CONFIG:=P1020SKU


define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./files/* $(PKG_BUILD_DIR)
	find $(PKG_BUILD_DIR) -name .svn | $(XARGS) rm -rf
endef

define Build/Configure
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(UBOOT_CONFIG)_config
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CROSS_COMPILE=$(TARGET_CROSS)
endef

define Package/uboot/install/template
define Package/uboot_$(1)/install
	$(CP) $(PKG_BUILD_DIR)/u-boot.bin $(BIN_DIR)/$(2)
endef
endef

$(foreach u,$(UBOOTS), \
	$(eval $(call Package/uboot/install/template,$(u),$(BOARDNAME)-$(PKG_RELEASE)-uboot.bin)) \
)

$(foreach u,$(UBOOTS), \
	$(eval $(call BuildUbootPackage,$(u))) \
	$(eval $(call BuildPackage,uboot_$(u))) \
)
