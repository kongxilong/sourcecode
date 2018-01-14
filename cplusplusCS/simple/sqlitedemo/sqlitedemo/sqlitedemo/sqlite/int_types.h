
#pragma once

#ifndef     _INT_TYPES_H_
#define     _INT_TYPES_H_

#ifndef     __cplusplus
typedef     unsigned char bool;
#define     true    1
#define     false   0
#endif      /* !__cplusplus */

#ifndef     __GNUC__
#define     __attribute__(p) /* nothing */
#endif
#ifdef      _MSC_VER
#pragma     pack(push, 1)
#endif

#include <tchar.h>

typedef     signed char         int8_t;
typedef     signed short        int16_t;
typedef     signed int          int32_t;
typedef     signed __int64      int64_t;
typedef     unsigned char       uint8_t;
typedef     unsigned short      uint16_t;
typedef     unsigned int        uint32_t;
typedef     unsigned __int64    uint64_t;


#ifdef      _MSC_VER
#pragma     pack(pop)
#endif

#endif // _INT_TYPES_H_