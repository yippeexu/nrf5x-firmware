/* 
 * File:   sensepi_pir.c
 * Copyright (c) 2018 Appiko
 * Created on 1 June, 2018, 3:30 PM
 * Author: Tejas Vasekar (https://github.com/tejas-tj)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE
 */


#include "sensepi_pir.h"

#include "data_process.h"
#include "sensepi_ble.h"

#include "pir_sense.h"
#include "led_sense.h"
#include "log.h"
#include "device_tick.h"
#include "mcp4012_x.h"

#include "boards.h"

#include "hal_pin_analog_input.h"
#include "hal_nop_delay.h"

#include "nrf_util.h"

#include "stdint.h"
#include "stdbool.h"

#define DAY_LIGHT_THRESHOLD 15
#define NIGHT_LIGHT_THRESHOLD 2000
#define LED_WAIT_TIME_MS 301
#define PIR_SENSE_INTERVAL_MS 50
#define PIR_SENSE_THRESHOLD 600
#define PIR_THRESHOLD_MULTIPLY_FACTOR 3

static sensepi_pir_config_t *config;
static bool led_sense_flag = 0;
static bool pir_sense_flag = 1;
static pir_sense_cfg config_pir;
static bool inter_trig_time_flag = 1;
static uint32_t intr_trig_time_in = 0;
static bool led_sense_init_done_flag = 1;//CHANGE THIS TO ) WHHILE IMPLEMENTING NON_BLOCKING
static uint32_t led_wait_time_count = 0; 
static uint32_t intr_trig_time_count = 0;

static sensepi_pir_substate pir_substate = PIR_ENABLED;


/**IRQ Handler for PIR interrupt*/
void pir_handler(int32_t adc_val)
{
    log_printf("Sensed %d\n", adc_val);
    device_tick_switch_mode(DEVICE_TICK_FAST);
    data_process_pattern_gen();
    if(pir_sense_flag == 1)
    {
        sensepi_pir_stop();
    }
}
#if 0
static pir_sense_cfg default_pir_config = 
{
    .sense_interval_ms = PIR_SENSE_INTERVAL_MS,
    .pir_signal_analog_in = PIN_TO_ANALOG_INPUT(PIR_AMP_SIGNAL_PIN),
    .pir_offset_analog_in = PIN_TO_ANALOG_INPUT(PIR_AMP_OFFSET_PIN),
    .threshold = PIR_SENSE_THRESHOLD,
    .irq_priority = APP_IRQ_PRIORITY_HIGH,
    .handler = pir_handler
};
#endif

void sensepi_pir_led_sense_conf(sensepi_pir_config_t * led_conf)
{
    led_sense_flag = 0;
    led_sense_init(led_conf->led_sense_out_pin,
            led_conf->led_sense_analog_in_pin,
            led_conf->led_sense_off_val);
    led_sense_cfg_input(1);
    led_wait_time_count = 0;
    led_sense_init_done_flag = 1; //CHANGE THIS TO 0 WHHILE IMPLEMENTING NON_BLOCKING
    hal_nop_delay_ms(301);
}

void sensepi_pir_init(sensepi_pir_config_t * config_sensepi_pir)
{
    config = config_sensepi_pir;
    sensepi_config * local_sensepi_config;
    local_sensepi_config = config->config_sensepi;
    pir_sense_cfg local_config_pir = 
    {
        PIR_SENSE_INTERVAL_MS, config->pir_sense_signal_input,
        config->pir_sense_offset_input,
        ((uint32_t)local_sensepi_config->threshold)*PIR_THRESHOLD_MULTIPLY_FACTOR,
        APP_IRQ_PRIORITY_HIGH, pir_handler, 
    };
    config_pir = local_config_pir;
    intr_trig_time_in = local_sensepi_config->inter_trig_time;
    sensepi_pir_led_sense_conf(config);
    mcp4012_init(MCP4012T_CS_PIN, MCP4012T_UD_PIN, SPI_SCK_PIN);
    mcp4012_set_value(config_sensepi_pir->config_sensepi->amplification);
    return;
}

void sensepi_pir_update(sensepi_config * update_config)
{
    config_pir.threshold = ((uint32_t) update_config->threshold)*PIR_THRESHOLD_MULTIPLY_FACTOR;
    mcp4012_set_value(update_config->amplification);
    intr_trig_time_in = (uint32_t)update_config->inter_trig_time;
    data_process_local_config_copy(update_config);
    sensepi_pir_led_sense_conf(config);
}

void sensepi_pir_start()
{
    log_printf("Sense pir : SENSE_PIR_ENABLED\n");
    pir_sense_start(&config_pir);
    pir_sense_flag = 1;
    device_tick_switch_mode(DEVICE_TICK_SLOW);
    pir_substate = PIR_DETECTING;
    return;
}

void sensepi_pir_stop()
{
    pir_sense_stop();
    pir_substate = PIR_DISABLED;
    inter_trig_time_flag = 0;
    intr_trig_time_count = 0;
    pir_sense_flag = 0;
}

void sensepi_pir_add_tick(uint32_t interval)
{
    switch(pir_substate)
    {
        case PIR_DETECTING:
        {
            break;
        }

        case PIR_ENABLED :
        {
            sensepi_pir_start();
            break;
        }
        case PIR_DISABLED :
        {
            intr_trig_time_count += interval;
            if(intr_trig_time_count >= intr_trig_time_in)
            {
                inter_trig_time_flag = 1;
            }
            if((config->config_sensepi->oper_time == DAY_ONLY &&
                    (led_sense_get()) > DAY_LIGHT_THRESHOLD) || 
                (config->config_sensepi->oper_time == NIGHT_ONLY &&
                    (led_sense_get()) < NIGHT_LIGHT_THRESHOLD) || 
                (config->config_sensepi->oper_time == DAY_AND_NIGHT))
            {
                led_sense_flag = 1;
            }
            else
            {
                led_sense_flag = 0;
            }
            if(led_sense_flag == 0)
            {
                device_tick_switch_mode(DEVICE_TICK_SLOW);
            }
            if(inter_trig_time_flag == 1 && led_sense_flag == 1)
            {
                pir_substate = PIR_ENABLED;
            }
            break;
        }
        case PIR_WAIT_LED :
        {
            led_wait_time_count += interval;
            if(led_wait_time_count >= LED_WAIT_TIME_MS)
            {
                led_sense_init_done_flag = 1;
                sensepi_pir_start();
            }
        }
    }
}