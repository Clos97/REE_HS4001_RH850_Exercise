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
* File Name        : Config_RTCA0.c
* Component Version: 1.5.0
* Device(s)        : R7F701649
* Description      : This file implements device driver for Config_RTCA0.
***********************************************************************************************************************/
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_RTCA0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint32_t g_cg_sync_read;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Create
* Description  : This function initializes the RTCA0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Create(void)
{
    /* Stop counter operation */
    RTCA0.CTL0.BIT.CE = _RTC_COUNTER_STOP;
    /* Disable RTC interrupt (INTRTCA0R) operation and clear request */
    INTC2.ICRTCA0R.BIT.MKRTCA0R = _INT_PROCESSING_DISABLED;
    INTC2.ICRTCA0R.BIT.RFRTCA0R = _INT_REQUEST_NOT_OCCUR;
    /* Disable RTC interrupt (INTRTCA0AL) operation and clear request */
    INTC2.ICRTCA0AL.BIT.MKRTCA0AL = _INT_PROCESSING_DISABLED;
    INTC2.ICRTCA0AL.BIT.RFRTCA0AL = _INT_REQUEST_NOT_OCCUR;
    /* Disable RTC interrupt (INTRTCA01S) operation and clear request */
    INTC2.ICRTCA01S.BIT.MKRTCA01S = _INT_PROCESSING_DISABLED;
    INTC2.ICRTCA01S.BIT.RFRTCA01S = _INT_REQUEST_NOT_OCCUR;
    /* Set RTC operation setting */
    RTCA0.CTL0.BIT.AMPM = _RTC_COUNTER_FORMAT_24;
    RTCA0.CTL0.BIT.SLSB = _RTC_OPERATION_MODE_FREQUENCY;
    RTCA0.CTL1.UINT8 = _RTC_OUTPUT_1HZ_DISABLE | _RTC_INT_ALARM_DISABLE | _RTC_INT_1SECOND_DISABLE | 
                       _RTC_INTERVAL_INT_DISABLE;
    /* Set counter overflow value */
    RTCA0.SCMP = _RTC_SUB_COUNTER_COMPARE_VALUE;
    /* Set real-time clock */
    RTCA0.YEAR = _RTC_COUNTER_YEAR;
    RTCA0.MONTH = _RTC_COUNTER_MONTH;
    RTCA0.DAY = _RTC_COUNTER_DAY;
    RTCA0.WEEK = _RTC_COUNTER_WEEK;
    RTCA0.HOUR = _RTC_COUNTER_HOUR;
    RTCA0.MIN = _RTC_COUNTER_MIN;
    RTCA0.SEC = _RTC_COUNTER_SEC;
    /* Synchronization processing */
    g_cg_sync_read = RTCA0.CTL0.UINT8;
    __syncp();

    R_Config_RTCA0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Start
* Description  : This function enables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Start(void)
{
    volatile uint32_t w_count;
    /* Start counter operation */
    RTCA0.CTL0.BIT.CE = _RTC_COUNTER_START;
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < _RTC_WAITTIME_2FRTC; w_count++ )
    {
        NOP();
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Stop
* Description  : This function disables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Stop(void)
{
    volatile uint32_t w_count;

    /* Stop counter operation */
    RTCA0.CTL0.BIT.CE = _RTC_COUNTER_STOP;
    /* Synchronization processing */
    g_cg_sync_read = RTCA0.CTL0.UINT8;
    __syncp();
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < _RTC_WAITTIME_2FRTC; w_count++ )
    {
        NOP();
    }
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_HourSystem
* Description  : This function chooses 12-hour system or 24-hour system.
* Arguments    : hour_system -
*                    the selection of real-time clock hour system
* Return Value : status -
*                    MD_OK, MD_ARGERROR, MD_BUSY1 or MD_BUSY2
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Set_HourSystem(rtc_hour_system_t hour_system)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;

    switch (hour_system)
    {
        case HOUR12:
            if (RTCA0.CTL0.BIT.AMPM == _RTC_COUNTER_FORMAT_24)
            {
                /* Stop all clock counter */
                RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_STOP;

                /* Change the waiting time according to the system */
                for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
                {
                    NOP();
                }

                if (RTCA0.CTL2.BIT.WST != _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
                {
                    status = MD_BUSY1;
                }
                else
                {
                    /* Select 12-hour format */
                    RTCA0.CTL0.BIT.AMPM = _RTC_COUNTER_FORMAT_12;
                    RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_RESTART;

                    /* Change the waiting time according to the system */
                    for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
                    {
                        NOP();
                    }

                    if (RTCA0.CTL2.BIT.WST == _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
                    {
                        status = MD_BUSY2;
                    }
                }
            }
            break;
        case HOUR24:
            if (RTCA0.CTL0.BIT.AMPM == _RTC_COUNTER_FORMAT_12)
            {
                /* Stop all clock counter */
                RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_STOP;

                /* Change the waiting time according to the system */
                for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
                {
                    NOP();
                }

                if (RTCA0.CTL2.BIT.WST != _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
                {
                    status = MD_BUSY1;
                }
                else
                {
                    /* Select 24-hour format */
                    RTCA0.CTL0.BIT.AMPM = _RTC_COUNTER_FORMAT_24;
                    /* Start all clock counter */
                    RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_RESTART;

                    /* Change the waiting time according to the system */
                    for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
                    {
                        NOP();
                    }

                    if (RTCA0.CTL2.BIT.WST == _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
                    {
                        status = MD_BUSY2;
                    }
                }
            }
            break;

        default:
            status = MD_ARGERROR;
            break;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Get_CounterBufferValue
* Description  : This function reads the count buffer register and store them in the variables.
* Arguments    : counter_read_val -
*                    the current real-time clock value (BCD code)
* Return Value : status -
*                    MD_OK, MD_BUSY1, MD_BUSY2
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Get_CounterBufferValue(rtc_counter_value_t * const counter_read_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;

    /* Stop all clock counter */
    RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_STOP;

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
    {
        NOP();
    }

    if (RTCA0.CTL2.BIT.WST != _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
    {
        status = MD_BUSY1;
    }
    else
    {
        counter_read_val->sec = RTCA0.SEC;
        counter_read_val->min = RTCA0.MIN;
        counter_read_val->hour = RTCA0.HOUR;
        counter_read_val->week = RTCA0.WEEK;
        counter_read_val->day = RTCA0.DAY;
        counter_read_val->month = RTCA0.MONTH;
        counter_read_val->year = RTCA0.YEAR;
        /* Start all clock counter */
        RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_RESTART;

        /* Change the waiting time according to the system */
        for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
        {
            NOP();
        }

        if (RTCA0.CTL2.BIT.WST == _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
        {
            status = MD_BUSY2;
        }
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Get_CounterDirectValue
* Description  : This function reads the counter register directly and store them in the variables.
* Arguments    : counter_read_val -
*                    the current real-time clock value (BCD code)
* Return Value : status -
*                    MD_OK, MD_ERROR
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Get_CounterDirectValue(rtc_counter_value_t * const counter_read_val)
{
    MD_STATUS status = MD_OK;
    uint8_t  t_sec_1st;
    uint8_t  t_sec_2nd;

    t_sec_1st = RTCA0.SECC;
    counter_read_val->sec = RTCA0.SECC;
    counter_read_val->min = RTCA0.MINC;
    counter_read_val->hour = RTCA0.HOURC;
    counter_read_val->week = RTCA0.WEEKC;
    counter_read_val->day = RTCA0.DAYC;
    counter_read_val->month = RTCA0.MONC;
    counter_read_val->year = RTCA0.YEARC;
    t_sec_2nd = RTCA0.SECC;

    if (t_sec_2nd != t_sec_1st)
    {
        status = MD_ERROR;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_CounterValue
* Description  : This function changes the real-time clock value.
* Arguments    : counter_write_val -
*                    the expected real-time clock value (BCD code)
* Return Value : status -
*                    MD_OK, MD_BUSY1 or MD_BUSY2
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Set_CounterValue(rtc_counter_value_t counter_write_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;

    /* Stop all clock counter */
    RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_STOP;

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
    {
        NOP();
    }

    if (RTCA0.CTL2.BIT.WST != _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
    {
        status = MD_BUSY1;
    }
    else
    {
        RTCA0.SEC = counter_write_val.sec;
        RTCA0.MIN = counter_write_val.min;
        RTCA0.HOUR = counter_write_val.hour;
        RTCA0.WEEK = counter_write_val.week;
        RTCA0.DAY = counter_write_val.day;
        RTCA0.MONTH = counter_write_val.month;
        RTCA0.YEAR = counter_write_val.year;
        /* Start all clock counter */
        RTCA0.CTL2.BIT.WAIT = _RTC_ALL_CLOCK_COUNTERS_RESTART;

        /* Change the waiting time according to the system */
        for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
        {
            NOP();
        }

        if (RTCA0.CTL2.BIT.WST == _RTC_ALL_CLOCK_COUNTERS_STATUS_STOPPED)
        {
            status = MD_BUSY2;
        }
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Get_SubCounterValue
* Description  : This function reads buffer register for the sub-counter.
* Arguments    : subcounter_read_val -
*                    buffer register for the sub-counter
* Return Value : status -
*                    MD_OK, MD_BUSY1
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Get_SubCounterValue(uint32_t * const subcounter_read_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;

    /* Trigger the read operation */
    RTCA0.CTL2.BIT.RSUB = _RTC_TRANSFER_TRIGGERED;

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
    {
        NOP();
    }

    if (RTCA0.CTL2.BIT.RSST != _RTC_TRANSFER_COMPLETED)
    {
        status = MD_BUSY1;
    }
    else
    {
        *subcounter_read_val = RTCA0.SRBU;
        /* Disable the read operation */
        RTCA0.CTL2.BIT.RSUB = _RTC_TRANSFER_STATUS_CLEARED;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_ErrorCorrectionValue
* Description  : This function changes the error correction value.
* Arguments    : errorcorrection_write_val -
*                    the expected error correction value
* Return Value : status -
*                    MD_OK, MD_BUSY1 or MD_BUSY2
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Set_ErrorCorrectionValue(uint8_t const errorcorrection_write_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;

    if (RTCA0.CTL0.BIT.CEST != _RTC_COUNTER_STATUS_ENABLE)
    {
        /* Enable counter operation */
        RTCA0.CTL0.BIT.CE = _RTC_COUNTER_START;
    }

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
    {
        NOP();
    }

    if (RTCA0.CTL0.BIT.CEST != _RTC_COUNTER_STATUS_ENABLE)
    {
        status = MD_BUSY1;
    }

    if (RTCA0.CTL2.BIT.WUST != _RTC_SUBU_WRITE_COMPLETED )
    {
        status = MD_BUSY2;
    }
    else
    {
        RTCA0.SUBU = errorcorrection_write_val;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_SubCounterCompareValue
* Description  : This function changes sub-counter compare register.
* Arguments    : subcompare_write_val -
*                    sub-counter compare value
* Return Value : status -
*                    MD_OK, MD_BUSY1 or MD_BUSY2
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Set_SubCounterCompareValue(uint32_t const subcompare_write_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;

    if (RTCA0.CTL0.BIT.CEST != _RTC_COUNTER_STATUS_ENABLE)
    {
        /* Enable counter operation */
        RTCA0.CTL0.BIT.CE = _RTC_COUNTER_START;
    }

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < _RTC_WAITTIME; w_count++ )
    {
        NOP();
    }

    if (RTCA0.CTL0.BIT.CEST != _RTC_COUNTER_STATUS_ENABLE)
    {
        status = MD_BUSY1;
    }

    if (RTCA0.CTL2.BIT.WSST != _RTC_SCMP_WRITE_COMPLETED)
    {
        status = MD_BUSY2;
    }
    else
    {
        RTCA0.SCMP = subcompare_write_val;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_AlarmValue
* Description  : This function sets alarm value.
* Arguments    : alarm_val -
*                    the expected alarm value (BCD code)
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_AlarmValue(rtc_alarm_value_t alarm_val)
{
    /* Set alarm clock */
    RTCA0.ALM = alarm_val.alarmwm;
    RTCA0.ALH = alarm_val.alarmwh;
    RTCA0.ALW = alarm_val.alarmww;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Get_AlarmValue
* Description  : This function sets alarm interrupr on.
* Arguments    : alarm_val -
*                    the address to save alarm value (BCD code)
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Get_AlarmValue(rtc_alarm_value_t * const alarm_val)
{
    /* Get alarm clock */
    alarm_val->alarmwm = RTCA0.ALM;
    alarm_val->alarmwh = RTCA0.ALH;
    alarm_val->alarmww = RTCA0.ALW;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_AlarmOn
* Description  : This function sets alarm interrupr on.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_AlarmOn(void)
{
    /* Clear RTC interrupt (INTRTCA0AL) request and enable operation */
    INTC2.ICRTCA0AL.BIT.RFRTCA0AL = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICRTCA0AL.BIT.MKRTCA0AL = _INT_PROCESSING_ENABLED;
    /* Enable alarm interrupt */
    RTCA0.CTL1.BIT.ENALM = _RTC_INT_ENABLE;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_AlarmOff
* Description  : This function sets alarm interrupr off.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_AlarmOff(void)
{
    /* Disable alarm interrupt */
    RTCA0.CTL1.BIT.ENALM = _RTC_INT_DISABLE;
    /* Disable RTC interrupt (INTRTCA0AL) operation and clear request */
    INTC2.ICRTCA0AL.BIT.MKRTCA0AL = _INT_PROCESSING_DISABLED;
    INTC2.ICRTCA0AL.BIT.RFRTCA0AL = _INT_REQUEST_NOT_OCCUR;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_ConstPeriodInterruptOn
* Description  : This function enables fixed interval interrupt.
* Arguments    : period -
*                    the constant period of INTRTC
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_Config_RTCA0_Set_ConstPeriodInterruptOn(rtc_int_period_t period)
{
    MD_STATUS status = MD_OK;

    if ((period < QUARTERSEC) || (period > ONEMONTH))
    {
        status = MD_ARGERROR;
    }
    else
    {
        /* Disable RTC interrupt (INTRTCA0R)  operation */
        INTC2.ICRTCA0R.BIT.MKRTCA0R = _INT_PROCESSING_DISABLED;
        /* Set interval setting */
        RTCA0.CTL1.UINT8 &= _RTC_INTERVAL_INT_CLEAR;
        RTCA0.CTL1.UINT8 |= period;
        /* Clear RTC interrupt (INTRTCA0R) request and enable operation */
        INTC2.ICRTCA0R.BIT.RFRTCA0R = _INT_REQUEST_NOT_OCCUR;
        INTC2.ICRTCA0R.BIT.MKRTCA0R = _INT_PROCESSING_ENABLED;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_ConstPeriodInterruptOff
* Description  : This function disables fixed interval interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_ConstPeriodInterruptOff(void)
{
    /* Disable const period interrupt */
    RTCA0.CTL1.UINT8 &= _RTC_INTERVAL_INT_CLEAR;
    /* Disable RTC interrupt (INTRTCA0R) operation and clear request */
    INTC2.ICRTCA0R.BIT.MKRTCA0R = _INT_PROCESSING_DISABLED;
    INTC2.ICRTCA0R.BIT.RFRTCA0R = _INT_REQUEST_NOT_OCCUR;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_1secondInterruptOn
* Description  : This function sets 1-second interrupr on.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_1secondInterruptOn(void)
{
    /* Clear RTC interrupt (INTRTCA01S) request and enable operation */
    INTC2.ICRTCA01S.BIT.RFRTCA01S = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICRTCA01S.BIT.MKRTCA01S = _INT_PROCESSING_ENABLED;
    /* Enable 1 second interrupt */
    RTCA0.CTL1.BIT.EN1S = _RTC_INT_ENABLE;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_1secondInterruptOff
* Description  : This function sets 1-second interrupr off.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_1secondInterruptOff(void)
{
    /* Disable 1 second interrupt */
    RTCA0.CTL1.BIT.EN1S = _RTC_INT_DISABLE;
    /* Disable RTC interrupt (INTRTCA01S) operation and clear request */
    INTC2.ICRTCA01S.BIT.MKRTCA01S = _INT_PROCESSING_DISABLED;
    INTC2.ICRTCA01S.BIT.RFRTCA01S = _INT_REQUEST_NOT_OCCUR;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_RTCA1HZOn
* Description  : This function enables RTC1HZ output.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_RTCA1HZOn(void)
{
    /* Enable 1Hz output */
    RTCA0.CTL1.BIT.EN1HZ = _RTC_OUTPUT_ENABLE;
}

/***********************************************************************************************************************
* Function Name: R_Config_RTCA0_Set_RTCA1HZOff
* Description  : This function disables RTC1HZ output.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_RTCA0_Set_RTCA1HZOff(void)
{
    /* Disable 1Hz output */
    RTCA0.CTL1.BIT.EN1HZ = _RTC_OUTPUT_DISABLE;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
