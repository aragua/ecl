
ifeq ($(CONFIG_ECL_SYS_LOG_PRINTF),y)
TARGETS += sys_log_printf
TARGET_OBJS+= sys_log_printf.o
TARGET_HDRS+= sys_log_printf.h
else ifeq ($(CONFIG_ECL_SYS_LOG_ZLOG),y)
TARGETS += sys_log_printf
TARGET_OBJS+= sys_log_printf.o
TARGET_HDRS+= sys_log_printf.h
else
$(error Unknown logging system)
endif
