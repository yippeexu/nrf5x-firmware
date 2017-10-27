#ifndef __IR_SIGNAL_H_
#define __IR_SIGNAL_H_

#include <stdint.h>
#include "boards.h"

/* to get a 38kHz from a 16Mhz clock */
#define APPIKO_SNAP_IR_FREQUENCY_FACTOR 210

#define APPIKO_SNAP_IR_TIMER_PRESCALER 0

#define IR_GPIO_PIN_NUMER IR_PIN

#define IR_OFF 0
#define IR_ON 1

bool IR_SIGNAL_STATE;

/*
 * Run a 38Khz IR signal
 */
void run_ir_signal(void);

void stop_ir_signal(void);

#endif // __IR_SIGNAL_H_
