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
* File Name        : r_cg_main.c
* Version          : 1.0.140
* Device(s)        : R7F701649
* Description      : This file implements main function.
***********************************************************************************************************************/
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_smc_entry.h"
/* Start user code for include. Do not edit comment generated here */
#include "state_machine.h"
#include "UART_Functions.h"
#include "temperature_reading.h"
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

#define SENSOR_SLAVE_ADDRESS	(0x44)

// Globale State Machine Instanz
StateMachine_t g_StateMachine;

// Globale Event-Variable (könnte auch eine Queue oder ein Bitfeld sein)
volatile Event_t g_event = EVT_NONE;

// Maybe thes variables are obsolete -> State machine needs to handle this
MD_STATUS	err;
extern volatile uint8_t          g_riic0_state;
uint8_t g_useStateMachine = 1U; // Turn on the state machine -> Off for testing the individual peripherals
extern uint8_t uart_isr_status_flag_send_complete;
TemperatureMeasurement g_temp_measurement;
rtc_counter_value_t g_rtc_value;

// DMA TESTING AREA TODO: Delete if the code is Working
uint8_t len;
#define I2C_RECEIVE_SIZE 	4

volatile uint8_t i2c_receive_buffer[I2C_RECEIVE_SIZE];
volatile uint8_t i2c_receive_buffer_dma[I2C_RECEIVE_SIZE];
extern uint8_t i2c_transmit_flag;
extern uint8_t i2c_receive_flag;
volatile uint32_t *ptr = (uint32_t *)0xFEF01000;
volatile uint32_t *ptr2 = (uint32_t *)0xFEF01004;

// Test Function
//void testFunction(void)
//{
//	uint32_t cnt_sfty =0;
//	R_Systeminit();
//	EI();
//
//	// Start I2C Peripheral
//	R_Config_RIIC0_Start();
//
//	// Init DMA
//	PDMA0.DCEN0  	= 0x0;	// Channel disable
//	PDMA0.DDA0 		= 0xFEF01004;//- 0x200000;//(uint32_t) &i2c_receive_buffer_dma - 0x200000; 		// Destination Address (CPU Self area offset)
//	PDMA0.DSA0 		= (uint32_t) &RIIC0.DRR;//&RIIC0.DRR;					// Source Address
//
//	// 1101 1011
//
//	PDMA0.DCEN0  	= 0x1;	// Channel Enable
//	/* Address Mode:
//	   - Source Address: Fixed (IICRX immer gleiche Adresse)
//	   - Destination Address: Incremented (Empfangspuffer inkrementieren)
//	*/
//	//PDMA0.DMCRB = 0x00000000; // Default-Einstellung passt meistens
//
//	// Set the Memory protection (PEG register) b.c. I want to access the local RAM
//	// Schritt 1: PEG global aktivieren
//	PEG.SP.UINT32 |= 0x00000001;       // Bit 0:0      Accesses by the external master having SPID is enabled
//
//
//	// Schritt 2: Maske für Region 0 setzen (64 KB)
//	PEG.G0MK.UINT32 = (END_OF_RAM - START_OF_RAM) & 0xFFFFF000;   // Address Mask
//
//	// Schritt 3: Basisadresse und Rechte setzen:
//	// Bit 0: GnEN  = 1 → Bereich aktiv
//	// Bit 1: GnRD  = 1 → Lesezugriff erlaubt
//	// Bit 2: GnWR  = 1 → Schreibzugriff erlaubt
//	// Bit 4: GnSP0 = 1 → SPID 0 darf zugreifen
//	// Bits 12–31: Basisadresse (0xFEDE0000 >> 12 = 0xFEDE0)
//	PEG.G0BA.UINT32 = START_OF_RAM + 0x00000017;    // Bit 12-31:   Address 0xfebc0000
//													// Bit 5:0      Access by the external master having SPID = 1 is disabled.
//													// Bit 4:1      Access by the external master having SPID = 0 is enabled.
//													// Bit 2:1      Write access enabled
//													// Bit 1:1      Read access enabled
//													// Bit 0:1      PEG area enabled
//	// Start the DMA
//	R_Config_DMAC00_Start();
//
//	i2c_transmit_flag 	= 0;
//	cnt_sfty			= 0;
//
//	R_Config_RIIC0_Master_Send(SENSOR_SLAVE_ADDRESS,"", 0);
//
//	while(i2c_transmit_flag	==	0U)
//	{
//		if(cnt_sfty >= SAVETY_CNT)
//		{
//			return;
//		}
//		cnt_sfty++;
//	}
//
//	cnt_sfty = 0;
//
//	// Delay
//	while(cnt_sfty < DELAY_CNT)
//	{
//		cnt_sfty++;
//	}
//
//	cnt_sfty = 0;
//	// Measure
//	memset(i2c_receive_buffer, 0, sizeof(i2c_receive_buffer));
//	memset(i2c_receive_buffer_dma, 0, sizeof(i2c_receive_buffer_dma));
//
//
//	*ptr = 0x12345678;
//
//	R_Config_RIIC0_Master_Receive_custom(0x44);
//
//	// Delay
//	while(cnt_sfty < DELAY_CNT)
//	{
//		cnt_sfty++;
//	}
//
//	cnt_sfty = 0;
//}
/* End user code. Do not edit comment generated here */
void r_main_userinit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    r_main_userinit();
    /* Start user code for main. Do not edit comment generated here */
    //testFunction();



    if(g_useStateMachine == 1U)
    {
		// Init the state machine
		StateMachine_Init(&g_StateMachine);


		// Start the state machine with the INIT State
		g_StateMachine.stateFunctions[g_StateMachine.currentState]();

		// Main Loop
		while (1)
		{
			//When an event is raised, the new State must be handled
			if (g_event != EVT_NONE)
			{
				StateMachine_HandleEvent(&g_StateMachine, g_event);

				// Reset the event after handling
				g_event = EVT_NONE;

				// Processing of the State function
				g_StateMachine.stateFunctions[g_StateMachine.currentState]();
			}
		} // end while
	} // End use state machine
} // end main

/***********************************************************************************************************************
* Function Name: r_main_userinit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_main_userinit(void)
{
    DI();
    /* Start user code for r_main_userinit. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}


/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
