#
# Copyright (C) Sean@liteon.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=wifi_QCA_10.1.436
PKG_SOURCE_SUBDIR:=drivers/wlan_modules
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_SOURCE_DIR:=$(PKG_BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_MD5SUM:=9c924401d2e439410ee7dc58f1dafea7

PKG_BUILD_DEPENDS:=wprobe

SUB_INCLUDE_DIR:=./include

include $(INCLUDE_DIR)/package.mk

include $(SUB_INCLUDE_DIR)/MADWIFI-EXPORT
include $(SUB_INCLUDE_DIR)/MADWIFI-MAKE_ARGS
include $(SUB_INCLUDE_DIR)/MADWIFI_INC


MADWIFI_FILES:= \
	$(PKG_SOURCE_DIR)/adf/adf.ko \
	$(PKG_SOURCE_DIR)/asf/asf.ko \
	$(PKG_SOURCE_DIR)/umac/umac.ko \
	$(PKG_SOURCE_DIR)/lmac/ath_dev/ath_dev.ko \
	$(PKG_SOURCE_DIR)/lmac/spectral/ath_spectral.ko \
	$(PKG_SOURCE_DIR)/lmac/dfs/ath_dfs.ko \
	$(PKG_SOURCE_DIR)/lmac/ath_pktlog/ath_pktlog.ko \
	$(PKG_SOURCE_DIR)/lmac/ratectrl/ath_rate_atheros.ko \
	$(PKG_SOURCE_DIR)/os/linux/ath_hal/ath_hal.ko 

MADWIFI_AUTOLOAD:= \
	adf \
	asf \
	ath_hal \
	ath_rate_atheros \
	ath_spectral \
	ath_dfs  \
	ath_dev \
	umac \
	ath_pktlog 

#MADWIFI_APPLETS:=80211stats athchans athkey athstats wlanconfig ath_info madwifi_multi
# Sean@liteon.com
MADWIFI_APPLETS:=cgiMain 80211stats athstats athstatsclr pktlogconf pktlogdump wlanconfig
ifdef CONFIG_MADWIFI_DEBUG
  MADWIFI_APPLETS += athdebug 80211debug
endif

ifneq ($(strip ${ATH_SUPPORT_DFS}),0)
  MADWIFI_APPLETS += radartool
endif

ifneq ($(strip ${ATH_SUPPORT_SPECTRAL}),0)
  MADWIFI_APPLETS += spectraltool
endif

define KernelPackage/wifi_wp838_QCA_10.1.436
  SUBMENU:=Wireless Drivers
  TITLE:=Driver for WP838's WiFi
  URL:=http://madwifi-project.org/
  MAINTAINER:=Liteon NABU BJRD PT <Sean@liteon.com>
  DEPENDS:=@TARGET_atheros_wp838_wp838_QCA10_1_436 +wireless-tools @PCI_SUPPORT @(!(TARGET_avr32||TARGET_cobalt||TARGET_ep93xx||TARGET_etrax||TARGET_octeon||TARGET_pxcab||TARGET_sibyte||LINUX_3_1||LINUX_3_2)||BROKEN) +@DRIVER_WEXT_SUPPORT 
  FILES:=$(MADWIFI_FILES)
  #AUTOLOAD:=$(call AutoLoad,25,$(MADWIFI_AUTOLOAD))
  MENU:=1
endef

define KernelPackage/wifi_wp838_QCA_10.1.436/description
 This package contains a driver for WP838 based on atheros wifi chipsets.
endef

define Build/Compile/hal
	@echo HAL Build
	$(MAKE) -C $(PKG_SOURCE_DIR)/hal/linux $(MAKE_ARGS) TARGET="$(HAL_TARGET)"
	$(MAKE) -C $(PKG_SOURCE_DIR)/hal/linux $(MAKE_ARGS) TARGET="$(HAL_TARGET)" release
endef

define Build/Compile/os
	@echo $(1) Build
	$(MAKE) -C $(PKG_SOURCE_DIR)/os/linux $(MAKE_ARGS) TARGET="$(HAL_TARGET)" BUS="$(1)"
endef

define Build/Compile/tool
	@echo Tool Build
	$(MAKE) -C $(PKG_SOURCE_DIR)/os/linux/tools $(MAKE_ARGS) $(MADWIFI_APPLETS)
endef

define Build/Compile/perf_pwr_offload
	@echo perf_pwr_offload Build
	$(MAKE) -C $(PKG_BUILD_DIR)/perf_pwr_offload/drivers/host/os/linux/tools/athdiag all
endef

define Build/Compile
	$(call Build/Compile/hal)
	$(call Build/Compile/perf_pwr_offload)

    ifeq ($(findstring AHB,$(BUS)),AHB)
	$(call Build/Compile/os,AHB)
    endif

    ifeq ($(findstring PCI,$(BUS)),PCI)
	$(call Build/Compile/os,PCI)
    endif

    ifeq ($(findstring dual,$(BUS)),dual)
	$(call Build/Compile/os,dual)
    endif

	$(call Build/Compile/tool)
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include/madwifi
	$(CP) $(PKG_SOURCE_DIR)/include $(1)/usr/include/madwifi/
endef

define KernelPackage/wifi_wp838_QCA_10.1.436/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/bin
	ln -s /usr/sbin/cgiMain $(1)/bin/cfg
	#$(CP) ./files/* $(1)/usr/sbin
	$(CP) $(foreach applet,$(MADWIFI_APPLETS),$(PKG_SOURCE_DIR)/os/linux/tools/$(applet)) $(1)/usr/sbin/
endef

$(eval $(call KernelPackage,wifi_wp838_QCA_10.1.436))
