
ifeq ($(CONFIG_ECL_SYS_LOG_PRINTF),y)
SYSTEM_LOG_SRCS=printf.c
SYSTEM_LOG_HDRS=printf.h
else ifeq ($(CONFIG_ECL_SYS_LOG_ZLOG),y)
SYSTEM_LOG_SRCS+=
SYSTEM_LOG_SRCS+=
else
$(error Unknown logging system)
endif

TARGETS+= SYSTEM_LOG

