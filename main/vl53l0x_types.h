/**
 * @file  vl53l0x_types.h
 * @brief VL53L0X types definition
 */

#ifndef VL53L0X_TYPES_H_
#define VL53L0X_TYPES_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#error "Error NULL definition should be done. Please add required include"
#endif

#define VL53L0X_MAX_STRING_LENGTH 32 



#if !defined(STDINT_H) && !defined(_GCC_STDINT_H) && !defined(__STDINT_DECLS) && !defined(_GCC_WRAP_STDINT_H)

#pragma message("Please review type definition of STDINT define for your platform and add to list above")

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

#endif /* _STDINT_H */

/** use where fractional values are expected
 * Given a floating point value f, its .16 bit point is (int)(f*(1<<16))
 */
typedef uint32_t FixPoint1616_t;

/** GPIO Functionality */
#ifndef VL53L0X_GPIO_FUNCTIONALITY_TYPEDEF
#define VL53L0X_GPIO_FUNCTIONALITY_TYPEDEF

typedef enum {
    VL53L0X_GPIO_FUNCTIONALITY_OFF = 0,
    VL53L0X_GPIO_FUNCTIONALITY_THRESHOLD_CROSSED_LOW,
    VL53L0X_GPIO_FUNCTIONALITY_THRESHOLD_CROSSED_HIGH,
    VL53L0X_GPIO_FUNCTIONALITY_THRESHOLD_CROSSED_OUT,
    VL53L0X_GPIO_FUNCTIONALITY_NEW_MEASURE_READY = 0x0B
} VL53L0X_GpioFunctionality;

#endif

#ifdef __cplusplus
}
#endif

#endif /* VL53L0X_TYPES_H_ */

