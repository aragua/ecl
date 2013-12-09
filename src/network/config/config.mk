
ifeq ($(CONFIG_ECL_SRC_NETWORK_CONFIG_SETIP),y)
SRC_NETWORK_CONFIG_SRCS=setip.c
SRC_NETWORK_CONFIG_HDRS=setip.h
SRC_NETWORK_CONFIG_TEST=setip-test.c
endif

ifeq ($(CONFIG_ECL_SRC_NETWORK_CONFIG_NETIFACE),y)
SRC_NETWORK_CONFIG_SRCS+=netiface.c
SRC_NETWORK_CONFIG_HDRS+=netiface.h
SRC_NETWORK_CONFIG_TEST+=netiface-test.c
endif

TARGETS+=SRC_NETWORK_CONFIG
