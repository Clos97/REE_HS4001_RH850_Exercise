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
* File Name        : Config_TAUB0_1.c
* Component Version: 1.6.0
* Device(s)        : R7F701649
* Description      : This file implements device driver for Config_TAUB0_1.
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
#include "Config_TAUB0_1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint32_t g_cg_sync_read;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TAUB0_1_Create
* Description  : This function initializes the TAUB01 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUB0_1_Create(void)
{
    /* Disable channel 1 counter operation */
    TAUB0.TT |= _TAUB_CHANNEL1_COUNTER_STOP;
    /* Disable INTTAUB0I1 operation and clear request */
    INTC2.ICTAUB0I1.BIT.MKTAUB0I1 = _INT_PROCESSING_DISABLED;
    INTC2.ICTAUB0I1.BIT.RFTAUB0I1 = _INT_REQUEST_NOT_OCCUR;
    /* Set INTTAUB0I1 setting */
    INTC2.ICTAUB0I1.BIT.TBTAUB0I1 = _INT_TABLE_VECTOR;
    INTC2.ICTAUB0I1.UINT16 &= _INT_PRIORITY_LOWEST;
    TAUB0.TPS &= _TAUB_CK0_PRS_CLEAR;
    TAUB0.TPS |= _TAUB_CK0_PRE_PCLK_15;
    /* Set channel 1 setting */
    TAUB0.CMOR1 = _TAUB_SELECTION_CK0 | _TAUB_COUNT_CLOCK_PCLK | _TAUB_INDEPENDENT_CHANNEL | _TAUB_SOFTWARE_TRIGGER | 
                  _TAUB_OVERFLOW_AUTO_CLEAR | _TAUB_INTERVAL_TIMER_MODE | _TAUB_START_INT_NOT_GENERATED;
    /* Set compare match register */
    TAUB0.CMUR1 = _TAUB_INPUT_EDGE_UNUSED;
    TAUB0.CDR1 = _TAUB01_COMPARE_VALUE;
    /* Set output mode setting */
    TAUB0.TOE |= _TAUB_CHANNEL1_ENABLES_OUTPUT_MODE;
    TAUB0.TOM &= _TAUB_CHANNEL1_INDEPENDENT_OUTPUT_MODE;
    TAUB0.TOC &= _TAUB_CHANNEL1_OPERATION_MODE1;
    TAUB0.TOL &= _TAUB_CHANNEL1_POSITIVE_LOGIC;
    TAUB0.TDE &= _TAUB_CHANNEL1_DISABLE_DEAD_TIME_OPERATE;
    TAUB0.TDL &= _TAUB_CHANNEL1_POSITIVE_PHASE_PERIOD;
    /* Synchronization processing */
    g_cg_sync_read = TAUB0.TPS;
    __syncp();
    /* Set TAUB0O1 pin */
    PORT.PIBC10 &= _PORT_CLEAR_BIT11;
    PORT.PBDC10 &= _PORT_CLEAR_BIT11;
    PORT.PM10 |= _PORT_SET_BIT11;
    PORT.PMC10 &= _PORT_CLEAR_BIT11;
    PORT.PIPC10 &= _PORT_CLEAR_BIT11;
    PORT.PFC10 |= _PORT_SET_BIT11;
    PORT.PFCE10 |= _PORT_SET_BIT11;
    PORT.PFCAE10 &= _PORT_CLEAR_BIT11;
    PORT.PMC10 |= _PORT_SET_BIT11;
    PORT.PM10 &= _PORT_CLEAR_BIT11;

    R_Config_TAUB0_1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TAUB0_1_Start
* Description  : This function starts the TAUB01 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUB0_1_Start(void)
{
    /* Clear INTTAUB0I1 request and enable operation */
    INTC2.ICTAUB0I1.BIT.RFTAUB0I1 = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICTAUB0I1.BIT.MKTAUB0I1 = _INT_PROCESSING_ENABLED;
    /* Enable channel 1 counter operation */
    TAUB0.TS |= _TAUB_CHANNEL1_COUNTER_START;
}

/***********************************************************************************************************************
* Function Name: R_Config_TAUB0_1_Stop
* Description  : This function stop the TAUB01 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUB0_1_Stop(void)
{
    /* Disable channel 1 counter operation */
    TAUB0.TT |= _TAUB_CHANNEL1_COUNTER_STOP;
    /* Disable INTTAUB0I1 operation and clear request */
    INTC2.ICTAUB0I1.BIT.MKTAUB0I1 = _INT_PROCESSING_DISABLED;
    INTC2.ICTAUB0I1.BIT.RFTAUB0I1 = _INT_REQUEST_NOT_OCCUR;
    /* Synchronization processing */
    g_cg_sync_read = TAUB0.TT;
    __syncp();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
