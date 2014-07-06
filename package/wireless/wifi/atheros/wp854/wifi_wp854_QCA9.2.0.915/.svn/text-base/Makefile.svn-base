#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

export BUILD_UMAC=1

# Set Phy Err Diagnostics (Radar detection) to be enabled for AP builds
export ATH_CAP_PHYERR_DIAG=1


export AH_SUPPORT_AR5210=0
export AH_SUPPORT_AR5212=0
export AH_SUPPORT_5111=0
export AH_SUPPORT_5112=0
export AH_SUPPORT_2413=0
export AH_SUPPORT_5111=0
export AH_SUPPORT_5112=0
export AH_SUPPORT_2413=0
export AH_SUPPORT_5413=0
export AH_SUPPORT_2316=0
export AH_SUPPORT_2317=0
export AH_SUPPORT_2425=0
export AH_SUPPORT_AR5416=1
export AR5416_G_MODE=1
export AH_SUPPORT_HOWL=0
export AH_SUPPORT_OWL=1
export AR5416_INT_MITIGATION=0
export ATH_SUPPORT_IQUE=1
export IEEE_MCAST_ENHANCEMENT=1


include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk


PKG_NAME:=wifi_QCA9.2.0.915
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.bz2
PKG_SOURCE_URL:=http://ptnas/sources
PKG_MD5SUM:=6bb4f4de58eab0e419221b408d9dec94

PKG_BUILD_DEPENDS:=wprobe

include $(INCLUDE_DIR)/package.mk

ifdef CONFIG_MADWIFI_COMPRESSION
  COMPRESSION:=1
else
  COMPRESSION:=0
endif

ifneq ($(CONFIG_TARGET_atheros),)
  BUS:=AHB
else
  ifneq ($(CONFIG_PCI_SUPPORT),)
    BUS:=PCI
  endif
endif

ifneq ($(CONFIG_CPU_MIPS32_R2),)
  ifeq ($(ARCH),mips)
    HAL_TARGET:=mips32r2-be-elf
  endif
  ifeq ($(ARCH),mipsel)
    HAL_TARGET:=mips32r2-le-elf
  endif
else
  ifeq ($(ARCH),mips)
    HAL_TARGET:=mips32-be-elf
  endif
  ifeq ($(ARCH),mipsel)
    HAL_TARGET:=mips32-le-elf
  endif
endif
ifeq ($(ARCH),i386)
  HAL_TARGET:=i386-elf
endif
ifeq ($(ARCH),i686)
  HAL_TARGET:=i386-elf
endif
ifeq ($(ARCH),armeb)
  HAL_TARGET:=xscale-be-elfgnueabi
endif
ifeq ($(ARCH),arm)
  HAL_TARGET:=xscale-le-elfgnueabi
  ifeq ($(BOARD),cns21xx)
    HAL_TARGET:=armv4-le-elfgnueabi
  endif
  ifeq ($(BOARD),cns3xxx)
    HAL_TARGET:=arm11-le-elfgnueabi
  endif
  ifeq ($(BOARD),gemini)
    HAL_TARGET:=armv4-le-elfgnueabi
  endif
endif
ifeq ($(ARCH),powerpc)
  HAL_TARGET:=powerpc-be-elf
endif
ifneq ($(CONFIG_TARGET_atheros),)
  HAL_TARGET:=wisoc
endif

MADWIFI_FILES:= \
	$(PKG_BUILD_DIR)/adf/adf.ko \
	$(PKG_BUILD_DIR)/asf/asf.ko \
	$(PKG_BUILD_DIR)/umac/umac.ko \
	$(PKG_BUILD_DIR)/lmac/ath_dev/ath_dev.ko \
	$(PKG_BUILD_DIR)/lmac/dfs/ath_dfs.ko \
	$(PKG_BUILD_DIR)/lmac/ath_pktlog/ath_pktlog.ko \
	$(PKG_BUILD_DIR)/lmac/ratectrl/ath_rate_atheros.ko \
	$(PKG_BUILD_DIR)/os/linux/ath_hal/ath_hal.ko 

MADWIFI_AUTOLOAD:= \
	adf \
	asf \
	ath_hal \
	ath_rate_atheros \
	ath_dfs  \
	ath_dev \
	umac \
	ath_pktlog 

#MADWIFI_APPLETS:=80211stats athchans athkey athstats wlanconfig ath_info madwifi_multi
MADWIFI_APPLETS:=wlanconfig
ifdef CONFIG_MADWIFI_DEBUG
  MADWIFI_APPLETS += athdebug 80211debug
endif

define KernelPackage/wifi_wp854_QCA9.2.0.915
  SUBMENU:=Wireless Drivers
  TITLE:=Driver for wp854 wifi
  URL:=http://madwifi-project.org/
  MAINTAINER:=Liteon NABU BJRD PT <Daniel@liteon.com>
  DEPENDS:=@TARGET_freescale_wp854_wp854_FSLp1020wlan101112 +wireless-tools @PCI_SUPPORT  @(!(TARGET_avr32||TARGET_cobalt||TARGET_ep93xx||TARGET_etrax||TARGET_octeon||TARGET_pxcab||TARGET_sibyte||LINUX_3_1||LINUX_3_2)||BROKEN) +@DRIVER_WEXT_SUPPORT
  FILES:=$(MADWIFI_FILES)
  AUTOLOAD:=$(call AutoLoad,25,$(MADWIFI_AUTOLOAD))
  MENU:=1
endef

define KernelPackage/wifi_wp854_QCA9.2.0.915/description
 This package contains a driver for wp854.
endef

define KernelPackage/wifi_wp854_QCA9.2.0.915/config
	source "$(SOURCE)/Config.in"
endef

HAL_COPTS_VARS:= \
	$(TARGET_CFLAGS) \
	-I$(LINUX_DIR)/arch/$(LINUX_KARCH)/include \
	-DATH_SUPPORT_VOWEXT=1 \
	-DAH_REGOPS_FUNC \
	-ffixed-r2

ifeq ($(CONFIG_BIG_ENDIAN),y)
HAL_COPTS_VARS+= \
	-DAH_BYTE_ORDER=AH_BIG_ENDIAN 
else
HAL_COPTS_VARS+= \
	-DAH_BYTE_ORDER=AH_LITTLE_ENDIAN 
endif

MADWIFI_INC = \
	-I$(PKG_BUILD_DIR)/ \
	-I$(PKG_BUILD_DIR)/include \
	-I$(PKG_BUILD_DIR)/hal \
	-I$(PKG_BUILD_DIR)/hal/linux \
	-I$(PKG_BUILD_DIR)/os/linux/include \
	-I$(PKG_BUILD_DIR)/os/linux/src \
	-I$(PKG_BUILD_DIR)/os/linux/ath_hal \
	-I$(PKG_BUILD_DIR)/lmac/ath_pktlog \
	-I$(PKG_BUILD_DIR)/lmac/ratectrl \
	-I$(PKG_BUILD_DIR)/lmac/ath_dev \
	-I$(PKG_BUILD_DIR)/lmac/tx99 \
	-I$(PKG_BUILD_DIR)/lmac/dfs \
	-I$(PKG_BUILD_DIR)/umac/include \
	-I$(PKG_BUILD_DIR)/umac/if_lmac \
	-I$(PKG_BUILD_DIR)/umac/crypto \
	-I$(PKG_BUILD_DIR)/umac/pm \
	-I$(PKG_BUILD_DIR)/umac/resmgr \
	-I$(PKG_BUILD_DIR)/umac/scan \
	-I$(PKG_BUILD_DIR)/umac/p2p \
	-I$(PKG_BUILD_DIR)/umac/vi_dbg \
	-I$(PKG_BUILD_DIR)/umac/rpt_placement \
	-I$(PKG_BUILD_DIR)/umac/ique \
	-I$(PKG_BUILD_DIR)/umac/tdls \
	-I$(PKG_BUILD_DIR)/umac/mlme \
	-I$(PKG_BUILD_DIR)/umac/sme \
	-I$(PKG_BUILD_DIR)/umac/txrx \
	-I$(PKG_BUILD_DIR)/umac/ique \
	-I$(PKG_BUILD_DIR)/umac/smart_antenna \
	-I$(PKG_BUILD_DIR)/umac/base \
	-I$(PKG_BUILD_DIR)/asf/include \
	-I$(PKG_BUILD_DIR)/adf/include/os \
	-I$(PKG_BUILD_DIR)/adf/include/net \
	-I$(PKG_BUILD_DIR)/adf/include/nbuf \
	-I$(PKG_BUILD_DIR)/adf/os/linux \
	-I$(PKG_BUILD_DIR)/adf/net/linux \
	-I$(PKG_BUILD_DIR)/adf/nbuf/linux \
	-I$(PKG_BUILD_DIR)/htc/adf/linux/os \
	-I$(STAGING_DIR)/usr/include/wprobe \
	-include $(PKG_BUILD_DIR)/include/compat.h

MAKE_ARGS:= \
	PATH="$(TARGET_PATH)" \
	ARCH="$(LINUX_KARCH)" \
	ARCH-y="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	TARGET="$(HAL_TARGET)" \
	TOOLPREFIX="$(KERNEL_CROSS)" \
	TOOLPATH="$(KERNEL_CROSS)" \
	HAL=$(PKG_BUILD_DIR)/hal \
	ATH_RATE=$(PKG_BUILD_DIR)/ratectrl11n \
	ATH_PHYERR=$(PKG_BUILD_DIR)/lmac/dfs \
	KERNELPATH="$(LINUX_DIR)" \
	$(if $(CONFIG_MADWIFI_DEBUG),,DEBUG=) WARNINGS="-Wno-unused"

#
# UMAC build option
#

MAKE_ARGS+=BUILD_UMAC=1

#
# Select the support required for this board
#

MAKE_ARGS+=AH_SUPPORT_AR5210=0
MAKE_ARGS+=AH_SUPPORT_AR5212=0
MAKE_ARGS+=AH_SUPPORT_5111=0
MAKE_ARGS+=AH_SUPPORT_5112=0
MAKE_ARGS+=AH_SUPPORT_2413=0
MAKE_ARGS+=AH_SUPPORT_5413=0
MAKE_ARGS+=AH_SUPPORT_2316=0
MAKE_ARGS+=AH_SUPPORT_2317=0
MAKE_ARGS+=AH_SUPPORT_2425=0
MAKE_ARGS+=AH_SUPPORT_AR5416=1
#MAKE_ARGS+=ATH_SUPPORT_TX99=1

#
# Environmental variables used to configure the HAL/ATH layers at compile time
#

MAKE_ARGS+=ATH_CAP_PHYERR_DIAG=1

MAKE_ARGS+=AR5416_G_MODE=1
MAKE_ARGS+=AH_SUPPORT_HOWL=0
MAKE_ARGS+=AH_SUPPORT_OWL=1
MAKE_ARGS+=AH_DEBUG=0
MAKE_ARGS+=ATH_DEBUG=1
MAKE_ARGS+=HAL_DEBUG=0
MAKE_ARGS+=AR5416_INT_MITIGATION=0
MAKE_ARGS+=ATH_SUPPORT_DFS=1
MAKE_ARGS+=ATH_SUPPORT_IQUE=1
MAKE_ARGS+=IEEE_MCAST_ENHANCEMENT=1
MAKE_ARGS+=ATH_SUPPORT_VOWEXT=1
MAKE_ARGS+=ATH_VOW_EXT_STATS=1

MAKE_ARGS+=ATH_WPS_IE=0
MAKE_ARGS+=ATH_SUPPORT_VLAN=1
MAKE_ARGS+=ATH_EXT_AP=1
MAKE_ARGS+=ATH_SUPPORT_LED=1
MAKE_ARGS+=GPIO_PIN_FUNC_0=10
MAKE_ARGS+=GPIO_PIN_FUNC_1=0
MAKE_ARGS+=GPIO_PIN_FUNC_2=7
MAKE_ARGS+=AP_USB_LED_GPIO=0

MAKE_ARGS+=WIRELESSTOOLS=wireless_tools.28
MAKE_ARGS+=WIRELESSTOOLSLIB=libiw.so.28

MAKE_ARGS+=NO_SIMPLE_CONFIG=1

define Build/Prepare
	$(call Build/Prepare/Default)
endef

define Build/Configure
	# TODO: 
endef

define Build/Compile/hal
	@echo HAL Build
	$(MAKE) -C $(PKG_BUILD_DIR)/hal/linux $(MAKE_ARGS) COPTS="$(HAL_COPTS_VARS)"
	$(MAKE) -C $(PKG_BUILD_DIR)/hal/linux $(MAKE_ARGS) COPTS="$(HAL_COPTS_VARS)" release
endef

ifeq ($(findstring AHB,$(BUS)),AHB)
  define Build/Compile/ahb
	@echo AHB Build
	$(MAKE) -C $(PKG_BUILD_DIR)/os/linux $(MAKE_ARGS) INCS="$(MADWIFI_INC)" BUS="AHB" 
  endef
endif

ifeq ($(findstring PCI,$(BUS)),PCI)
  define Build/Compile/pci
	@echo PCI Build
	$(MAKE) -C $(PKG_BUILD_DIR)/os/linux $(MAKE_ARGS) INCS="$(MADWIFI_INC)" BUS="PCI" 
  endef
endif

define Build/Compile/tool
	$(MAKE) -C $(PKG_BUILD_DIR)/os/linux/tools $(MAKE_ARGS) INCS="$(MADWIFI_INC)" $(MADWIFI_APPLETS)
endef

define Build/Compile
	$(call Build/Compile/hal)
	$(call Build/Compile/ahb)
	$(call Build/Compile/pci)
	$(call Build/Compile/tool)
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include/wifi_wp854_QCA9.2.0.915
	$(CP) $(PKG_BUILD_DIR)/include $(1)/usr/include/madwifi/
endef

define KernelPackage/wifi_wp854_QCA9.2.0.915/install
	$(INSTALL_DIR) $(1)/usr/sbin
	#$(CP) ./files/* $(1)/
	$(CP) $(foreach applet,$(MADWIFI_APPLETS),$(PKG_BUILD_DIR)/os/linux/tools/$(applet)) $(1)/usr/sbin/
endef

$(eval $(call KernelPackage,wifi_wp854_QCA9.2.0.915))
