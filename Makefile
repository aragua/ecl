#################################################################################
#										#
#    Easy C library 								#
#    Copyright (C) 2013 Lahoudere Fabien					#
#										#
#    This program is free software; you can redistribute it and/or modify	#
#    it under the terms of the GNU General Public License as published by	#
#    the Free Software Foundation; either version 2 of the License, or		#
#    (at your option) any later version.					#
#										#
#    This program is distributed in the hope that it will be useful,		#
#    but WITHOUT ANY WARRANTY; without even the implied warranty of		#
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		#
#    GNU General Public License for more details.				#
#										#
#    You should have received a copy of the GNU General Public License along	#
#    with this program; if not, write to the Free Software Foundation, Inc.,	#
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.		#
#										#
#################################################################################

BASE=${shell pwd}
TOOLS=$(BASE)/tools/
MCONF=$(TOOLS)/bin/kconfig-mconf
KCONFIG_ENTRY=Config.in
CONFIG=$(BASE)/.config

TARGETS=

TARGET_OBJS=
TARGET_HDRS=
TARGET_TESTS=


noconfig_targets:=menuconfig nconfig gconfig xconfig config oldconfig randconfig \
	%_defconfig allyesconfig allnoconfig silentoldconfig release \
	randpackageconfig allyespackageconfig allnopackageconfig \
	source-check print-version olddefconfig


ifeq ($(filter $(noconfig_targets),$(MAKECMDGOALS)),)
ifeq ($(shell ls $(CONFIG)  2> /dev/null),)
$(error No .config : do make menuconfig)
else
include $(CONFIG)
include system/system.mk
include src/src.mk
endif
endif

all: #$(TARGET_OBJS)
	@echo $(TARGET_OBJS)
	@echo $(TARGET_HDRS)
	@echo $(TARGETS)
	@echo $(CONFIG_ECL_LD) $(CONFIG_ECL_LDFLAGS) -shared $(TARGET_OBJS)


clean:


menuconfig: $(MCONF) $(KCONFIG_ENTRY)
	$(MCONF) $(KCONFIG_ENTRY)

$(MCONF): $(TOOLS) 
	wget http://ymorin.is-a-geek.org/download/kconfig-frontends/kconfig-frontends-3.12.0.0.tar.xz
	tar xvf kconfig-frontends-3.12.0.0.tar.xz
	cd kconfig-frontends-3.12.0.0; ./configure --prefix=$(TOOLS)
	make -C kconfig-frontends-3.12.0.0
	make -C kconfig-frontends-3.12.0.0 install
	rm -rf kconfig-frontends-3.12.0.0*
	
$(TOOLS) : 
	@mkdir -p $@
	
.PHONY: all clean menuconfig
