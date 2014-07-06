#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mips
BOARD:=atheros
BOARDNAME:=Atheros Solution
LINUX_VERSION:=
SUBTARGETS:=wr8401/wr8401_QCA9.2.0_U10.1020 wp838/wp838_QCA10.1.436

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += 

define Target/Description
	Build firmware images based on Atheros solution
endef

$(eval $(call BuildTarget))
