/*
 *  log.h
 *
 *  Created on: 15-May-2017
 *
 *  Copyright (c) 2017, Appiko
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

#ifndef CODEBASE_PERIPHERAL_MODULES_LOG_H_
#define CODEBASE_PERIPHERAL_MODULES_LOG_H_

#if defined LOG_BMP_SWO
#define log_printf(...)
#elif defined LOG_SEGGER_RTT
#include "SEGGER_RTT.h"
#define log_init()
#define log_printf(...)  SEGGER_RTT_printf(0, __VA_ARGS__)
#elif defined LOG_UART_DMA_PRINTF//UARTE printf
#include "nrf.h"
#include "tinyprintf.h"
#include "uart_printf.h"
#define log_init()       uart_printf_init(UART_PRINTF_BAUD_1M)
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic push
#define log_printf(...)  tfp_printf(__VA_ARGS__)
#pragma GCC diagnostic pop
#elif defined LOG_UART_PRINTF//UARTE printf
#include "nrf.h"
#include "tinyprintf.h"
#include "hal_uart.h"
#define log_init()       hal_uart_init(HAL_UART_BAUD_1M, NULL)
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic push
#define log_printf(...)  tfp_printf(__VA_ARGS__)
#pragma GCC diagnostic pop
#else
#define log_init()
#define log_printf(...)
#endif

#endif /* CODEBASE_PERIPHERAL_MODULES_LOG_H_ */
