#ifndef SYSTEM_LOG_PRINTF_H
#define SYSTEM_LOG_PRINTF_H

#include <stdio.h>

#define LOG_DEBUG	0
#define LOG_INFO	1
#define LOG_WARN	2
#define LOG_ERR		3
#define LOG_CRIT	4

#define LOG_PRINT(level,fmt,...) \
{\
	switch (level){\
	case LOG_DEBUG:\
		fprintf( stdout, "\033[34m[DEBUG - %s:%d] " fmt "\033[0m", __FILE__, __LINE__, ##__VA_ARGS__);\
		break;\
	case LOG_INFO:\
		fprintf( stdout, "\033[32m" fmt "\033[0m", ##__VA_ARGS__);\
		break;\
	case LOG_WARN:\
		fprintf( stderr, "\033[33m[WARNING - %s:%d] " fmt "\033[0m", __FILE__, __LINE__, ##__VA_ARGS__);\
		break;\
	case LOG_ERR:\
		fprintf( stderr, "\033[31m[ERROR - %s:%d] " fmt "\033[0m", __FILE__, __LINE__, ##__VA_ARGS__);\
		break;\
	case LOG_CRIT:\
		fprintf( stderr, "\x1B[48;5;160m\033[30m[FATAL ERROR - %s:%d] " fmt "\033[0m\x1B[0m", __FILE__, __LINE__, ##__VA_ARGS__);\
		break;\
	default:\
		fprintf( stderr, "[UNKNOWN - %s:%d] " fmt , __FILE__, __LINE__, ##__VA_ARGS__);\
		break;\
	}\
}

#define LOG(level,fmt,...) LOG_PRINT(level,fmt,##__VA_ARGS__)

#endif
