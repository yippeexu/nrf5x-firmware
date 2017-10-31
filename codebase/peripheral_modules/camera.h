#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <stdint.h>
#include "boards.h"

typedef enum CameraModel {
	Unknown=0,
	NikonD3200,
	TestNewChap,
	NUM_CAMERAS /* this must be last */
} CameraModel;

typedef enum CameraOperation {
	FOCUS=0,
	TRIGGER,
	BURST,
	VIDEO_START,
	VIDEO_STOP,
	NUM_OPS /* this must be last */
} CameraOperation;

typedef struct CameraDefinition {
	CameraModel camera_model;
	uint16_t *trigger_sequence;
	//bool[NUM_OPS] camera_capabilities;
} CameraDefinition;


void init_camera(CameraDefinition *camera, CameraModel cam_model);

/*
 * Testing sequence : first member is number of subsequent members, 
 * followed by the sequence : Turn IR on for x seconds, then toggle after 
 * next x seconds, on and on 
 */

//uint16_t nikon_trigger_sequence[] = {7, 2000, 27830, 400, 1580, 400, 3580, 400};

#endif // __CAMERA_H_
