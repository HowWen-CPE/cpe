#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
LINUX_VERSION:=2.6.31
FEATURES:=
BOARDNAME:=wp838_QCA10.1.436

export AP_SPI_CS2_GPIO=16
export CONFIG_MIPS_74K_KERNEL_OPTIMIZATION=1

define Target/Description
        Build firmware images for wp838 project.
endef

