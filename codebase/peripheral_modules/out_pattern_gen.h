/*
 *  out_pattern_gen.h
 *
 *  Created on: 21-May-2018
 *
 *  Copyright (c) 2018, Appiko
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
 * @addtogroup group_peripheral_modules
 * @{
 *
 * @defgroup group_out_pattern_gen Output pattern generator
 *
 * @brief Output pattern generator module is used for generating a one time digital
 *  signal pattern on a number of pins.
 *
 * @{
 */

#ifndef CODEBASE_PERIPHERAL_MODULES_OUT_PATTERN_GEN_H_
#define CODEBASE_PERIPHERAL_MODULES_OUT_PATTERN_GEN_H_

#include "stdint.h"
#include "stdbool.h"

/** The maximum number of transitions that can occur in the generated pattern */
#define OUT_GEN_MAX_TRANSITIONS 64

/** The maximum number of output pins for which pattern can be generated */
#define OUT_GEN_MAX_NUM_OUT     4

/**
 * @brief Initialize the output pattern generator module with the information of
 *  the pins on which the pattern is generated
 * @param num_out The number of pins with pattern generation
 * @param out_pins Pointer to array with the pin numbers
 * @note This function assumes the pins would already be initialized as output
 */
void out_gen_init(uint32_t num_out, uint32_t * out_pins);

/**
 * @brief Start the generation of the pattern with the information provided
 *  in this function on the pins initialized.
 * @param num_transitions The number of transitions in this pattern
 * @param transitions_durations Pointer to array containing the durations in
 *  terms of LFCLK frequency ticks for the transitions
 * @param next_out A pointer to a two dimensional array containing the next
 *  digital output value for the various transitions across the initialized pins.
 */
void out_gen_start(uint32_t num_transitions, uint32_t * transitions_durations,
        bool next_out[][OUT_GEN_MAX_TRANSITIONS]);

/**
 * @brief Stop the output pattern generation and sets the output pins
 *  as specified in the arguments
 * @param out_vals Pointer to an array of digital values for the pins to be
 *  set to on stopping the pattern generation.
 */
void out_gen_stop(bool * out_vals);

/**
 * @brief To know if the output pattern generator module is on
 * @return True if pattern generation is on and false if off
 */
bool out_gen_is_on(void);

#endif /* CODEBASE_PERIPHERAL_MODULES_OUT_PATTERN_GEN_H_ */

/**
 * @}
 * @}
 */
