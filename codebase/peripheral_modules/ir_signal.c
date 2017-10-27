
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

/*
 * Run a 38Khz IR signal
 */
void run_ir_signal(void){

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

}

/*
 * Stop IR signal timer
 */
void stop_ir_signal(void){

	NRF_TIMER1->TASKS_STOP      = 1;
	IR_SIGNAL_STATE = IR_OFF;

}
