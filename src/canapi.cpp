/*
 * canapi.cpp
 *
 *  Created on: 25 Aug 2016
 *      Author: fwidmaier
 */

#include "canapi.h"


void InitECana()        // Initialize eCAN-A module
{
	/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return
 false data. */

	struct ECAN_REGS ECanaShadow;

	ENABLE_PROTECTED_REGISTER_WRITE_MODE;


	/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/

	ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
	ECanaShadow.CANTIOC.bit.TXFUNC = 1;
	ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

	ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
	ECanaShadow.CANRIOC.bit.RXFUNC = 1;
	ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;


	/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
	// HECC mode also enables time-stamping feature

	// only use SSC mode
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.SCB = 0;
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;


	/* Initialize all bits of 'Message Control Register' to zero */
	// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
	// all bits (including reserved bits) of MSGCTRL must be initialized to zero

	ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;


	// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
	//  as a matter of precaution.

	ECanaRegs.CANTA.all = 0xFFFFFFFF;   /* Clear all TAn bits */
	ECanaRegs.CANRMP.all = 0xFFFFFFFF;  /* Clear all RMPn bits */
	ECanaRegs.CANGIF0.all = 0xFFFFFFFF; /* Clear all interrupt flag bits */
	ECanaRegs.CANGIF1.all = 0xFFFFFFFF;


	/* Configure bit timing parameters for eCANA*/

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 1;            // Set CCR = 1
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;


	// Wait until the CPU has been granted permission to change the configuration registers
	do
	{
		ECanaShadow.CANES.all = ECanaRegs.CANES.all;
	} while(ECanaShadow.CANES.bit.CCE != 1 );       // Wait for CCE bit to be set..

	ECanaShadow.CANBTC.all = 0;

	/* The following block is for 80 MHz SYSCLKOUT. (40 MHz CAN module clock Bit rate = 1 Mbps
       See Note at end of file. */

	// 125 kBit/s
	ECanaShadow.CANBTC.bit.BRPREG = 9;
	ECanaShadow.CANBTC.bit.TSEG1REG = 12;
	ECanaShadow.CANBTC.bit.TSEG2REG = 3;
	ECanaShadow.CANBTC.bit.SJWREG = 1;
	ECanaShadow.CANBTC.bit.SAM = 1;
	ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;


	// Wait until the CPU no longer has permission to change the configuration registers
	do
	{
		ECanaShadow.CANES.all = ECanaRegs.CANES.all;
	} while(ECanaShadow.CANES.bit.CCE != 0 );       // Wait for CCE bit to be  cleared..


	/* Disable all Mailboxes  */

	ECanaRegs.CANME.all = 0;        // Required before writing the MSGIDs


	DISABLE_PROTECTED_REGISTER_WRITE_MODE;
}


void InitECanaGpio(HAL_Handle halHandle)
{
	HAL_Obj *obj = (HAL_Obj *)halHandle;

	/* Enable internal pull-up for the selected CAN pins */
	// Pull-ups can be enabled or disabled by the user.
	// This will enable the pullups for the specified pins.
	// Comment out other unwanted lines.
	GPIO_setPullup(obj->gpioHandle, GPIO_Number_30, GPIO_Pullup_Enable); // CANRXA
	GPIO_setPullup(obj->gpioHandle, GPIO_Number_31, GPIO_Pullup_Enable); // CANTXA

	/* Set qualification for selected CAN pins to asynch only */
	// Inputs are synchronized to SYSCLKOUT by default.
	// This will select asynch (no qualification) for the selected pins.
	//gpio->GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
	GPIO_setQualification(obj->gpioHandle, GPIO_Number_30, GPIO_Qual_ASync);

	/* Configure eCAN-A pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be eCAN functional pins.
	//gpio->GPAMUX2.bit.GPIO30 = 1;    // Configure GPIO30 for CANRXA operation
	//gpio->GPAMUX2.bit.GPIO31 = 1;    // Configure GPIO31 for CANTXA operation
	GPIO_setMode(obj->gpioHandle, GPIO_Number_30, GPIO_30_Mode_CANRXA);
	GPIO_setMode(obj->gpioHandle, GPIO_Number_31, GPIO_31_Mode_CANTXA);
}

void CAN_setupMboxes()
{
	   // Mailboxs can be written to 16-bits or 32-bits at a time
    // Write to the MSGID field of TRANSMIT mailboxes MBOX0 - 15
    ECanaMboxes.MBOX15.MSGID.all = (uint32_t)CAN_ID_STATUSMSG << 18;
    ECanaMboxes.MBOX14.MSGID.all = (uint32_t)CAN_ID_Iq << 18;
    ECanaMboxes.MBOX13.MSGID.all = (uint32_t)CAN_ID_POSITION << 18;
    ECanaMboxes.MBOX12.MSGID.all = (uint32_t)CAN_ID_SPEED << 18;
    ECanaMboxes.MBOX0.MSGID.all = (uint32_t)CAN_ID_COMMANDS << 18;
//    ECanaMboxes.MBOX1.MSGID.all = 0x9555AAA1;
//    ECanaMboxes.MBOX2.MSGID.all = 0x9555AAA2;
//    ECanaMboxes.MBOX3.MSGID.all = 0x9555AAA3;
//    ECanaMboxes.MBOX4.MSGID.all = 0x9555AAA4;
//    ECanaMboxes.MBOX5.MSGID.all = 0x9555AAA5;
//    ECanaMboxes.MBOX6.MSGID.all = 0x9555AAA6;
//    ECanaMboxes.MBOX7.MSGID.all = 0x9555AAA7;
//    ECanaMboxes.MBOX8.MSGID.all = 0x9555AAA8;
//    ECanaMboxes.MBOX9.MSGID.all = 0x9555AAA9;
//    ECanaMboxes.MBOX10.MSGID.all = 0x9555AAAA;
//    ECanaMboxes.MBOX11.MSGID.all = 0x9555AAAB;
//    ECanaMboxes.MBOX12.MSGID.all = 0x9555AAAC;
//    ECanaMboxes.MBOX13.MSGID.all = 0x9555AAAD;
//    ECanaMboxes.MBOX14.MSGID.all = 0x9555AAAE;
//    ECanaMboxes.MBOX15.MSGID.all = 0x9555AAAF;

    // Write to the MSGID field of RECEIVE mailboxes MBOX16 - 31
//    ECanaMboxes.MBOX16.MSGID.all = 0x9555AAA0;
//    ECanaMboxes.MBOX17.MSGID.all = 0x9555AAA1;
//    ECanaMboxes.MBOX18.MSGID.all = 0x9555AAA2;
//    ECanaMboxes.MBOX19.MSGID.all = 0x9555AAA3;
//    ECanaMboxes.MBOX20.MSGID.all = 0x9555AAA4;
//    ECanaMboxes.MBOX21.MSGID.all = 0x9555AAA5;
//    ECanaMboxes.MBOX22.MSGID.all = 0x9555AAA6;
//    ECanaMboxes.MBOX23.MSGID.all = 0x9555AAA7;
//    ECanaMboxes.MBOX24.MSGID.all = 0x9555AAA8;
//    ECanaMboxes.MBOX25.MSGID.all = 0x9555AAA9;
//    ECanaMboxes.MBOX26.MSGID.all = 0x9555AAAA;
//    ECanaMboxes.MBOX27.MSGID.all = 0x9555AAAB;
//    ECanaMboxes.MBOX28.MSGID.all = 0x9555AAAC;
//    ECanaMboxes.MBOX29.MSGID.all = 0x9555AAAD;
//    ECanaMboxes.MBOX30.MSGID.all = 0x9555AAAE;
//    ECanaMboxes.MBOX31.MSGID.all = 0x9555AAAF;

    // Configure Mailboxes 0-4 for receiving, rest for transmitting
    // Since this write is to the entire register (instead of a bit field) a
    // shadow register is not required.
    ECanaRegs.CANMD.all = 0x0000000F;

    // Enable all used Mailboxes */
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    ECanaRegs.CANME.all = CAN_MBOX_ALL;

    // Specify that 8 bits will be sent/received
//    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
//    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
//    ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
//    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
//    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX7.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX8.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX9.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX10.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX12.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX14.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX15.MSGCTRL.bit.DLC = 8;
}