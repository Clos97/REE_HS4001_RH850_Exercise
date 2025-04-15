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
* File Name        : Config_TAUB0_0.c
* Component Version: 1.6.0
* Device(s)        : R7F701649
* Description      : This file implements device driver for Config_TAUB0_0.
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
#include "Config_TAUB0_0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint32_t g_cg_sync_read;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TAUB0_0_Create
* Description  : This function initializes the TAUB00 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUB0_0_Create(void)
{
    /* Disable channel 0 counter operation */
    TAUB0.TT |= _TAUB_CHANNEL0_COUNTER_STOP;
    /* Disable INTTAUB0I0 operation and clear request */
    INTC2.ICTAUB0I0.BIT.MKTAUB0I0 = _INT_PROCESSING_DISABLED;
    INTC2.ICTAUB0I0.BIT.RFTAUB0I0 = _INT_REQUEST_NOT_OCCUR;
    /* Set INTTAUB0I0 setting */
    INTC2.ICTAUB0I0.BIT.TBTAUB0I0 = _INT_TABLE_VECTOR;
    INTC2.ICTAUB0I0.UINT16 &= _INT_PRIORITY_LEVEL9;
    TAUB0.TPS &= _TAUB_CK0_PRS_CLEAR;
    TAUB0.TPS |= _TAUB_CK0_PRE_PCLK_15;
    /* Set channel 0 setting */
    TAUB0.CMOR0 = _TAUB_SELECTION_CK0 | _TAUB_COUNT_CLOCK_PCLK | _TAUB_INDEPENDENT_CHANNEL | _TAUB_SOFTWARE_TRIGGER | 
                  _TAUB_OVERFLOW_AUTO_CLEAR | _TAUB_INTERVAL_TIMER_MODE | _TAUB_START_INT_GENERATED;
    /* Set compare match register */
    TAUB0.CMUR0 = _TAUB_INPUT_EDGE_UNUSED;
    TAUB0.CDR0 = _TAUB00_COMPARE_VALUE;
    /* Set output mode setting */
    TAUB0.TOE |= _TAUB_CHANNEL0_ENABLES_OUTPUT_MODE;
    TAUB0.TOM &= _TAUB_CHANNEL0_INDEPENDENT_OUTPUT_MODE;
    TAUB0.TOC &= _TAUB_CHANNEL0_OPERATION_MODE1;
    TAUB0.TOL &= _TAUB_CHANNEL0_POSITIVE_LOGIC;
    TAUB0.TDE &= _TAUB_CHANNEL0_DISABLE_DEAD_TIME_OPERATE;
    TAUB0.TDL &= _TAUB_CHANNEL0_POSITIVE_PHASE_PERIOD;
    /* Synchronization processing */
    g_cg_sync_read = TAUB0.TPS;
    __syncp();
    /* Set TAUB0O0 pin */
    PORT.PIBC0 &= _PORT_CLEAR_BIT7;
    PORT.PBDC0 &= _PORT_CLEAR_BIT7;
    PORT.PM0 |= _PORT_SET_BIT7;
    PORT.PMC0 &= _PORT_CLEAR_BIT7;
    PORT.PFC0 |= _PORT_SET_BIT7;
    PORT.PFCE0 |= _PORT_SET_BIT7;
    PORT.PFCAE0 &= _PORT_CLEAR_BIT7;
    PORT.PMC0 |= _PORT_SET_BIT7;
    PORT.PM0 &= _PORT_CLEAR_BIT7;

    R_Config_TAUB0_0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TAUB0_0_Start
* Description  : This function starts the TAUB00 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUB0_0_Start(void)
{
    /* Clear INTTAUB0I0 request and enable operation */
    INTC2.ICTAUB0I0.BIT.RFTAUB0I0 = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICTAUB0I0.BIT.MKTAUB0I0 = _INT_PROCESSING_ENABLED;
    /* Enable channel 0 counter operation */
    TAUB0.TS |= _TAUB_CHANNEL0_COUNTER_START;
}

/***********************************************************************************************************************
* Function Name: R_Config_TAUB0_0_Stop
* Description  : This function stop the TAUB00 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUB0_0_Stop(void)
{
    /* Disable channel 0 counter operation */
    TAUB0.TT |= _TAUB_CHANNEL0_COUNTER_STOP;
    /* Disable INTTAUB0I0 operation and clear request */
    INTC2.ICTAUB0I0.BIT.MKTAUB0I0 = _INT_PROCESSING_DISABLED;
    INTC2.ICTAUB0I0.BIT.RFTAUB0I0 = _INT_REQUEST_NOT_OCCUR;
    /* Synchronization processing */
    g_cg_sync_read = TAUB0.TT;
    __syncp();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
