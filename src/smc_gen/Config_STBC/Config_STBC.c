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
* File Name        : Config_STBC.c
* Component Version: 1.3.1
* Device(s)        : R7F701649
* Description      : This file implements device driver for Config_STBC.
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
#include "Config_STBC.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_STBC_Prepare_Stop_Mode
* Description  : This function prepares STBC stop mode.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_STBC_Prepare_Stop_Mode(void)
{
    /* Stop all of the peripheral functions to which the clock supply is to be stopped */
    R_Config_STBC_Prepare_Stop_Mode_Set_Peripheral();
    /* Disable interrupts */
    DI();
    /* Clear all interrupt flags */
    R_Config_STBC_Prepare_Stop_Mode_Set_Interrupt();
    /* Clear wake-up factor */
    STBC_WUF0.WUFC0 = _STBC_WUFC0_CLEAR;
    STBC_WUF1.WUFC1 = _STBC_WUFC1_CLEAR;
    STBC_WUFISO.WUFC_ISO0 = _STBC_WUFC_ISO0_CLEAR;
    /* Enable wake-up factor */
    STBC_WUF0.WUFMSK0 = _STBC_FACTOR_DEFAULT_VALUE & _STBC_WUFMSK0_FACTOR_INTP12 & _STBC_WUFMSK0_FACTOR_INTRTCA0AL;
    STBC_WUF1.WUFMSK1 = _STBC_FACTOR_DEFAULT_VALUE;
    STBC_WUFISO.WUFMSK_ISO0 = _STBC_FACTOR_DEFAULT_VALUE;

    /* Set the masks for the clock domains */
    R_Config_STBC_Prepare_Stop_Mode_Set_Clock_Mask();
    /* Set the masks for the source clocks */
    R_Config_STBC_Prepare_Stop_Mode_Set_Clock_Source();
}

/***********************************************************************************************************************
* Function Name: R_Config_STBC_Start_Stop_Mode
* Description  : This function starts Stop Mode.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_STBC_Start_Stop_Mode(void)
{
    WPROTR.PROTCMD0 = _WRITE_PROTECT_COMMAND;
    STBC0.STPT = _STBC_STOP_MODE_ENTERED;
    STBC0.STPT = (uint32_t) ~_STBC_STOP_MODE_ENTERED;
    STBC0.STPT = _STBC_STOP_MODE_ENTERED;
    while (STBC0.STPT == _STBC_STOP_MODE_ENTERED)
    {
        NOP();
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
