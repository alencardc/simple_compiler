#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef DEBUG
  #define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ## args)
#else
  #define DEBUG_PRINT(fmt, args...)
#endif // DEBUG

#endif // __DEBUG_H__