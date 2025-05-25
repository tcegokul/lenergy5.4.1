#ifndef _VL53L0X_DEVICE_STRUCT_H_
#define _VL53L0X_DEVICE_STRUCT_H_

#include "vl53l0x_types.h"  // ✅ Pour FixPoint1616_t et GpioFunctionality

// ✅ Structure complète utilisée dans Dev->DeviceSpecificParameters
typedef struct {
    VL53L0X_GpioFunctionality Pin0GpioFunctionality;

    // Champs explicitement utilisés dans le code :
    uint8_t RefSpadsInitialised;
    uint8_t ReferenceSpadCount;
    uint8_t ReferenceSpadType;
    uint8_t ReadDataFromDeviceDone;
    uint32_t OscFrequencyMHz;
    uint8_t ModuleId;
    uint8_t Revision;
#ifndef VL53L0X_MAX_STRING_LENGTH
#define VL53L0X_MAX_STRING_LENGTH 16
#endif
char ProductId[VL53L0X_MAX_STRING_LENGTH];


    uint32_t PartUIDUpper;
    uint32_t PartUIDLower;

    FixPoint1616_t SignalRateMeasFixed400mm;

    // Optionnels mais parfois nécessaires :
    uint16_t SigmaEstRefArray;
    uint16_t SigmaEstEffPulseWidth;
    uint16_t SigmaEstEffAmbWidth;

    uint16_t LastEncodedTimeout;
    uint32_t FinalRangeTimeoutMicroSecs;
    uint8_t FinalRangeVcselPulsePeriod;
    uint32_t PreRangeTimeoutMicroSecs;
    uint8_t PreRangeVcselPulsePeriod;

} VL53L0X_DeviceSpecificParameters_t;


// ✅ Macros d’accès standardisées
#define VL53L0XDevDataGet(dev, field)       (((VL53L0X_DevData_t*)(dev))->field)
#define PALDevDataGet(dev, field)           (((VL53L0X_DevData_t*)(dev))->field)
#define PALDevDataSet(dev, field, value)    (((VL53L0X_DevData_t*)(dev))->field = (value))

#endif // _VL53L0X_DEVICE_STRUCT_H_
