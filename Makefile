##Copyright (C) 2020 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

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
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/meshserver $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/meshconfig $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/mesh.init $(1)/etc/init.d/mesh
	$(INSTALL_BIN) ./files/cap.sh $(1)/root/cap.sh
	$(INSTALL_BIN) ./files/re.sh $(1)/root/re.sh
endef

$(eval $(call BuildPackage,meshconfig))
