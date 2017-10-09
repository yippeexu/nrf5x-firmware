#ifndef __APPIKO_SNAP_H_
#define __APPIKO_SNAP_H_
#include <stdint.h>

/* to get a 38kHz from a 16Mhz clock */
#define APPIKO_SNAP_FREQUENCY_FACTOR 210

#define IR_TIMER_PRESCALER 0

#define IR_GPIO_PIN_NUMER 31

#define IR_ON 1
#define IR_OFF 0
#define IR_TURN_ON 1
#define IR_TURN_OFF 0

// the Nikon trigger sequence
int16_t ir_sequence_length = 7;

uint16_t ir_nikon_sequence[] = {1, 2000, 0, 27830, 1, 400, 0, 1580, 1, 400, 0, 3580, 1, 400};

//uint16_t ir_nikon_sequence_toggle[] = {2000, 27830, 400, 1580, 400, 3580, 400};
uint16_t ir_nikon_sequence_toggle[] = {2000, 27830, 400, 1580, 400, 3580, 400};


/*
 * Design the structure to hold the per camera IR patterns
 * 1. A lookup data structure to hold the camera string to ID
 * Eg: { // "Camera Name", ID
 *      "NIKON 3250", 0,
 *      "CANON 110" 1
 *     }
 * 2. A camera ID to array for half-press, full-press sequence
 * Eg: { // ID, NumTogglePatterns, {half-press-seq}, NumTogglePatterns, {full-press-seq}
 *      0, 7, {2000, 27830, 400, 1580, 400, 3580, 400}, 5, {2000, 27830, 100, 1580, 100},
 *      1, 3, {1000, 7300, 1000}, 3, {1000, 5300, 1000}
 *     }
 * 3. Now everytime we receive the signal (for now we use a button press?), lookup the table and send
 * off the IR signal as per that pattern
 * 4. The Camera type will be configured at startup.
 * 5. what else?
 *
 */


/*
 * The config parameters
 * 1. triggered into config mode by button press.
 * 2. config includes
 * -- camera type (NIKON, CANON etc)
 * -- the mode (IR or cable)
 * -- the Seek radio protocols parameters (which are??)
 */


/*
 * The Radio protocol Seek
 * -- The listening side runs on Snap (does Sense ever need to get a signal from Snap?)
 * -- The transmitting side runs on Sense.
 * --
 */

#endif // __APPIKO_SNAP_H_
