
ifeq ($(CONFIG_ECL_SRC_NETWORK_TCP_CLIENT),y)
TARGETS += src_network_tcp_client
TARGET_OBJS += src_network_tcp_client.o
TARGET_HDRS += src_network_tcp_client.h
endif

ifeq ($(CONFIG_ECL_SRC_NETWORK_TCP_SERVER),y)
TARGETS += src_network_tcp_server
TARGET_OBJS += src_network_tcp_server.o
TARGET_HDRS += src_network_tcp_server.h
endif
