
/**
 * logread |grep quectel
 */

#include "logger.h"


void log_debug(int level, const char * format, ... )
{
  va_list args;
  va_start (args, format);
  if (STD_OUT || level == LOG_INFO)
  {
    vprintf(format, args);
  }

  if (LOG_ENABLE){
  	vsyslog (level, format, args);
  }
  va_end (args);
}

void log_debug_ex(int level, const char * format, ... )
{
  if (LOG_ENABLE)
  {
    va_list args;
    va_start (args, format);
    vsyslog (level, format, args);
    va_end (args);
  }
}

