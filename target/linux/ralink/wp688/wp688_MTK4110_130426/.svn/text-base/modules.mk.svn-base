define KernelPackage/raeth
  SUBMENU:=$(NETWORK_DEVICES_MENU)
  TITLE:=MT7620 Ethernet support
  FILES:=$(LINUX_DIR)/drivers/net/raeth/raeth.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,60,raeth)
endef
define KernelPackage/raeth/description
 Kernel modules for MT7620 ethernet support
endef
$(eval $(call KernelPackage,raeth))

# hw_nat comes after raeth
define KernelPackage/hw_nat
  SUBMENU:=$(NETWORK_DEVICES_MENU)
  TITLE:=MT7620 hardware NAT support
  FILES:=$(LINUX_DIR)/drivers/net/nat/hw_nat/hw_nat.$(LINUX_KMOD_SUFFIX)
#  AUTOLOAD:=$(call AutoLoad,65,hw_nat)
endef
define KernelPackage/hw_nat/description
 Kernel modules for MT7620 hardware NAT support
endef
$(eval $(call KernelPackage,hw_nat))

define KernelPackage/netusb
  SUBMENU:=$(USB_MENU)
  TITLE:=Net USB support
  FILES:=$(LINUX_DIR)/drivers/net/GPL_NetUSB.$(LINUX_KMOD_SUFFIX) \
    $(LINUX_DIR)/drivers/net/NetUSB.$(LINUX_KMOD_SUFFIX)
#  AUTOLOAD:=$(call AutoLoad,80,GPL_NetUSB NetUSB)
endef
define KernelPackage/netusb/description
 Kernel modules for Net Usb device
endef
$(eval $(call KernelPackage,netusb))
