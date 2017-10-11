/*  Copyright (c) 2017, Appiko
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its contributors
 *  may be used to endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 *  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @addtogroup group_platform
 * @{
 *
 * @defgroup board_snap_rev1 Snap rev 1
 * @brief The defines for the Snap board 1
 * @{
 */

#ifndef PLATFORM_SNAP_REV1
#define PLATFORM_SNAP_REV1

/** @anchor snap_rev1-leds
 * @name LED definitions for Snap Rev1
 * @note LED_1, LED_2 and LED_3 are defines of the RGB LED
 *  for compatibility with existing examples
 * @{*/
#define LED_RED        4
#define LED_GREEN      6
#define LED_BLUE       7
#define LED_AMBIENT_LIGHT 5

#define LED_1          4
#define LED_2          7
#define LED_3          6
#define LED_A			5
/** The output level at which the LEDs shine */
#define LEDS_ACTIVE_STATE 1
/** @} */

/** @anchor snap_rev1-buttons
 * @name Button definitions for Snap Rev1
 * @{*/
#define BUTTON_1       12
/** The input level when the button is pressed */
#define BUTTONS_ACTIVE_STATE 0
/** The kind of internal resistors required for the button */
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP
/** @} */

/** @anchor snap_rev1-serial
 * @name Serial port definitions for Snap Rev1
 * @{*/
#define RX_PIN_NUMBER  13
#define TX_PIN_NUMBER  17
#define CTS_PIN_NUMBER 15
#define RTS_PIN_NUMBER 16

/**
 * @anchor snap_rev1-ir
 * @name IR pin Snap Rev1
 * @{*
 */
#define IR_PIN 11
#define IR_ACTIVE_STATE 0
/** @} */

/**
 * @anchor snap_rev1-regulator
 * @name Regulator for low or high power mode for Snap Rev1
 * @{*
 */
#define REGULATOR_MODE_PIN 26

/** @} */

/**
 * @anchor snap_rev1-camera_trigger
 * @name Camera Trigger - focus or trigger press for Snap Rev1
 * @{*
 */
#define CAMERA_FOCUS_PIN 9
#define CAMERA_TRIGGER_PIN 8

/** @} */


/** Bool to say if the hardware flow control is required */
#define HWFC           false

/** @} */

///Bool define if the circuitry is present for the internal DC-DC of nRF52
#define DC_DC_CIRCUITRY     true

///Bool define if a NFC Antenna circuitry is present
#define NFC_CIRCUITRY       false

///Bool define if the 32 kHz crystal is present for the LFCLK
#define LFCLK_XTAL_PRESENT  true

///Bool define if a crystal is present for the HFCLK
#define HFCLK_XTAL_PRESENT  true

/** Low frequency clock source used when initializing the SoftDevice */
#define BOARD_LFCLKSRC_STRUCT  {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                    .rc_ctiv       = 0,                                \
                                    .rc_temp_ctiv  = 0,                                \
                                    .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#define BOARD_LFCLKSRC         NRF_CLOCK_LFCLK_Xtal

#endif /* PLATFORM_SNAP_REV1 */

/** @} */
/** @} */
