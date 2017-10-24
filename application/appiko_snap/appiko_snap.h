#ifndef __APPIKO_SNAP_H_
#define __APPIKO_SNAP_H_
#include <stdint.h>
#include "boards.h"

/* to get a 38kHz from a 16Mhz clock */
#define APPIKO_SNAP_IR_FREQUENCY_FACTOR 210

#define APPIKO_SNAP_IR_TIMER_PRESCALER 0

// all this ir stuff will change
#define IR_GPIO_PIN_NUMER IR_PIN

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
enum CameraModel {
	DEFAULT=0,
	NIKON101,
	CANON202,
};

/* TBD : struct here for IR patterns for each camera model */

enum TriggerMode {
	TRIGGER_IR=0,
	TRIGGER_CABLE,
	TRIGGER_BOTH
};

enum TimeSetting {
	TIME_DAY=0,
	TIME_NIGHT,
	TIME_DAY_AND_NIGHT
};

enum ClickMode {
	CLICK_SINGLE=0,
	CLICK_BURST
};

enum FocusSet {
	FOCUS_ON=0,
	FOCUS_OFF
};

/* all time here in ms */
typedef struct ExposureParams {
	uint16_t burst_interval;
	uint16_t bulb_exposure;
	uint16_t night_exposure;
} ExposureParams;

/* all time here in ms */
typedef struct VideoParams {
	uint16_t video_duration;
	uint16_t extension_time;
} VideoParams;

/* TBD : data structure for list of detectors.
 * Need to init with a global id (mac id?), a list of detectors that can be triggered
 */
typedef struct Detectors {

} Detectors;

/*
 * The config parameters
 * 1. triggered into config mode by button press.
 * 2. Config Params
 * 2a. Camera Model (camera_model = NIKON, CANON, etc) (Camera model attached to the Snap unit)
 * 2b. Trigger Mode (trigger_mode = IR, CABLE, IR_AND_CABLE) (If the IR trigger is also to be used along with the cable (default on)
 * 2c. Time setting (time_setting = DAY, NIGHT, DAY_AND_NIGHT) (Time of the day - Day only, Night only, Both day and night)
 * 2d. Click Mode (click_mode = SINGLE, BURST) (Mode Image trigger - single Image trigger - burst)
 * 2e. Exposure Params ( burst_interval, bulb_exposure, night_exposure all in ms) (Time between bursts in ms, Bulb exposure with its duration, Time for exposure at night in ms)
 * 2f. Video Params (video_duration, extension_time in sec) (Video - Duration of the video in s, Extension time in case more motion is detected)
 * 2g. Focus Set (focus_on = TRUE, FALSE) (Focus the camera (half press the shutter)
 * 2h. Detector Sensitivity (detector_threshold) (Sensitivity threshold for the detector)
 * 2i. Worst Case Latency (worst_case_latency : whats this) (Worst case latency in ms)
 * 2j. Time Between Triggers (time_bet_triggers in ms) (Time between triggers in ms)
 * 2k. Time to set focus (time_set_focus in ms) (Time to set the focus signal in ms before trigger)
 * 2l. (This shd be cameras ?) Detectors to Trigger ( detectors_to_trigger : struct with list) (Detector to trigger units mapping for the setting up multiple units)
 * the Seek radio protocols parameters (which are??)
 */


/*
 * The Radio protocol Seek
 * -- The listening side runs on Snap (does Sense ever need to get a signal from Snap?)
 * -- The transmitting side runs on Sense.
 * --
 */

#endif // __APPIKO_SNAP_H_
