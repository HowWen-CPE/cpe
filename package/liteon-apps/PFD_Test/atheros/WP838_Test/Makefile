# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=PFD_Test_wp838
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/PFD_Test_wp838
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=PFD_Test_wp838
  	SUBMENU:=Liteon Application
	DEPENDS:=@TARGET_atheros_wp838_wp838_QCA10_1_436
endef

define Package/PFD_Test_wp838/description
	PFD_Test for wp838
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)/
endef

define Build/Compile	
	#nothing to do
endef

define Package/PFD_Test_wp838/install
	#$(INSTALL_DIR) $(1)/etc
	$(CP) ./files/* $(1)/
endef


$(eval $(call BuildPackage,PFD_Test_wp838))
