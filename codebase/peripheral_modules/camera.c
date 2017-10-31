#include <stdio.h>
#include "camera.h"

/* Camera trigger sequences
 * assuming a max of 10 toggles.
 * first member is number of subsequent members,
 * followed by the sequence : Turn IR on for x seconds, then toggle after
 * next x seconds, on and on
 * See enum CameraModels for position
 */

uint16_t camera_trigger_sequence[NUM_CAMERAS][10] = {
		{2, 100, 100}, /* Unknown camera */
		{7, 2000, 27830, 400, 1580, 400, 3580, 400}, /* NikonD3200 */
		{9, 100, 200, 300, 400, 500, 600, 700, 800, 900}, /* TestNewChap */
};


void init_camera(CameraDefinition *camera, CameraModel cam_model) {

	camera->camera_model = cam_model;
	camera->trigger_sequence = camera_trigger_sequence[cam_model];
}
