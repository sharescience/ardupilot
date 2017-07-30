#pragma once

#include <AP_HAL/AP_HAL_Boards.h>

// event id
enum event_id {
	EM_FAILSAFE_RADIO           =  1,
	EM_FAILSAFE_BATTERY         =  2,
	EM_FAILSAFE_GCS             =  3,
	EM_FAILSAFE_EKF             =  4,
	EM_FAILSAFE_TERRAIN         =  5,
	EM_FAILSAFE_ADSB            =  6,
};

// event state
enum event_state {
	EM_EVENT_OFF,
	EM_EVENT_ON,
};

// event on strategy
enum event_on_strategy{
	EM_EVENT_ON_DISABLE             =  0,
	EM_EVENT_ON_ALWAYS_LOITER       =  1,
	EM_EVENT_ON_ALWAYS_RTL          =  2,
	EM_EVENT_ON_ALWAYS_LAND         =  3,
	EM_EVENT_ON_LOITER_EXCEPT_AUTO  =  4,
	EM_EVENT_ON_RTL_EXCEPT_AUTO     =  5,
	EM_EVENT_ON_LAND_EXCEPT_AUTO    =  6,
	EM_EVENT_ON_TURN_ALT_HOLD       =  7,
};

// event off strategy
enum event_off_strategy{
	EM_EVENT_OFF_DISABLE,
};
