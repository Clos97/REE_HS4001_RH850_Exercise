/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018, 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_RTCA0.h
* Component Version: 1.5.0
* Device(s)        : R7F701649
* Description      : This file implements device driver for Config_RTCA0.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_rtc.h"

#ifndef CFG_Config_RTCA0_H
#define CFG_Config_RTCA0_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _RTC_WAITTIME                          (320U)
#define _RTC_WAITTIME_2FRTC                    (2U)
#define _RTC_INT_DISABLE                       (0x0U)
#define _RTC_INT_ENABLE                        (0x1U)
#define _RTC_OUTPUT_DISABLE                    (0x0U)
#define _RTC_OUTPUT_ENABLE                     (0x1U)
#define _RTC_COUNTER_SEC                       (0x00U)
#define _RTC_COUNTER_MIN                       (0x17U)
#define _RTC_COUNTER_HOUR                      (0x13U)
#define _RTC_COUNTER_DAY                       (0x15U)
#define _RTC_COUNTER_WEEK                      (0x02U)
#define _RTC_COUNTER_MONTH                     (0x04U)
#define _RTC_COUNTER_YEAR                      (0x25U)
#define _RTC_ALARM_MIN                         (0x18U)
#define _RTC_ALARM_HOUR                        (0x13U)
#define _RTC_SUB_COUNTER_COMPARE_VALUE         (0x0001D4BFUL)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct 
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t week;
    uint8_t month;
    uint8_t year;
}rtc_counter_value_t;

typedef struct
{
    uint8_t alarmwm;
    uint8_t alarmwh;
    uint8_t alarmww;
}rtc_alarm_value_t;

typedef enum
{ 
    HOUR12,
    HOUR24
}rtc_hour_system_t;

typedef enum 
{
    QUARTERSEC = 1U,
    HALFSEC,
    ONESEC,
    ONEMIN,
    ONEHOUR,
    ONEDAY,
    ONEMONTH
}rtc_int_period_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_RTCA0_Create(void);
void R_Config_RTCA0_Start(void);
void R_Config_RTCA0_Stop(void);
MD_STATUS R_Config_RTCA0_Set_HourSystem(rtc_hour_system_t hour_system);
MD_STATUS R_Config_RTCA0_Get_CounterBufferValue(rtc_counter_value_t * const counter_read_val);
MD_STATUS R_Config_RTCA0_Get_CounterDirectValue(rtc_counter_value_t * const counter_read_val);
MD_STATUS R_Config_RTCA0_Set_CounterValue(rtc_counter_value_t counter_write_val);
MD_STATUS R_Config_RTCA0_Get_SubCounterValue(uint32_t * const subcounter_read_val);
MD_STATUS R_Config_RTCA0_Set_ErrorCorrectionValue(uint8_t const errorcorrection_write_val);
MD_STATUS R_Config_RTCA0_Set_SubCounterCompareValue(uint32_t const subcompare_write_val);
void R_Config_RTCA0_Set_AlarmOn(void);
void R_Config_RTCA0_Set_AlarmOff(void);
void R_Config_RTCA0_Set_AlarmValue(rtc_alarm_value_t alarm_val);
void R_Config_RTCA0_Get_AlarmValue(rtc_alarm_value_t * const alarm_val);
MD_STATUS R_Config_RTCA0_Set_ConstPeriodInterruptOn(rtc_int_period_t period);
void R_Config_RTCA0_Set_ConstPeriodInterruptOff(void);
void R_Config_RTCA0_Set_1secondInterruptOn(void);
void R_Config_RTCA0_Set_1secondInterruptOff(void);
void R_Config_RTCA0_Set_RTCA1HZOn(void);
void R_Config_RTCA0_Set_RTCA1HZOff(void);
void R_Config_RTCA0_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */
/** Since the time format is sometimes not right ( RTC uses bin values where the Alarm uses BCD values)
 *  BIN: 13 für 13:00 uhr
 *  BCD: 0x13 für 13:00 Uhr */
uint8_t bin_to_bcd(uint8_t val);
uint8_t bcd_to_bin(uint8_t val);
/* End user code. Do not edit comment generated here */
#endif
