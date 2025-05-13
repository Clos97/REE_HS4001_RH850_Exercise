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
* File Name        : Config_DMAC00.h
* Component Version: 1.6.0
* Device(s)        : R7F701649
* Description      : This file implements device driver for Config_DMAC00.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_dma.h"

#ifndef CFG_Config_DMAC00_H
#define CFG_Config_DMAC00_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _DMAC00_SOURCE_ADDRESS                (0x00000000UL)
#define _DMAC00_DESTINATION_ADDRESS           (0x00000000UL)
#define _DMAC00_TRANSFER_COUNT                (0x00000004UL)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_DMAC00_Create(void);
void R_Config_DMAC00_Start(void);
void R_Config_DMAC00_Stop(void);
void R_Config_DMAC00_Set_SoftwareTrigger(void);
void R_Config_DMAC00_Suspend(void);
void R_Config_DMAC00_Resume(void);
void R_Config_DMAC00_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */
#define START_OF_RAM		0xFEBE0000
#define END_OF_RAM			0xFEBF7FFF 	// Retention RAM from FEBF:8000-FEBF:FFFF
#define DMA_SRC_ADDRESS		0xFEF01004 // This address is the source address
/* End user code. Do not edit comment generated here */
#endif
