# $Id$

include $(TOPDIR)/rules.mk

#Add for rt_mmap.h, Sean@liteon, 2012-12-28----->
include $(INCLUDE_DIR)/kernel.mk
#<-----Sean@liteon, 2012-12-28

PKG_NAME:=reset_button
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/reset_button
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=reset_button
#  DEPENDS:=@TARGET_ralink
  SUBMENU:=Liteon Application
endef

define Package/reset_button/description
	reset_button	
endef

define Package/reset_button/config
source "$(SOURCE)/Config.in"
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="-DFACTORY_DEFAULT_GPIO=$(CONFIG_FACTORY_DEFAULT_GPIO) -DLITEON_GPIO_ENA_INT=$(CONFIG_LITEON_GPIO_ENA_INT) -DLITEON_GPIO_REG_IRQ=$(CONFIG_LITEON_GPIO_REG_IRQ) -DGPIO_DEV=$(CONFIG_GPIO_DEV) $(TARGET_CFLAGS) -I$(LINUX_DIR)/include" 
endef

define Package/reset_button/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/resetbutton $(1)/sbin
	chmod 755 $(1)/sbin/resetbutton
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/reset_button.init $(1)/etc/init.d/resetbutton

endef

$(eval $(call BuildPackage,reset_button))
