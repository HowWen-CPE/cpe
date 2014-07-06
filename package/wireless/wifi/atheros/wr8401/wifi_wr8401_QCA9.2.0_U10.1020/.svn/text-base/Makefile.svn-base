#
# Copyright (C) Sean@liteon.com
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

# Add by Sean@liteon.com
export AH_SUPPORT_KITE_10=1
export AH_SUPPORT_KITE_ANY=1
export AH_DESC_NOPACK=1
export AH_SUPPORT_SOWL=0
export AH_SUPPORT_K2=0
export AH_CAL_IN_FLASH_PCI=1
export AH_CAL_RADIOS_PCI=1
export AH_CAL_IN_FLASH_AHB=1
export AH_CAL_RADIOS_AHB=1

export ATH_SUPPORT_TxBF=1
export ATH_SUPPORT_PAPRD=1
export ATH_TRAFFIC_FAST_RECOVER=1
export ATH_CAL_NAND_FLASH=1
export ATH_CAL_NAND_PARTITION="caldata"

export UMAC_SUPPORT_APONLY=1
export CONFIG_MIPS_74K_KERNEL_OPTIMIZATION=1
export BUS=dual


include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=wifi_QCA9.2.0_U10.1020
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.bz2
PKG_MD5SUM:=fad73801f7a754854c8544155fd8a646

PKG_BUILD_DEPENDS:=wprobe

include $(INCLUDE_DIR)/package.mk

ifdef CONFIG_MADWIFI_COMPRESSION
  COMPRESSION:=1
else
  COMPRESSION:=0
endif

ifneq ($(CONFIG_TARGET_atheros),)
  # Modify by Sean@liteon.com
  #BUS:=AHB
  BUS:=dual
else
  ifneq ($(CONFIG_PCI_SUPPORT),)
    BUS:=PCI
  endif
endif

ifneq ($(CONFIG_CPU_MIPS32_R2),)
  ifeq ($(ARCH),mips)
    #Sean@liteon.com
    #HAL_TARGET:=mips32-be-elf
    HAL_TARGET:=mipsisa32-be-elf
  endif
  ifeq ($(ARCH),mipsel)
    HAL_TARGET:=mips32r2-le-elf
  endif
else
  ifeq ($(ARCH),mips)
    #Sean@liteon.com
    #HAL_TARGET:=mips32-be-elf
    HAL_TARGET:=mipsisa32-be-elf
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

# Delete by Sean@liteon.com.
#ifneq ($(CONFIG_TARGET_atheros),)
#  HAL_TARGET:=wisoc
#endif

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
# Sean@liteon.com
MADWIFI_APPLETS:=80211stats athstats athstatsclr pktlogconf pktlogdump wlanconfig
ifdef CONFIG_MADWIFI_DEBUG
  MADWIFI_APPLETS += athdebug 80211debug
endif

define KernelPackage/wifi_wr8401_QCA9.2.0_U10.1020
  SUBMENU:=Wireless Drivers
  TITLE:=Driver for wr8401 wifi
  URL:=http://madwifi-project.org/
  MAINTAINER:=Liteon NABU BJRD PT <Sean@liteon.com>
  DEPENDS:=@TARGET_atheros_wr8401_wr8401_QCA9_2_0_U10_1020 +wireless-tools @PCI_SUPPORT @(!(TARGET_avr32||TARGET_cobalt||TARGET_ep93xx||TARGET_etrax||TARGET_octeon||TARGET_pxcab||TARGET_sibyte||LINUX_3_1||LINUX_3_2)||BROKEN) +@DRIVER_WEXT_SUPPORT 
  FILES:=$(MADWIFI_FILES)
  AUTOLOAD:=$(call AutoLoad,25,$(MADWIFI_AUTOLOAD))
  MENU:=1
endef

define KernelPackage/wifi_wr8401_QCA9.2.0_U10.1020/description
 This package contains a driver for wr8401 based on atheros wifi chipsets.
endef

define KernelPackage/wifi_wr8401_QCA9.2.0_U10.1020/config
	source "$(SOURCE)/Config.in"
endef

# Sean@liteon.com
#HAL_COPTS_VARS:= \
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

#Sean@liteon.com
#MAKE_ARGS:= \
	PATH="$(TARGET_PATH)" \
	ARCH="$(LINUX_KARCH)" \
	ARCH-y="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	TARGET="$(HAL_TARGET)" \
	TOOLPREFIX="$(KERNEL_CROSS)" \
	TOOLPATH="$(KERNEL_CROSS)" \
	HAL=$(PKG_BUILD_DIR)/hal \
	ATH_PHYERR=$(PKG_BUILD_DIR)/lmac/dfs \
	KERNELPATH="$(LINUX_DIR)" \
	$(if $(CONFIG_MADWIFI_DEBUG),,DEBUG=) WARNINGS="-Wno-unused"
MAKE_ARGS:= \
	PATH="$(TARGET_PATH)" \
	ARCH="$(LINUX_KARCH)" \
	ARCH-y="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	TARGET="$(HAL_TARGET)" \
	TOOLPREFIX="$(KERNEL_CROSS)" \
	TOOLPATH="$(KERNEL_CROSS)" \
	HAL=$(PKG_BUILD_DIR)/hal \
	ATH_PHYERR=$(PKG_BUILD_DIR)/lmac/dfs \
	KERNELPATH="$(LINUX_DIR)" \
	WLAN_TOP=$(PKG_BUILD_DIR) \
	KERNELARCH=$(LINUX_KARCH) \
	KERNELVER=$(LINUX_VERSION) \
	$(if $(CONFIG_MADWIFI_DEBUG),,DEBUG=) WARNINGS="-Wno-unused"

MAKE_TOOLS_ARGS:= \
	PATH="$(TARGET_PATH)" \
	ARCH="$(LINUX_KARCH)" \
	ARCH-y="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	TOOLPREFIX="$(KERNEL_CROSS)" \
	TOOLPATH="$(KERNEL_CROSS)" \
	HAL=$(PKG_BUILD_DIR)/hal \
	ATH_PHYERR=$(PKG_BUILD_DIR)/lmac/dfs \
	KERNELPATH="$(LINUX_DIR)" \
	WLAN_TOP=$(PKG_BUILD_DIR) \
	KERNELARCH=$(LINUX_KARCH) \
	KERNELVER=$(LINUX_VERSION) \
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
#Sean@liteon.com
#MAKE_ARGS+=AH_SUPPORT_HOWL=0
#MAKE_ARGS+=AH_SUPPORT_OWL=1
MAKE_ARGS+=AH_DEBUG=0
#MAKE_ARGS+=ATH_DEBUG=1
#MAKE_ARGS+=HAL_DEBUG=0
#MAKE_ARGS+=AR5416_INT_MITIGATION=0
#MAKE_ARGS+=ATH_SUPPORT_DFS=1
MAKE_ARGS+=AR5416_INT_MITIGATION=1
MAKE_ARGS+=ATH_SUPPORT_DFS=0
#MAKE_ARGS+=ATH_SUPPORT_IQUE=1
#MAKE_ARGS+=IEEE_MCAST_ENHANCEMENT=1
#MAKE_ARGS+=ATH_SUPPORT_VOWEXT=1
#MAKE_ARGS+=ATH_VOW_EXT_STATS=1

#MAKE_ARGS+=ATH_WPS_IE=0
MAKE_ARGS+=ATH_WPS_IE=1
MAKE_ARGS+=ATH_SUPPORT_VLAN=1
MAKE_ARGS+=ATH_EXT_AP=1
MAKE_ARGS+=ATH_SUPPORT_LED=1
#MAKE_ARGS+=GPIO_PIN_FUNC_0=10
#MAKE_ARGS+=GPIO_PIN_FUNC_1=0
#MAKE_ARGS+=GPIO_PIN_FUNC_2=7
MAKE_ARGS+=GPIO_PIN_FUNC_0=0
MAKE_ARGS+=GPIO_PIN_FUNC_1=0
MAKE_ARGS+=GPIO_PIN_FUNC_2=0
#MAKE_ARGS+=AP_USB_LED_GPIO=0
MAKE_ARGS+=AP_USB_LED_GPIO=11

# Sean@liteon.com
#MAKE_ARGS+=WIRELESSTOOLS=wireless_tools.28
#MAKE_ARGS+=WIRELESSTOOLSLIB=libiw.so.28
MAKE_ARGS+=WIRELESSTOOLS=wireless_tools.29
MAKE_ARGS+=WIRELESSTOOLSLIB=libiw.so.29

MAKE_ARGS+=NO_SIMPLE_CONFIG=1

# Sean@Liteon.com
MAKE_ARGS+=MADWIFITARGET=mipsisa32-be-elf
MAKE_ARGS+=FUSIONTARGET=mipsisa32-be-elf
MAKE_ARGS+=TARGETARCH=mipsisa32-be-elf

define Build/Prepare
	$(call Build/Prepare/Default)
endef

define Build/Configure
	# TODO: 
endef

# Sean@liteon.com
#define Build/Compile/hal
	#@echo HAL Build
	$(MAKE) -C $(PKG_BUILD_DIR)/hal/linux $(MAKE_ARGS) COPTS="$(HAL_COPTS_VARS)"
	$(MAKE) -C $(PKG_BUILD_DIR)/hal/linux $(MAKE_ARGS) COPTS="$(HAL_COPTS_VARS)" release
#endef
define Build/Compile/hal
	@echo HAL Build
	$(MAKE) -C $(PKG_BUILD_DIR)/hal/linux $(MAKE_ARGS) 
	$(MAKE) -C $(PKG_BUILD_DIR)/hal/linux $(MAKE_ARGS) release
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

# Add by Sean@liteon.com
ifeq ($(findstring dual,$(BUS)),dual)
  define Build/Compile/dual
	@echo dual Build
	$(MAKE) -C $(PKG_BUILD_DIR)/os/linux $(MAKE_ARGS) INCS="$(MADWIFI_INC)"  
  endef
endif

# Modify by Sean@liteon.com
#define Build/Compile/tool
#	$(MAKE) -C $(PKG_BUILD_DIR)/os/linux/tools $(MAKE_ARGS) INCS="$(MADWIFI_INC)" $(MADWIFI_APPLETS)
#endef
define Build/Compile/tool
	$(MAKE) -C $(PKG_BUILD_DIR)/os/linux/tools $(MAKE_TOOLS_ARGS) INCS="$(MADWIFI_INC)" $(MADWIFI_APPLETS)
endef

define Build/Compile
	$(call Build/Compile/hal)
	$(call Build/Compile/ahb)
	$(call Build/Compile/pci)
	$(call Build/Compile/dual)
	$(call Build/Compile/tool)
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include/madwifi
	$(CP) $(PKG_BUILD_DIR)/include $(1)/usr/include/madwifi/
endef

define KernelPackage/wifi_wr8401_QCA9.2.0_U10.1020/install
	$(INSTALL_DIR) $(1)/usr/sbin
	#$(CP) ./files/* $(1)/
	$(CP) $(foreach applet,$(MADWIFI_APPLETS),$(PKG_BUILD_DIR)/os/linux/tools/$(applet)) $(1)/usr/sbin/
endef

$(eval $(call KernelPackage,wifi_wr8401_QCA9.2.0_U10.1020))
