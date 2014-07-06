#
# Copyright (C) 2008-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mipsel
BOARD:=ralink
BOARDNAME:=Ralink Solution
SUBTARGETS:=wp688/wp688_MTK4110_130426
CFLAGS:=-Os -pipe -mips32r2 -mtune=mips32r2 -fno-caller-saves

LINUX_VERSION:=

include $(INCLUDE_DIR)/target.mk
DEFAULT_PACKAGES+=

define Target/Description
	Build firmware images for Ralink MT7620 based boards.
endef

$(eval $(call BuildTarget))
