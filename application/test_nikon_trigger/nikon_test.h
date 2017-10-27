#ifndef __NIKON_TEST_H_
#define __NIKON_TEST_H_
#include <stdint.h>
#include "boards.h"

/*
 * Testing sequence : first member is number of subsequent members, 
 * followed by the sequence : Turn IR on for x seconds, then toggle after 
 * next x seconds, on and on 
 */

uint16_t nikon_trigger_sequence[] = {7, 2000, 27830, 400, 1580, 400, 3580, 400};

#endif // __NIKON_TEST_H_
