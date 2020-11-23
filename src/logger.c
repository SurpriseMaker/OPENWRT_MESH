
/**
 * logread |grep quectel
 */

#include "logger.h"
bool debug_log = false;


void log_debug(int level, const char * format, ... )
{
  va_list args;
  va_start (args, format);
  if (STD_OUT || level == LOG_INFO)
  {
    vprintf(format, args);
  }
  else
    vsyslog (level, format, args);
  va_end (args);
}

void log_debug_ex(int level, const char * format, ... )
{
  if (debug_log)
  {
    va_list args;
    va_start (args, format);
    vsyslog (level, format, args);
    va_end (args);
  }
}

