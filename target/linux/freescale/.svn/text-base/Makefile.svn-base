#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=powerpc
BOARD:=freescale
BOARDNAME:=Freescale Solution
LINUX_VERSION:=
SUBTARGETS:=wp854/wp854_FSLp1020wlan101112
CFLAGS:=-O2 -pipe -fno-caller-saves -mcpu=8540

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += 

define Target/Description
	Build firmware images based on Freescale solution
endef

$(eval $(call BuildTarget))
