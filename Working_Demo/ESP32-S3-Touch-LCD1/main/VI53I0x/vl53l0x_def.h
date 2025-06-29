#ifndef _VL53L0X_DEF_H_
#define _VL53L0X_DEF_H_

#include <stdint.h>
#include <stdbool.h>
#include "vl53l0x_types.h"
#include "vl53l0x_error_codes.h"

// ⚠️ NE PAS inclure vl53l0x_device.h ici !

#define VL53L0X_SPEED_OF_LIGHT_IN_AIR 299704  // en mm/s

#define VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE           0
#define VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE     1
#define VL53L0X_CHECKENABLE_SIGNAL_REF_CLIP             2
#define VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD      3
#define VL53L0X_CHECKENABLE_SIGNAL_RATE_MSRC            4
#define VL53L0X_CHECKENABLE_SIGNAL_RATE_PRE_RANGE       5
#define VL53L0X_CHECKENABLE_SIGMA_MSRC                  6
#define VL53L0X_CHECKENABLE_SIGMA_PRE_RANGE             7

#define VL53L0X_CHECKENABLE_NUMBER_OF_CHECKS            6

#define VL53L0X_REG_ALGO_PART_TO_PART_RANGE_OFFSET_MM      0x28
#define VL53L0X_REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0       0xB0
#define VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG                 0x01
#define VL53L0X_REG_RESULT_PEAK_SIGNAL_RATE_REF            0xB6
#define VL53L0X_REG_DYNAMIC_SPAD_REF_EN_START_OFFSET       0x4F
#define VL53L0X_REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD    0x4E
#define VL53L0X_REG_GLOBAL_CONFIG_REF_EN_START_SELECT      0xB6
#define VL53L0X_REG_POWER_MANAGEMENT_GO1_POWER_FORCE       0x80
#define VL53L0X_REG_SYSRANGE_START                         0x00
#define VL53L0X_REG_SYSRANGE_MODE_START_STOP               0x01

// ✅ SpadData struct doit être ici
#define VL53L0X_REF_SPAD_BUFFER_SIZE 6

typedef struct {
    uint8_t RefSpadEnables[VL53L0X_REF_SPAD_BUFFER_SIZE];
    uint8_t RefGoodSpadMap[VL53L0X_REF_SPAD_BUFFER_SIZE];
} VL53L0X_SpadData_t;


// ✅ Pas d'inclusion de vl53l0x_device.h ici → éviter les boucles
#ifdef __cplusplus
extern "C" {
#endif

#include "vl53l0x_device_struct.h"

/** @defgroup VL53L0X_globaldefine_group VL53L0X Defines
 *	@brief	  VL53L0X Defines
 *	@{
 */


/** PAL SPECIFICATION major version */
#define VL53L0X10_SPECIFICATION_VER_MAJOR   1
/** PAL SPECIFICATION minor version */
#define VL53L0X10_SPECIFICATION_VER_MINOR   2
/** PAL SPECIFICATION sub version */
#define VL53L0X10_SPECIFICATION_VER_SUB	   7
/** PAL SPECIFICATION sub version */
#define VL53L0X10_SPECIFICATION_VER_REVISION 1440

/** VL53L0X PAL IMPLEMENTATION major version */
#define VL53L0X10_IMPLEMENTATION_VER_MAJOR	1
/** VL53L0X PAL IMPLEMENTATION minor version */
#define VL53L0X10_IMPLEMENTATION_VER_MINOR	0
/** VL53L0X PAL IMPLEMENTATION sub version */
#define VL53L0X10_IMPLEMENTATION_VER_SUB		9
/** VL53L0X PAL IMPLEMENTATION sub version */
#define VL53L0X10_IMPLEMENTATION_VER_REVISION	3673

/** PAL SPECIFICATION major version */
#define VL53L0X_SPECIFICATION_VER_MAJOR	 1
/** PAL SPECIFICATION minor version */
#define VL53L0X_SPECIFICATION_VER_MINOR	 2
/** PAL SPECIFICATION sub version */
#define VL53L0X_SPECIFICATION_VER_SUB	 7
/** PAL SPECIFICATION sub version */
#define VL53L0X_SPECIFICATION_VER_REVISION 1440

/** VL53L0X PAL IMPLEMENTATION major version */
#define VL53L0X_IMPLEMENTATION_VER_MAJOR	  1
/** VL53L0X PAL IMPLEMENTATION minor version */
#define VL53L0X_IMPLEMENTATION_VER_MINOR	  0
/** VL53L0X PAL IMPLEMENTATION sub version */
#define VL53L0X_IMPLEMENTATION_VER_SUB	  4
/** VL53L0X PAL IMPLEMENTATION sub version */
#define VL53L0X_IMPLEMENTATION_VER_REVISION	  4960
#define VL53L0X_DEFAULT_MAX_LOOP 2000

//#define VL53L0X_MAX_STRING_LENGTH 32


/****************************************
 * PRIVATE define do not edit
 ****************************************/

/** @brief Defines the parameters of the Get Version Functions
 */
typedef struct {
	uint32_t	 revision; /*!< revision number */
	uint8_t		 major;	   /*!< major number */
	uint8_t		 minor;	   /*!< minor number */
	uint8_t		 build;	   /*!< build number */
} VL53L0X_Version_t;


/** @brief Defines the parameters of the Get Device Info Functions
 */
typedef struct {
	char Name[VL53L0X_MAX_STRING_LENGTH];
		/*!< Name of the Device e.g. Left_Distance */
	char Type[VL53L0X_MAX_STRING_LENGTH];
		/*!< Type of the Device e.g VL53L0X */
	char ProductId[VL53L0X_MAX_STRING_LENGTH];
		/*!< Product Identifier String	*/
	uint8_t ProductType;
		/*!< Product Type, VL53L0X = 1, VL53L1 = 2 */
	uint8_t ProductRevisionMajor;
		/*!< Product revision major */
	uint8_t ProductRevisionMinor;
		/*!< Product revision minor */
} VL53L0X_DeviceInfo_t;


/** @defgroup VL53L0X_define_Error_group Error and Warning code returned by API
 *	The following DEFINE are used to identify the PAL ERROR
 *	@{
 */

typedef int8_t VL53L0X_Error;

#define VL53L0X_ERROR_NONE			   ((VL53L0X_Error)	0)
#define VL53L0X_ERROR_CALIBRATION_WARNING	   ((VL53L0X_Error) - 1)
	/*!< Warning invalid calibration data may be in used
	 *	\a	VL53L0X_InitData()
	 *	\a VL53L0X_GetOffsetCalibrationData
	 *	\a VL53L0X_SetOffsetCalibrationData
	 */
#define VL53L0X_ERROR_MIN_CLIPPED		   ((VL53L0X_Error) - 2)
	/*!< Warning parameter passed was clipped to min before to be applied */

#define VL53L0X_ERROR_UNDEFINED			   ((VL53L0X_Error) - 3)
	/*!< Unqualified error */
#define VL53L0X_ERROR_INVALID_PARAMS		   ((VL53L0X_Error) - 4)
	/*!< Parameter passed is invalid or out of range */
#define VL53L0X_ERROR_NOT_SUPPORTED		   ((VL53L0X_Error) - 5)
	/*!< Function is not supported in current mode or configuration */
#define VL53L0X_ERROR_RANGE_ERROR		   ((VL53L0X_Error) - 6)
	/*!< Device report a ranging error interrupt status */
#define VL53L0X_ERROR_TIME_OUT			   ((VL53L0X_Error) - 7)
	/*!< Aborted due to time out */
#define VL53L0X_ERROR_MODE_NOT_SUPPORTED		   ((VL53L0X_Error) - 8)
	/*!< Asked mode is not supported by the device */
#define VL53L0X_ERROR_BUFFER_TOO_SMALL		   ((VL53L0X_Error) - 9)
	/*!< ... */
#define VL53L0X_ERROR_GPIO_NOT_EXISTING		   ((VL53L0X_Error) - 10)
	/*!< User tried to setup a non-existing GPIO pin */
#define VL53L0X_ERROR_GPIO_FUNCTIONALITY_NOT_SUPPORTED  ((VL53L0X_Error) - 11)
	/*!< unsupported GPIO functionality */
#define VL53L0X_ERROR_INTERRUPT_NOT_CLEARED		((VL53L0X_Error) - 12)
	/*!< Error during interrupt clear */
#define VL53L0X_ERROR_CONTROL_INTERFACE		   ((VL53L0X_Error) - 20)
	/*!< error reported from IO functions */
#define VL53L0X_ERROR_INVALID_COMMAND		   ((VL53L0X_Error) - 30)
	/*!< The command is not allowed in the current device state
	 *	(power down)
	 */
#define VL53L0X_ERROR_DIVISION_BY_ZERO		   ((VL53L0X_Error) - 40)
	/*!< In the function a division by zero occurs */
#define VL53L0X_ERROR_REF_SPAD_INIT		   ((VL53L0X_Error) - 50)
	/*!< Error during reference SPAD initialization */
#define VL53L0X_ERROR_NOT_IMPLEMENTED		   ((VL53L0X_Error) - 99)
	/*!< Tells requested functionality has not been implemented yet or
	 * not compatible with the device
	 */
/** @} VL53L0X_define_Error_group */


/** @defgroup VL53L0X_define_DeviceModes_group Defines Device modes
 *	Defines all possible modes for the device
 *	@{
 */
typedef uint8_t VL53L0X_DeviceModes;

#define VL53L0X_DEVICEMODE_SINGLE_RANGING	   ((VL53L0X_DeviceModes)  0)
#define VL53L0X_DEVICEMODE_CONTINUOUS_RANGING	   ((VL53L0X_DeviceModes)  1)
#define VL53L0X_DEVICEMODE_SINGLE_HISTOGRAM	   ((VL53L0X_DeviceModes)  2)
#define VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING ((VL53L0X_DeviceModes)  3)
#define VL53L0X_DEVICEMODE_SINGLE_ALS		   ((VL53L0X_DeviceModes) 10)
#define VL53L0X_DEVICEMODE_GPIO_DRIVE		   ((VL53L0X_DeviceModes) 20)
#define VL53L0X_DEVICEMODE_GPIO_OSC		   ((VL53L0X_DeviceModes) 21)
	/* ... Modes to be added depending on device */
/** @} VL53L0X_define_DeviceModes_group */



/** @defgroup VL53L0X_define_HistogramModes_group Defines Histogram modes
 *	Defines all possible Histogram modes for the device
 *	@{
 */
typedef uint8_t VL53L0X_HistogramModes;

#define VL53L0X_HISTOGRAMMODE_DISABLED		((VL53L0X_HistogramModes) 0)
	/*!< Histogram Disabled */
#define VL53L0X_HISTOGRAMMODE_REFERENCE_ONLY	((VL53L0X_HistogramModes) 1)
	/*!< Histogram Reference array only */
#define VL53L0X_HISTOGRAMMODE_RETURN_ONLY	((VL53L0X_HistogramModes) 2)
	/*!< Histogram Return array only */
#define VL53L0X_HISTOGRAMMODE_BOTH		 ((VL53L0X_HistogramModes) 3)
	/*!< Histogram both Reference and Return Arrays */
	/* ... Modes to be added depending on device */
/** @} VL53L0X_define_HistogramModes_group */


/** @defgroup VL53L0X_define_PowerModes_group List of available Power Modes
 *	List of available Power Modes
 *	@{
 */

typedef uint8_t VL53L0X_PowerModes;

#define VL53L0X_POWERMODE_STANDBY_LEVEL1 ((VL53L0X_PowerModes) 0)
	/*!< Standby level 1 */
#define VL53L0X_POWERMODE_STANDBY_LEVEL2 ((VL53L0X_PowerModes) 1)
	/*!< Standby level 2 */
#define VL53L0X_POWERMODE_IDLE_LEVEL1	((VL53L0X_PowerModes) 2)
	/*!< Idle level 1 */
#define VL53L0X_POWERMODE_IDLE_LEVEL2	((VL53L0X_PowerModes) 3)
	/*!< Idle level 2 */

/** @} VL53L0X_define_PowerModes_group */


#define VL53L0X_DMAX_LUT_SIZE 7
	/*!< Defines the number of items in the DMAX lookup table */

/** @brief Structure defining data pair that makes up the DMAX Lookup table.
 */
typedef struct {
	FixPoint1616_t ambRate_mcps[VL53L0X_DMAX_LUT_SIZE];
		/*!< Ambient rate (mcps) */
	FixPoint1616_t dmax_mm[VL53L0X_DMAX_LUT_SIZE];
		/*!< DMAX Value (mm)  */
} VL53L0X_DMaxLUT_t;

/** @brief Defines all parameters for the device
 */
typedef struct {
	VL53L0X_DeviceModes DeviceMode;
	/*!< Defines type of measurement to be done for the next measure */
	VL53L0X_HistogramModes HistogramMode;
	/*!< Defines type of histogram measurement to be done for the next
	 *	measure
	 */
	uint32_t MeasurementTimingBudgetMicroSeconds;
	/*!< Defines the allowed total time for a single measurement */
	uint32_t InterMeasurementPeriodMilliSeconds;
	/*!< Defines time between two consecutive measurements (between two
	 *	measurement starts). If set to 0 means back-to-back mode
	 */
	uint8_t XTalkCompensationEnable;
	/*!< Tells if Crosstalk compensation shall be enable or not	 */
	uint16_t XTalkCompensationRangeMilliMeter;
	/*!< CrossTalk compensation range in millimeter	 */
	FixPoint1616_t XTalkCompensationRateMegaCps;
	/*!< CrossTalk compensation rate in Mega counts per seconds.
	 *	Expressed in 16.16 fixed point format.
	 */
	int32_t RangeOffsetMicroMeters;
	/*!< Range offset adjustment (mm).	*/

	uint8_t LimitChecksEnable[VL53L0X_CHECKENABLE_NUMBER_OF_CHECKS];
	/*!< This Array store all the Limit Check enable for this device. */
	uint8_t LimitChecksStatus[VL53L0X_CHECKENABLE_NUMBER_OF_CHECKS];
	/*!< This Array store all the Status of the check linked to last
	 * measurement.
	 */
	FixPoint1616_t LimitChecksValue[VL53L0X_CHECKENABLE_NUMBER_OF_CHECKS];
	/*!< This Array store all the Limit Check value for this device */

	VL53L0X_DMaxLUT_t dmax_lut;
	/*!< Lookup table defining ambient rates and associated
	 * dmax values.
	 */

	uint8_t WrapAroundCheckEnable;
	/*!< Tells if Wrap Around Check shall be enable or not */
} VL53L0X_DeviceParameters_t;


/** @defgroup VL53L0X_define_State_group Defines the current status
 * of the device
 *	Defines the current status of the device
 *	@{
 */

typedef uint8_t VL53L0X_State;

#define VL53L0X_STATE_POWERDOWN		 ((VL53L0X_State)  0)
	/*!< Device is in HW reset	*/
#define VL53L0X_STATE_WAIT_STATICINIT ((VL53L0X_State)  1)
	/*!< Device is initialized and wait for static initialization  */
#define VL53L0X_STATE_STANDBY		 ((VL53L0X_State)  2)
	/*!< Device is in Low power Standby mode   */
#define VL53L0X_STATE_IDLE			 ((VL53L0X_State)  3)
	/*!< Device has been initialized and ready to do measurements  */
#define VL53L0X_STATE_RUNNING		 ((VL53L0X_State)  4)
	/*!< Device is performing measurement */
#define VL53L0X_STATE_UNKNOWN		 ((VL53L0X_State)  98)
	/*!< Device is in unknown state and need to be rebooted	 */
#define VL53L0X_STATE_ERROR			 ((VL53L0X_State)  99)
	/*!< Device is in error state and need to be rebooted  */

/** @} VL53L0X_define_State_group */


/**
 * @struct VL53L0X_RangeData_t
 * @brief Range measurement data.
 */
typedef struct {
	uint32_t TimeStamp;			  /*!< 32-bit time stamp. */
	uint32_t MeasurementTimeUsec;
		/*!< Give the Measurement time needed by the device to do the
		 * measurement.
		 */


	uint16_t RangeMilliMeter;	  /*!< range distance in millimeter. */

	uint16_t RangeDMaxMilliMeter;
		/*!< Tells what is the maximum detection distance of the device
		 * in current setup and environment conditions (Filled when
		 *	applicable)
		 */

	FixPoint1616_t SignalRateRtnMegaCps;
		/*!< Return signal rate (MCPS)\n these is a 16.16 fix point
		 *	value, which is effectively a measure of target
		 *	 reflectance.
		 */
	FixPoint1616_t AmbientRateRtnMegaCps;
		/*!< Return ambient rate (MCPS)\n these is a 16.16 fix point
		 *	value, which is effectively a measure of the ambien
		 *	t light.
		 */

	uint16_t EffectiveSpadRtnCount;
		/*!< Return the effective SPAD count for the return signal.
		 *	To obtain Real value it should be divided by 256
		 */

	uint8_t ZoneId;
		/*!< Denotes which zone and range scheduler stage the range
		 *	data relates to.
		 */
	uint8_t RangeFractionalPart;
		/*!< Fractional part of range distance. Final value is a
		 *	FixPoint168 value.
		 */
	uint8_t RangeStatus;
		/*!< Range Status for the current measurement. This is device
		 *	dependent. Value = 0 means value is valid.
		 *	See \ref RangeStatusPage
		 */
} VL53L0X_RangingMeasurementData_t;


#define VL53L0X_HISTOGRAM_BUFFER_SIZE 24

/**
 * @struct VL53L0X_HistogramData_t
 * @brief Histogram measurement data.
 */
typedef struct {
	/* Histogram Measurement data */
	uint32_t HistogramData[VL53L0X_HISTOGRAM_BUFFER_SIZE];
	/*!< Histogram data */
	/*!< Indicate the types of histogram data :
	 *Return only, Reference only, both Return and Reference
	 */
	uint8_t FirstBin; /*!< First Bin value */
	uint8_t BufferSize; /*!< Buffer Size - Set by the user.*/
	uint8_t NumberOfBins;
	/*!< Number of bins filled by the histogram measurement */

	VL53L0X_DeviceError ErrorStatus;
	/*!< Error status of the current measurement. \n
	 * see @a ::VL53L0X_DeviceError @a VL53L0X_GetStatusErrorString()
	 */
} VL53L0X_HistogramMeasurementData_t;


/**
 * @struct VL53L0X_SpadData_t
 * @brief Spad Configuration Data.
 */

/* ❌ Redéclaration inutile – déjà définie dans vl53l0x_device_struct.h
typedef struct {
    FixPoint1616_t OscFrequencyMHz;
    uint16_t LastEncodedTimeout;
    uint32_t FinalRangeTimeoutMicroSecs;
    uint8_t FinalRangeVcselPulsePeriod;
    uint32_t PreRangeTimeoutMicroSecs;
    uint8_t PreRangeVcselPulsePeriod;
    uint16_t SigmaEstRefArray;
    uint16_t SigmaEstEffPulseWidth;
    uint16_t SigmaEstEffAmbWidth;
    uint8_t ReadDataFromDeviceDone;
    uint8_t ModuleId;
    uint8_t Revision;
    char ProductId[VL53L0X_MAX_STRING_LENGTH];
    uint8_t ReferenceSpadCount;
    uint8_t ReferenceSpadType;
    uint8_t RefSpadsInitialised;
    uint32_t PartUIDUpper;
    uint32_t PartUIDLower;
    FixPoint1616_t SignalRateMeasFixed400mm;
} VL53L0X_DeviceSpecificParameters_t;
*/

// On ne redéfinit pas cette struct ici : elle est déjà déclarée dans vl53l0x_device_struct.h
// struct VL53L0X_DeviceSpecificParameters_t;

/**
 * @struct VL53L0X_DevData_t
 *
 * @brief VL53L0X PAL device ST private data structure \n
 * End user should never access any of these field directly
 *
 * These must never access directly but only via macro
 */
typedef struct {
	int32_t	 Part2PartOffsetNVMMicroMeter;
	int32_t	 Part2PartOffsetAdjustmentNVMMicroMeter;
	VL53L0X_DeviceParameters_t CurrentParameters;
	VL53L0X_RangingMeasurementData_t LastRangeMeasure;
	VL53L0X_HistogramMeasurementData_t LastHistogramMeasure;
	VL53L0X_DeviceSpecificParameters_t DeviceSpecificParameters;
	uint8_t SequenceConfig;
	uint8_t RangeFractionalEnable;
	VL53L0X_State PalState;
	VL53L0X_PowerModes PowerMode;
	uint16_t SigmaEstRefArray;
	uint16_t SigmaEstEffPulseWidth;
	uint16_t SigmaEstEffAmbWidth;
	uint8_t StopVariable;
	uint16_t targetRefRate;
	FixPoint1616_t SigmaEstimate;
	FixPoint1616_t SignalEstimate;
	FixPoint1616_t LastSignalRefMcps;
	uint8_t *pTuningSettingsPointer;
	uint8_t UseInternalTuningSettings;
	uint16_t LinearityCorrectiveGain;

	// ✅ Champs ajoutés pour corriger toutes les erreurs restantes :
	int32_t RangeOffsetMicroMeters;
	uint32_t OscFrequencyMHz;
	uint8_t RefSpadsInitialised;
	uint8_t ReferenceSpadCount;
	uint8_t ReferenceSpadType;
	uint8_t ReadDataFromDeviceDone;

	// ✅ Champ manquant pour les accès à .SpadData
	VL53L0X_SpadData_t SpadData;

} VL53L0X_DevData_t;

/** @defgroup VL53L0X_define_InterruptPolarity_group Defines the Polarity
 * of the Interrupt
 *	Defines the Polarity of the Interrupt
 *	@{
 */
typedef uint8_t VL53L0X_InterruptPolarity;

#define VL53L0X_INTERRUPTPOLARITY_LOW	   ((VL53L0X_InterruptPolarity)	0)
/*!< Set active low polarity best setup for falling edge. */
#define VL53L0X_INTERRUPTPOLARITY_HIGH	   ((VL53L0X_InterruptPolarity)	1)
/*!< Set active high polarity best setup for rising edge. */

/** @} VL53L0X_define_InterruptPolarity_group */


/** @defgroup VL53L0X_define_VcselPeriod_group Vcsel Period Defines
 *	Defines the range measurement for which to access the vcsel period.
 *	@{
 */
typedef uint8_t VL53L0X_VcselPeriod;

#define VL53L0X_VCSEL_PERIOD_PRE_RANGE	((VL53L0X_VcselPeriod) 0)
/*!<Identifies the pre-range vcsel period. */
#define VL53L0X_VCSEL_PERIOD_FINAL_RANGE ((VL53L0X_VcselPeriod) 1)
/*!<Identifies the final range vcsel period. */

/** @} VL53L0X_define_VcselPeriod_group */

/** @defgroup VL53L0X_define_SchedulerSequence_group Defines the steps
 * carried out by the scheduler during a range measurement.
 *	@{
 *	Defines the states of all the steps in the scheduler
 *	i.e. enabled/disabled.
 */
typedef struct {
	uint8_t		 TccOn;	   /*!<Reports if Target Centre Check On  */
	uint8_t		 MsrcOn;	   /*!<Reports if MSRC On  */
	uint8_t		 DssOn;		   /*!<Reports if DSS On  */
	uint8_t		 PreRangeOn;   /*!<Reports if Pre-Range On	*/
	uint8_t		 FinalRangeOn; /*!<Reports if Final-Range On  */
} VL53L0X_SchedulerSequenceSteps_t;

/** @} VL53L0X_define_SchedulerSequence_group */

/** @defgroup VL53L0X_define_SequenceStepId_group Defines the Polarity
 *	of the Interrupt
 *	Defines the the sequence steps performed during ranging..
 *	@{
 */
typedef uint8_t VL53L0X_SequenceStepId;

#define	 VL53L0X_SEQUENCESTEP_TCC		 ((VL53L0X_VcselPeriod) 0)
/*!<Target CentreCheck identifier. */
#define	 VL53L0X_SEQUENCESTEP_DSS		 ((VL53L0X_VcselPeriod) 1)
/*!<Dynamic Spad Selection function Identifier. */
#define	 VL53L0X_SEQUENCESTEP_MSRC		 ((VL53L0X_VcselPeriod) 2)
/*!<Minimum Signal Rate Check function Identifier. */
#define	 VL53L0X_SEQUENCESTEP_PRE_RANGE	 ((VL53L0X_VcselPeriod) 3)
/*!<Pre-Range check Identifier. */
#define	 VL53L0X_SEQUENCESTEP_FINAL_RANGE ((VL53L0X_VcselPeriod) 4)
/*!<Final Range Check Identifier. */

#define	 VL53L0X_SEQUENCESTEP_NUMBER_OF_CHECKS			 5
/*!<Number of Sequence Step Managed by the API. */

/** @} VL53L0X_define_SequenceStepId_group */


/* MACRO Definitions */
/** @defgroup VL53L0X_define_GeneralMacro_group General Macro Defines
 *	General Macro Defines
 *	@{
 */

/* Defines */
#define VL53L0X_SETPARAMETERFIELD(Dev, field, value) \
	PALDevDataSet(Dev, CurrentParameters.field, value)

#define VL53L0X_GETPARAMETERFIELD(Dev, field, variable) \
	(variable = ((PALDevDataGet(Dev, CurrentParameters)).field))


#define VL53L0X_SETARRAYPARAMETERFIELD(Dev, field, index, value) \
	PALDevDataSet(Dev, CurrentParameters.field[index], value)

#define VL53L0X_GETARRAYPARAMETERFIELD(Dev, field, index, variable) \
	(variable = (PALDevDataGet(Dev, CurrentParameters)).field[index])

#define VL53L0X_SETDEVICESPECIFICPARAMETER(Dev, field, value) \
		PALDevDataSet(Dev, DeviceSpecificParameters.field, value)

#define VL53L0X_GETDEVICESPECIFICPARAMETER(Dev, field) \
		PALDevDataGet(Dev, DeviceSpecificParameters).field


#define VL53L0X_FIXPOINT1616TOFIXPOINT97(Value) \
	(uint16_t)((Value>>9)&0xFFFF)
#define VL53L0X_FIXPOINT97TOFIXPOINT1616(Value) \
	(FixPoint1616_t)(Value<<9)

#define VL53L0X_FIXPOINT1616TOFIXPOINT88(Value) \
	(uint16_t)((Value>>8)&0xFFFF)
#define VL53L0X_FIXPOINT88TOFIXPOINT1616(Value) \
	(FixPoint1616_t)(Value<<8)

#define VL53L0X_FIXPOINT1616TOFIXPOINT412(Value) \
	(uint16_t)((Value>>4)&0xFFFF)
#define VL53L0X_FIXPOINT412TOFIXPOINT1616(Value) \
	(FixPoint1616_t)(Value<<4)

#define VL53L0X_FIXPOINT1616TOFIXPOINT313(Value) \
	(uint16_t)((Value>>3)&0xFFFF)
#define VL53L0X_FIXPOINT313TOFIXPOINT1616(Value) \
	(FixPoint1616_t)(Value<<3)

#define VL53L0X_FIXPOINT1616TOFIXPOINT08(Value) \
	(uint8_t)((Value>>8)&0x00FF)
#define VL53L0X_FIXPOINT08TOFIXPOINT1616(Value) \
	(FixPoint1616_t)(Value<<8)

#define VL53L0X_FIXPOINT1616TOFIXPOINT53(Value) \
	(uint8_t)((Value>>13)&0x00FF)
#define VL53L0X_FIXPOINT53TOFIXPOINT1616(Value) \
	(FixPoint1616_t)(Value<<13)

#define VL53L0X_FIXPOINT1616TOFIXPOINT102(Value) \
	(uint16_t)((Value>>14)&0x0FFF)
#define VL53L0X_FIXPOINT102TOFIXPOINT1616(Value) \
	(FixPoint1616_t)(Value<<12)

#define VL53L0X_MAKEUINT16(lsb, msb) (uint16_t)((((uint16_t)msb)<<8) + \
		(uint16_t)lsb)

/** @} VL53L0X_define_GeneralMacro_group */

/** @} VL53L0X_globaldefine_group */

// ✅ Ne surtout pas redéfinir VL53L0X_DevData_t ici si déjà défini plus haut
// Supprimé : typedef struct VL53L0X_DevData_t { ... } VL53L0X_DevData_t;

// ✅ Fin propre du fichier
#ifdef __cplusplus
}
#endif

#endif /* _VL53L0X_DEF_H_ */
