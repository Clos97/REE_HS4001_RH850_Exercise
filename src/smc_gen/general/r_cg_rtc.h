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
* File Name        : r_cg_rtc.h
* Version          : 1.0.140
* Device(s)        : R7F701649
* Description      : General header file for RTC peripheral.
***********************************************************************************************************************/

#ifndef RTC_H
#define RTC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    RTCA Control Register 0 (RTCAnCTL0)
*/
/* Starts/stops the sub-counter RTCAnSUBC operation (RTCAnCE) */
#define _RTC_COUNTER_STOP                        (0x0U) /* Stops the sub-counter operation */
#define _RTC_COUNTER_START                       (0x1U) /* Starts the sub-counter operation */
/* Indicates the operation enabled/stopped status of the sub-counter (RTCAnCEST) */
#define _RTC_COUNTER_STATUS_STOP                 (0x0U) /* Operation stopped status */
#define _RTC_COUNTER_STATUS_ENABLE               (0x1U) /* Operation enabled status */
/* Selects the format of the hours counter RTCAnHOURC and the alarm hour setting register RTCAnALH (RTCAnAMPM) */
#define _RTC_COUNTER_FORMAT_12                   (0x0U) /* 12-hour format (1 to 12, am/pm) */
#define _RTC_COUNTER_FORMAT_24                   (0x1U) /* 24-hour format (0 to 23, military time) */
/* Selects the operation mode (RTCAnSLSB) */
#define _RTC_OPERATION_MODE_32768                (0x0U) /* 32.768 kHz mode */
#define _RTC_OPERATION_MODE_FREQUENCY            (0x1U) /* Frequency selection mode */

/*
    RTCA Control Register 1 (RTCAnCTL1)
*/
/* Enables/stops 1-Hz pulse output (RTCAT1HZ) (RTCAnEN1HZ) */
#define _RTC_OUTPUT_1HZ_DISABLE                  (0x00U) /* RTCAT1HZ disabled (RTCAT1HZ is fixed to 0) */
#define _RTC_OUTPUT_1HZ_ENABLE                   (0x20U) /* RTCAT1HZ enabled */
/* Enables/disables alarm interrupt request generation (INTRTCA0AL) (RTCAnENALM) */
#define _RTC_INT_ALARM_DISABLE                   (0x00U) /* INTRTCA0AL disabled */
#define _RTC_INT_ALARM_ENABLE                    (0x10U) /* INTRTCA0AL enabled */
/* Enables/disables 1-second interrupt request generation (INTRTCA01S) (RTCAnEN1S) */
#define _RTC_INT_1SECOND_DISABLE                 (0x00U) /* INTRTCA01S disabled */
#define _RTC_INT_1SECOND_ENABLE                  (0x08U) /* INTRTCA01S enabled */
/* Specifies the fixed interval interrupt request (INTRTCA0R) setting (RTCAnCT[2-0]) */
#define _RTC_INTERVAL_INT_DISABLE                (0x00U) /* No interrupt request generation */
#define _RTC_INTERVAL_INT_025SEC                 (0x01U) /* Every 0.25, 0.5, 0.75 and 1 second */
#define _RTC_INTERVAL_INT_05SEC                  (0x02U) /* Every 0.5 and 1 second */
#define _RTC_INTERVAL_INT_1SEC                   (0x03U) /* Every 1 second */
#define _RTC_INTERVAL_INT_1MIN                   (0x04U) /* Every 1 minute 00 seconds */
#define _RTC_INTERVAL_INT_1HOUR                  (0x05U) /* Every 1 hour 00 minutes 00 seconds */
#define _RTC_INTERVAL_INT_1DAY                   (0x06U) /* Every 1 day 00 hours 00 minutes 00 seconds */
#define _RTC_INTERVAL_INT_1MONTH                 (0x07U) /* Every 1 month first day 00 hours 00 minutes 00 seconds */
#define _RTC_INTERVAL_INT_CLEAR                  (0xF8U) /* Clear fixed interval interrupt */

/*
    RTCA Control Register 2 (RTCAnCTL2)
*/
/* Indicates whether RTCAnSUBU write operation has been completed (RTCAnWUST) */
#define _RTC_SUBU_WRITE_COMPLETED                (0x0U) /* RTCAnSUBU write completed */
#define _RTC_SUBU_WRITE_IN_PROGRESS              (0x1U) /* RTCAnSUBU write in progress */
/* Indicates whether RTCAnSCMP write operation has been completed (RTCAnWSST) */
#define _RTC_SCMP_WRITE_COMPLETED                (0x0U) /* RTCAnSCMP write completed */
#define _RTC_SCMP_WRITE_IN_PROGRESS              (0x1U) /* RTCAnSCMP write in progress */
/* Indicates whether the value of RTCAnSUBC has been transferred to read buffer (RTCAnSRBU) (RTCAnRSST) */
#define _RTC_TRANSFER_IN_PROGRESS                (0x0U) /* Transfer in progress, or waiting for a transfer trigger */
#define _RTC_TRANSFER_COMPLETED                  (0x1U) /* Transfer completed */
/* Triggers transfer of the value of RTCAnSUBC to the buffer (RTCAnSRBU) or clears the transfer state (RTCAnRSUB) */
#define _RTC_TRANSFER_STATUS_CLEARED             (0x0U) /* Transfer status (RTCAnRSST) is cleared */
#define _RTC_TRANSFER_TRIGGERED                  (0x1U) /* Transfer is triggered */
/* Indicates the status of all clock counters (RTCAnWST) */
#define _RTC_ALL_CLOCK_COUNTERS_STATUS_RUNNING   (0x0U) /* All clock counters are running */
#define _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED   (0x1U) /* All clock counters are stopped */
/* Restarts/stops all clock counters (RTCAnWAIT) */
#define _RTC_ALL_CLOCK_COUNTERS_RESTART          (0x0U) /* Restarts all clock counters */
#define _RTC_ALL_CLOCK_COUNTERS_STOP             (0x1U) /* Stops all clock counters temporarily */

/*
    RTCA Clock Error Correction Register (RTCAnSUBU)
*/
/* Specifies how often clock error correction is performed per minute (RTCAnDEV) */
#define _RTC_ERROR_CORRECT_3TIMES                (0x00U) /* Three times every minute (when RTCAnSECC = 00, 20, 40) */
#define _RTC_ERROR_CORRECT_ONCE                  (0x80U) /* Once every minute (when RTCAnSECC equals 00) */
/* Specifies whether the sub-counter value is incremented or decremented (RTCAnF6) */
#define _RTC_ERROR_CORRECT_INCREMENT             (0x00U) /* Incremented (+ correction) */
#define _RTC_ERROR_CORRECT_DECREMENT             (0x40U) /* Decremented (- correction) */
/* Clock error correction value (RTCAnF[5:0]) */
#define _RTC_ERROR_CORRECT_DISABLE               (0x00U) /* clock error correction is not performed*/

/*
    RTCA Day of the Week Count Buffer Register (RTCAnWEEK)
*/
/* Day of the week (RTCAnWEEK[2:0]) */
#define _RTC_WEEK_DAY_SUNDAY                     (0x00U) /* Sunday */
#define _RTC_WEEK_DAY_MONDAY                     (0x01U) /* Monday */
#define _RTC_WEEK_DAY_TUESDAY                    (0x02U) /* Tuesday */
#define _RTC_WEEK_DAY_WEDNESDAY                  (0x03U) /* Wendesday */
#define _RTC_WEEK_DAY_THURSDAY                   (0x04U) /* Thursday */
#define _RTC_WEEK_DAY_FRIDAY                     (0x05U) /* Friday */
#define _RTC_WEEK_DAY_SATURDAY                   (0x06U) /* Saturday */

/*
    RTCA Alarm Day of the Week Setting Register (RTCAnALW)
*/
/* Specifies Saturday when an alarm interrupt request is generated (RTCAnALW6) */
#define _RTC_ALARM_SATURDAY_DISABLE              (0x00U) /* No alarm interrupt request is generated on Saturday */
#define _RTC_ALARM_SATURDAY_ENABLE               (0x40U) /* Alarm interrupt request is generated on Saturday */
/* Specifies Friday when an alarm interrupt request is generated (RTCAnALW5) */
#define _RTC_ALARM_FRIDAY_DISABLE                (0x00U) /* No alarm interrupt request is generated on Friday */
#define _RTC_ALARM_FRIDAY_ENABLE                 (0x20U) /* Alarm interrupt request is generated on Friday */
/* Specifies Thursday when an alarm interrupt request is generated (RTCAnALW4) */
#define _RTC_ALARM_THURSDAY_DISABLE              (0x00U) /* No alarm interrupt request is generated on Thursday */
#define _RTC_ALARM_THURSDAY_ENABLE               (0x10U) /* Alarm interrupt request is generated on Thursday */
/* Specifies Wednesday when an alarm interrupt request is generated (RTCAnALW3) */
#define _RTC_ALARM_WEDNESDAY_DISABLE             (0x00U) /* No alarm interrupt request is generated on Wednesday */
#define _RTC_ALARM_WEDNESDAY_ENABLE              (0x08U) /* Alarm interrupt request is generated on Wednesday */
/* Specifies Tuesday when an alarm interrupt request is generated (RTCAnALW2) */
#define _RTC_ALARM_TUESDAY_DISABLE               (0x00U) /* No alarm interrupt request is generated on Tuesday */
#define _RTC_ALARM_TUESDAY_ENABLE                (0x04U) /* Alarm interrupt request is generated on Tuesday */
/* Specifies Monday when an alarm interrupt request is generated (RTCAnALW6) */
#define _RTC_ALARM_MONDAY_DISABLE                (0x00U) /* No alarm interrupt request is generated on Monday */
#define _RTC_ALARM_MONDAY_ENABLE                 (0x02U) /* Alarm interrupt request is generated on Monday */
/* Specifies Sunday when an alarm interrupt request is generated (RTCAnALW6) */
#define _RTC_ALARM_SUNDAY_DISABLE                (0x00U) /* No alarm interrupt request is generated on Sunday */
#define _RTC_ALARM_SUNDAY_ENABLE                 (0x01U) /* Alarm interrupt request is generated on Sunday */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
