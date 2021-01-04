
include $(TOPDIR)/rules.mk

PKG_NAME:=meshconfig
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/meshconfig
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=This is meshconfig source code
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

TARGET_LDFLAGS := -lpthread -ldl -Wall

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS) -Wall" \
	LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/meshconfig/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/meshserver $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/meshconfig $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/mesh.init $(1)/etc/init.d/mesh
	$(INSTALL_BIN) ./files/topology.sh $(1)/usr/sbin/topo
endef

$(eval $(call BuildPackage,meshconfig))
