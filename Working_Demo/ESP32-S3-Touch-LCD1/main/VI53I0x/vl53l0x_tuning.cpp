#include "vl53l0x_api.h"
#include "vl53l0x_def.h"
#include "vl53l0x_platform.h"

extern "C" {

VL53L0X_Error VL53L0X_LoadTuningSettings(VL53L0X_DEV Dev)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    // Réglages de tuning basiques (extrait des API ST)
    Status |= VL53L0X_WrByte(Dev, 0xFF, 0x01);
    Status |= VL53L0X_WrByte(Dev, 0x00, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0xFF, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x09, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x10, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x11, 0x00);

    // Paramètres additionnels (sécurité)
    Status |= VL53L0X_WrByte(Dev, 0x24, 0x01);
    Status |= VL53L0X_WrByte(Dev, 0x25, 0xff);
    Status |= VL53L0X_WrByte(Dev, 0x75, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0xFF, 0x01);
    Status |= VL53L0X_WrByte(Dev, 0x4e, 0x2c);
    Status |= VL53L0X_WrByte(Dev, 0x48, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x30, 0x20);
    Status |= VL53L0X_WrByte(Dev, 0xFF, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x30, 0x09);
    Status |= VL53L0X_WrByte(Dev, 0x54, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x31, 0x04);
    Status |= VL53L0X_WrByte(Dev, 0x32, 0x03);
    Status |= VL53L0X_WrByte(Dev, 0x40, 0x83);
    Status |= VL53L0X_WrByte(Dev, 0x46, 0x25);
    Status |= VL53L0X_WrByte(Dev, 0x60, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x27, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x50, 0x06);
    Status |= VL53L0X_WrByte(Dev, 0x51, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x52, 0x96);
    Status |= VL53L0X_WrByte(Dev, 0x56, 0x08);
    Status |= VL53L0X_WrByte(Dev, 0x57, 0x30);
    Status |= VL53L0X_WrByte(Dev, 0x61, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x62, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x64, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x65, 0x00);
    Status |= VL53L0X_WrByte(Dev, 0x66, 0xa0);

    return Status;
}

VL53L0X_Error VL53L0X_GetVcselPulsePeriod(VL53L0X_DEV Dev,
                                          VL53L0X_VcselPeriod VcselPeriodType,
                                          uint8_t* pVcselPulsePeriod)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t RegisterValue = 0;

    if (pVcselPulsePeriod == nullptr)
        return VL53L0X_ERROR_INVALID_PARAMS;

    switch (VcselPeriodType) {
        case VL53L0X_VCSEL_PERIOD_PRE_RANGE:
            Status = VL53L0X_RdByte(Dev, 0x50, &RegisterValue);
            break;
        case VL53L0X_VCSEL_PERIOD_FINAL_RANGE:
            Status = VL53L0X_RdByte(Dev, 0x70, &RegisterValue);
            break;
        default:
            return VL53L0X_ERROR_INVALID_PARAMS;
    }

    if (Status == VL53L0X_ERROR_NONE) {
        // La valeur est codée selon un facteur de 2
        *pVcselPulsePeriod = (RegisterValue + 1) << 1;
    }

    return Status;
}

} // extern "C"