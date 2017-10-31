/*  Copyright (c) 2016, Appiko
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
 * @addtogroup group_appln
 * @{
 *
 * @defgroup Test the IR trigger for different cameras
 * @brief 
 * @{
 */

#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"

#include "boards.h"
#include "hal_clocks.h"
#include "hal_nop_delay.h"
#include "hal_gpio.h"
#include "common_util.h"
#include "log.h"
#include "tinyprintf.h"
#include "uart_printf.h"
#include "us_timer.h"
#include "nrf_util.h"
#include "ble_adv.h"
#include "ms_timer.h"
#include "profiler_timer.h"
#include "ir_signal.h"
#include "camera.h"


uint16_t sequence_pos=0;
uint16_t total_toggles=0;
uint16_t time_period=0;
CameraDefinition camera;

static void toggle_on(void);
static void toggle_off(void);

void toggle_on(void) {
	run_ir_signal();
	if (sequence_pos <= total_toggles) {
		time_period = camera.trigger_sequence[sequence_pos];
		log_printf("\nReading num %d, The signal is on, scheduled the stop %d micro seconds later\n", sequence_pos, time_period);
		us_timer_start(US_TIMER2, US_SINGLE_CALL, time_period, toggle_off);
		sequence_pos++;
	}
}

void toggle_off(void) {
	stop_ir_signal();
	if (sequence_pos <= total_toggles) {
		time_period = camera.trigger_sequence[sequence_pos];
		log_printf("\nReading num %d, The signal is off, scheduled the start %d micro seconds later\n", sequence_pos, time_period);
		us_timer_start(US_TIMER2, US_SINGLE_CALL, time_period, toggle_on);
		sequence_pos++;
	}
}

void send_trigger(void) {
    // first member of camera_trigger sequence is number of toggles, and 2nd member is length of the first on and the 3rd is the length of the off, etc
    sequence_pos = 0;
    total_toggles = camera.trigger_sequence[sequence_pos++];
    toggle_on();
}

/**
 * @brief Function for application main entry.
 */
int main()
{

    /* Initial printf */
    uart_printf_init(UART_PRINTF_BAUD_9600);

    log_printf("\n************ Snap : Starting up **************\n");
    log_printf("\n************ Testing Cameras' IR Triggers  **************\n");


    ms_timer_init(APP_IRQ_PRIORITY_LOWEST);
    us_timer_init(APP_IRQ_PRIORITY_HIGHEST);
    profiler_timer_init();

    hfclk_xtal_init_blocking();
    lfclk_init(LFCLK_SRC_Xtal);

    /* if new camera, update camera.h and camera.c with your camera def and ir sequence */
    
    /* set ur camera type here */
    init_camera(&camera, NikonD3200);

    send_trigger();

    // Listen
    while (true)
    {
        __WFI();
    }

}

/** @} */
/** @} */
