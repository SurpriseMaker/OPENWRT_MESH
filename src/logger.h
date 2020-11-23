#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


#define STD_OUT 0

#define dbg_time(fmt, args...) do { \
	log_debug(LOG_DEBUG, "[MESH: %s:%d] " fmt, __func__, __LINE__, ## args); \
} while(0)

void log_debug(int level, const char * format, ... );
void log_debug_ex(int level, const char * format, ... );


#endif
