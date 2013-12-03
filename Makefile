#################################################################################
#										#
#    Easy C library 								#
#    Copyright (C) 2013 Lahoudere Fabien					#
#										#
#    This library is free software; you can redistribute it and/or modify	#
#    it under the terms of the GNU Lesser General Public License as published 	#
#    by the Free Software Foundation; either version 2.1 of the License, or	#
#    (at your option) any later version.					#
#										#
#    This library is distributed in the hope that it will be useful,		#
#    but WITHOUT ANY WARRANTY; without even the implied warranty of		#
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		#
#    GNU Lesser General Public License for more details.			#
#										#
#    You should have received a copy of the GNU Lesser General Public License 	#
#    along with this library; if not, write to the Free Software Foundation, 	#
#    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.		#
#										#
#################################################################################

#BASE=${shell pwd}
BASE=.
TOOLS=${shell pwd}/tools
OUTPUT=$(BASE)/output
BINDIR=$(OUTPUT)/bin
LIBDIR=$(OUTPUT)/lib
INCDIR=$(OUTPUT)/include
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

###########################
# Define internal routine #
###########################
# Strip quotes and then whitespaces
qstrip=$(strip $(subst ",,$(1)))
lower_case = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))
upper_case = $(subst a,A,$(subst b,B,$(subst c,C,$(subst d,D,$(subst e,E,$(subst f,F,$(subst g,G,$(subst h,H,$(subst i,I,$(subst j,J,$(subst k,K,$(subst l,L,$(subst m,M,$(subst n,N,$(subst o,O,$(subst p,P,$(subst q,Q,$(subst r,R,$(subst s,S,$(subst t,T,$(subst u,U,$(subst v,V,$(subst w,W,$(subst x,X,$(subst y,Y,$(subst z,Z,$1))))))))))))))))))))))))))
var_to_path=$(subst _,/,$(call lower_case,$1))
path_to_var=$(subst /,_,$(call upper_case,$1))
var_to_obj=$(if $($1_SRCS),$(foreach src,$($1_SRCS),$(call var_to_path,$1)/$(subst .c,.o,$(src))))
var_to_hdr=$(if $($1_HDRS),$(INCDIR)/$(call lower_case,$1).h)
ifeq ($(CONFIG_ECL_TEST),y)
var_to_test=$(if $($1_TEST),$(foreach test,$($1_TEST),$(call var_to_path,$1)/$(subst .c,.test,$(test))))
else
var_to_test=
endif
obj_to_var=$(call upper_case,$(basename $(subst $(OBJDIR)/,,$1)))
obj_to_folder=$(call var_to_path,$(call obj_to_var,$1))
obj_to_src=$(foreach file,$($(call obj_to_var,$1)_SRCS), $(call obj_to_folder,$1)/$(file))
new_hdr_name=$(basename $(subst $(INCDIR)/,,$(notdir $1)))_$(notdir $2)
get_headers=$(foreach hdr,$($(call upper_case,$(notdir $(basename $1)))_HDRS),$(call var_to_path,$(notdir $(basename $1)))/$(hdr))
create_headers=$(foreach hdr,$(call get_headers,$1), cp $(hdr) $(INCDIR)/$(call new_hdr_name,$1,$(hdr)) ; echo \\\#include \"$(call new_hdr_name,$1,$(hdr))\" >> $1 ; )

var_to_targets=$(call var_to_obj,$1) $(call var_to_hdr,$1) $(call var_to_test,$1)


ifeq ($(CONFIG_ECL_STATIC),)
CONFIG_ECL_CFLAGS+="-fPIC"
endif

CC=$(if $(CONFIG_ECL_CC),$(call qstrip,$(CONFIG_ECL_CC)),CC)
CFLAGS=$(call qstrip,$(CONFIG_ECL_CFLAGS))
AR=$(if $(CONFIG_ECL_AR),$(call qstrip,$(CONFIG_ECL_AR)),AR)
LD=$(if $(CONFIG_ECL_LD),$(call qstrip,$(CONFIG_ECL_LD)),LD)
LDFLAGS=$(call qstrip,$(CONFIG_ECL_LDFLAGS))
LIBS=$(call qstrip,$(CONFIG_ECL_LIBS))


FINAL_TARGETS=$(foreach target,$(TARGETS),$(call var_to_targets,$(target)) )

all : see_targets $(OUTPUT) $(BINDIR) $(LIBDIR) $(INCDIR) start_header $(FINAL_TARGETS) finalize_header
	@echo
	@echo "Linking $(TARGETS)"
ifeq ($(CONFIG_ECL_STATIC),y)
	@echo $(AR) rcs $(LIBDIR)/libecl.a $(TARGET_OBJS)
else
	$(LD) -shared -o $(LIBDIR)/libecl.so $(filter %.o,$(FINAL_TARGETS)) $(LDFLAGS) $(LIBS)
endif
	@echo

see_targets:
	@echo
	@echo "Building targets:"
	@echo $(FINAL_TARGETS)
	@echo


%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(@D) -I$(INCDIR)

$(INCDIR)/%.h:
	@echo "#ifndef $(call upper_case,$(subst .,_,$(@F)))" > $@
	@echo "#define $(call upper_case,$(subst .,_,$(@F)))" >> $@
	@echo >> $@
	$(call create_headers,$@)
	@echo >> $@
	@echo "#endif" >> $@
	@echo "#include \"$(@F)\"" >> $(INCDIR)/ecl.h

%.test:%.c
	$(CC) $(CFLAGS) -o $@ $< $(@D)/*.o -I$(@D) -I$(INCDIR)
	mv $@ $(BINDIR)/$(basename $(subst /,-,$@))

start_header: $(INCDIR)
	@echo "#ifndef ECL_H" > $(INCDIR)/ecl.h
	@echo "#define ECL_H" >> $(INCDIR)/ecl.h
	@echo "" >> $(INCDIR)/ecl.h

finalize_header:
	@echo "" >> $(INCDIR)/ecl.h
	@echo "#endif /* ECL_H */" >> $(INCDIR)/ecl.h

clean:
	rm -rf $(OUTPUT) $(filter %.o,$(FINAL_TARGETS)) $(filter %.test,$(FINAL_TARGETS))

menuconfig: $(MCONF) $(KCONFIG_ENTRY)
	$(MCONF) $(KCONFIG_ENTRY)

$(MCONF): $(TOOLS) 
	wget http://ymorin.is-a-geek.org/download/kconfig-frontends/kconfig-frontends-3.12.0.0.tar.xz
	tar xvf kconfig-frontends-3.12.0.0.tar.xz
	cd kconfig-frontends-3.12.0.0; ./configure --prefix=$(TOOLS)
	make -C kconfig-frontends-3.12.0.0
	make -C kconfig-frontends-3.12.0.0 install
	rm -rf kconfig-frontends-3.12.0.0*
	
$(TOOLS) $(OUTPUT) $(BINDIR) $(LIBDIR) $(INCDIR): 
	@mkdir -p $@
	
.PHONY: all build clean menuconfig start_header finalize_header
