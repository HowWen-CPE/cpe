# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=liteonimage

include $(INCLUDE_DIR)/host-build.mk

HOST_CFLAGS += -DPIH_MAGIC=$(CONFIG_MAGIC_NUM)

define Host/Compile
	$(HOSTCC) $(HOST_CFLAGS) -O -c src/crc32.c -o $(HOST_BUILD_DIR)/crc32.o
	$(HOSTCC) $(HOST_CFLAGS) -O -c src/liteonimage.c -o $(HOST_BUILD_DIR)/liteonimage.o
	$(HOSTCC) $(HOST_CFLAGS) -O -o $(HOST_BUILD_DIR)/liteonimage $(HOST_BUILD_DIR)/liteonimage.o $(HOST_BUILD_DIR)/crc32.o
endef

define Host/Install
	$(CP) $(HOST_BUILD_DIR)/liteonimage $(STAGING_DIR_HOST)/bin/
endef

define Host/Clean
	rm -f $(STAGING_DIR_HOST)/bin/liteonimage
endef

$(eval $(call HostBuild))
