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
* File Name        : Config_DMAC00_user.c
* Component Version: 1.6.0
* Device(s)        : R7F701649
* Description      : This file implements device driver for Config_DMAC00.
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
#include "Config_DMAC00.h"
/* Start user code for include. Do not edit comment generated here */
#include "state_machine.h"
#include "Config_RIIC0.h"
#include "r_cg_port.h"
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern Event_t g_event;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_DMAC00_Create_UserInit
* Description  : This function adds user code after initializing DMAC0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_DMAC00_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
	// Init DMA
	PDMA0.DCEN0  	= 0x0;	// Channel disable
	PDMA0.DDA0 		= DMA_SRC_ADDRESS ;//- 0x200000;//(uint32_t) &i2c_receive_buffer_dma - 0x200000; 		// Destination Address (CPU Self area offset)
	PDMA0.DSA0 		= (uint32_t) &RIIC0.DRR;	// Source Address

	PDMA0.DCEN0  	= 0x1;	// Channel Enable


	// Set the Memory protection (PEG register) b.c. I want to access the local RAM
	// Schritt 1: PEG global aktivieren
	PEG.SP.UINT32 |= 0x00000001;       // Bit 0:0      Accesses by the external master having SPID is enabled


	// Schritt 2: Maske für Region 0 setzen (64 KB)
	PEG.G0MK.UINT32 = (END_OF_RAM - START_OF_RAM) & 0xFFFFF000;   // Address Mask

	// Schritt 3: Basisadresse und Rechte setzen:
	// Bit 0: GnEN  = 1 → Bereich aktiv
	// Bit 1: GnRD  = 1 → Lesezugriff erlaubt
	// Bit 2: GnWR  = 1 → Schreibzugriff erlaubt
	// Bit 4: GnSP0 = 1 → SPID 0 darf zugreifen
	// Bits 12–31: Basisadresse (0xFEDE0000 >> 12 = 0xFEDE0)
	PEG.G0BA.UINT32 = START_OF_RAM + 0x00000017;    // Bit 12-31:   Address 0xfebc0000
													// Bit 5:0      Access by the external master having SPID = 1 is disabled.
													// Bit 4:1      Access by the external master having SPID = 0 is enabled.
													// Bit 2:1      Write access enabled
													// Bit 1:1      Read access enabled
													// Bit 0:1      PEG area enabled
	// Start the DMA
	//R_Config_DMAC00_Start();

	/**
	 * The DMA Reacts to the Receive Interrupt of the I2C Peripheral. Since We are Sending an empty write command to the sensor
	 * the Interrupt is called once and writes the DRR Register value on the first address (which is 0).
	 * This is a known issue that could be resolved but its not worth the effort
	 */

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_DMAC00_dmac00_interrupt
* Description  : This function handles the INTDMA0 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_DMAC00_dmac00_interrupt(enable=false, channel=60, fpu=true, callt=false)
void r_Config_DMAC00_dmac00_interrupt(void)
{
    /* Start user code for r_Config_DMAC00_dmac00_interrupt. Do not edit comment generated here */
	// The event will be triggered after 3 bytes of data
	//TODO: Schauen WANN diese ISR gecalled wird?!-> GPIO togglen
	// Setze ACKBT = 1 → NACK für letztes Byte
	//RIIC0.MR3.UINT32 &= ~(1U << 2);   // ACKBR = 0
	R_Config_DMAC00_Stop();
	PORT.P0 = (PORT.P0 ==_PORT_Pn6_OUTPUT_LOW)?_PORT_Pn6_OUTPUT_HIGH:_PORT_Pn6_OUTPUT_LOW;
	//*(volatile uint32_t *) 0xFEF01010 = RIIC0.DRR.UINT32;
	while ((RIIC0.SR2.UINT32 & (1U << 5)) == 1);  // Bit 1: RDRF
	PORT.P0 = (PORT.P0 ==_PORT_Pn6_OUTPUT_LOW)?_PORT_Pn6_OUTPUT_HIGH:_PORT_Pn6_OUTPUT_LOW;

	// Set the Register in order to send NACK

	RIIC0.MR3.UINT32 |= (1U << 4);
	RIIC0.MR3.UINT32 |= (1U << 3);


	while ((RIIC0.SR2.UINT32 & (1U << 5)) == 0);  // Bit 1: RDRF

	PORT.P0 = (PORT.P0 ==_PORT_Pn6_OUTPUT_LOW)?_PORT_Pn6_OUTPUT_HIGH:_PORT_Pn6_OUTPUT_LOW;

	// Stop condition
	RIIC0.SR2.UINT32 = ~(1U << 3);
	RIIC0.CR2.UINT32 |= _RIIC_STOP_CONDITION_REQUEST;

	*(volatile uint32_t *) 0xFEF01014 = RIIC0.DRR.UINT32;



	//while ((RIIC0.SR2.UINT32 & (1U << 5)) == 0);  // Bit 1: RDRF

	// Wait for stop
	// Check stop condition
	while ((RIIC0.SR2.UINT32 & (1U << 3)) == 0);

	RIIC0.SR2.UINT32 = ~(1U << 4);
	RIIC0.SR2.UINT32 = ~(1U << 3);

	//while(RIIC0.R2)

	g_event = EVT_DMA_TRANSFER_FINISHED;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
