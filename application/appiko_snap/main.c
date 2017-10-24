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
 * @defgroup create a 38khz ir signal
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
#include "appiko_snap.h"

static bool IR_SIGNAL_STATE = IR_OFF;
uint16_t time_period=0;
int16_t pos=0;
static void stop_ir_signal(void);
static void run_ir_signal(void);

#if 0
/** @brief Configure the RGB LED pins as output and turn off LED */
static void rgb_led_init(void)
{
    hal_gpio_cfg_output(LED_RED, !(LEDS_ACTIVE_STATE));
    hal_gpio_cfg_output(LED_GREEN, !(LEDS_ACTIVE_STATE));
    hal_gpio_cfg_output(LED_BLUE, !(LEDS_ACTIVE_STATE));
}

/** @brief Configure the RGB LED pins as output and turn off LED */
static void rgb_led_cycle(void)
{
    hal_gpio_pin_write(LED_RED, (LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_GREEN, !(LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_BLUE, !(LEDS_ACTIVE_STATE));
    hal_nop_delay_ms(250);
    hal_gpio_pin_write(LED_RED, !(LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_GREEN, (LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_BLUE, !(LEDS_ACTIVE_STATE));
    hal_nop_delay_ms(250);
    hal_gpio_pin_write(LED_RED, !(LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_GREEN, !(LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_BLUE, (LEDS_ACTIVE_STATE));
    hal_nop_delay_ms(250);
    hal_gpio_pin_write(LED_RED, !(LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_GREEN, !(LEDS_ACTIVE_STATE));
    hal_gpio_pin_write(LED_BLUE, !(LEDS_ACTIVE_STATE));
}
/** @brief Turn on Green LED : using here as indication IR signal is on */
static void led_green_on(void)
{
	hal_gpio_pin_write(LED_GREEN, (LEDS_ACTIVE_STATE));
}
/** @brief Turn off Green LED : using here as indication IR signal is off */
static void led_green_off(void)
{
	hal_gpio_pin_write(LED_GREEN, !(LEDS_ACTIVE_STATE));
}

#endif
/*
 * Run a 38Khz IR signal
 */
static void run_ir_signal(void){

	/*
	creating a 38khz signal
	-use timer1 (timer0 is used by the profile timer, timer3 by the microsecond timer, and RTC by the millisecond timer
	-timer1 uses the HFCLK, so its freq is 16Mhz, use prescalar of 0 to use it s 16Mhz
	-to generate a signal of 38khz, we need to toggle every (500/38)milliseconds : so we get an on/off waveform every 1000/38 millsecs.
	- on the 16Mhz clock, thats every 210.52 ticks (500/38)/(1/16)
	- so set the timer as a counter, set the cc[0] to 210, and clear the counter everytime the compare event fires by setting the short of compare0_clear to 1
	*/

    NRF_TIMER1->TASKS_STOP      = 1;
    NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
    NRF_TIMER1->PRESCALER = APPIKO_SNAP_IR_TIMER_PRESCALER;
    NRF_TIMER1->BITMODE =  TIMER_BITMODE_BITMODE_08Bit;
    NRF_TIMER1->CC[0] = APPIKO_SNAP_IR_FREQUENCY_FACTOR ;
    NRF_TIMER1->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
    NRF_TIMER1->TASKS_CLEAR = 1;
    NRF_TIMER1->TASKS_START = 1;


    /*
     *Now choose an output pin for sending this signal on : eventually send directly to the IR pin
     *see the datasheet.
     * GPIOTE->CONFIG[] set MODE to 3 (TASK mode to act as output)
     * set PSEL to the GPIO PIN number
     * set POLARITY to toggle (3)
     * set OUTINT (initial val) to low (0)
     *
     */
    NRF_GPIOTE->CONFIG[0] = ((GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos) |
    		(IR_GPIO_PIN_NUMER << GPIOTE_CONFIG_PSEL_Pos) |
			(GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) |
			(GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos));


     /*
      * now set up the PPI to trigger the gpiote everytime timer event happens
      */
    NRF_PPI->CH[0].EEP = (uint32_t) &(NRF_TIMER1->EVENTS_COMPARE[0]);
    NRF_PPI->CH[0].TEP =  (uint32_t) &(NRF_GPIOTE->TASKS_OUT[0]);
    NRF_PPI->CHENSET = (PPI_CHENSET_CH0_Set << PPI_CHENSET_CH0_Pos);

    IR_SIGNAL_STATE = IR_ON;
    //PROFILE_STOP
    PROFILE_START;
    //schedule the stop
    if (pos < ir_sequence_length) {
    	time_period = ir_nikon_sequence_toggle[pos];
    	//log_printf("\nReading cfg %d, The signal is running, scheduled the stop %d micro seconds later\n", pos, time_period);
    	pos++;
    	us_timer_start(US_TIMER2, US_SINGLE_CALL, time_period, stop_ir_signal);
    } else {
    	log_printf("\nReached end of sequence, leaving the signal on");
    }

}


/*
 * Stop IR signal timer
 */
static void stop_ir_signal(void){

	NRF_TIMER1->TASKS_STOP      = 1;
	IR_SIGNAL_STATE = IR_OFF;
	//PROFILE_START;
	PROFILE_STOP;
	//schedule the next start
	if (pos < ir_sequence_length) {
		time_period = ir_nikon_sequence_toggle[pos];
		//log_printf("\nReading cfg %d, The signal is off, scheduled the start %d micro seconds later\n", pos, time_period);
		pos++;
		us_timer_start(US_TIMER2, US_SINGLE_CALL, time_period, run_ir_signal);
	} else {
    	log_printf("\nReached end of sequence, leaving the signal off");
    }

}

/**
 * @brief Function for application main entry.
 */
int main()
{
    //rgb_led_init();
    //rgb_led_cycle();
    /* Initial printf */
    uart_printf_init(UART_PRINTF_BAUD_9600);

    log_printf("\n************ Snap : Starting up **************\n");


    ms_timer_init(APP_IRQ_PRIORITY_LOWEST);
    us_timer_init(APP_IRQ_PRIORITY_HIGHEST);
    profiler_timer_init();

    hfclk_xtal_init_blocking();
    lfclk_init(LFCLK_SRC_Xtal);

    //PROFILE_START;
    run_ir_signal();

    // check the IR LED
    //hal_gpio_cfg_output(IR_PIN, IR_ACTIVE_STATE);

    // Listen
    while (true)
    {
        __WFI();
    }

}

/** @} */
/** @} */
