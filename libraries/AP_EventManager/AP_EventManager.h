#pragma once

#include <AP_Param/AP_Param.h>
#include <GCS_MAVLink/GCS_MAVLink.h>
#include <AP_Bitmask/AP_Bitmask.h>


#define AP_EVENTMANAGER_CONSOLE_DEBUG  0   // 0-Normal 1-Debug
#define MAX_EVENT_NUM                  6
#define PRIORITY_DEFAULT               0   // 0-low 255-high
#define TRIGGER_MODE_DEFAULT           0   // 0:Only once,1:Always
#define METHOD_ON_DEFAULT              0   // 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO


class AP_GPS;

class AP_EventManager {

public:

    // main program function pointers
    FUNCTOR_TYPEDEF(event_response_fn_t, void);
    FUNCTOR_TYPEDEF(write_event_log_fn_t, void, const uint8_t&);

    // Constructor
    AP_EventManager(event_response_fn_t  event_on_response,
    		        write_event_log_fn_t write_event_on_log,
			        write_event_log_fn_t write_event_off_log,
			        AP_GPS*              gps)
    		: _gps(gps)
    		, _event_on_respond(event_on_response)
    		, _write_event_on_log(write_event_on_log)
    		, _write_event_off_log(write_event_off_log)
    		, _should_be_responded_id(0)
    {
    	_instance = this;

    	// setup parameter defaults
        AP_Param::setup_object_defaults(this, var_info);
    }

    // get now timestamp
    uint64_t tnow();

    // check valid id
    bool is_valid_event(int16_t id);

    // event update when event on or event off
    void event_update(int16_t id, bool state);

    // find the id that should be responded when event off
    void find_should_be_responded_id(int16_t id);

    // return the method of current id when event on
    uint8_t get_current_method_on() const { return _event[0].method_on; }

    // return the trigger mode of current id
    uint8_t get_current_trigger_mode() const { return _event[0].trigger_mode; }

    // return the value of method_on
    uint8_t get_event_method_on(uint8_t i) const {return _event[i].method_on; }

    // return current id, range: 1~256
    int16_t get_current_id() const { return _current_id.get(); }

    // get singleton instance
    static AP_EventManager *instance(void) {
        return _instance;
    }

    // set method_on value
    void set_event_method_on(uint8_t i, int8_t value) { _event[i].method_on = value; }

    // load method_on value
    void load_event_method_on(uint8_t i) { _event[i].method_on.load(); }

    // parameter var table
    static const struct AP_Param::GroupInfo var_info[];

    // send seats by mavlink
//    void send_mavlink_event_status(mavlink_channel_t chan);

private:
    AP_GPS*      _gps;
    AP_Bitmask   _bits;

    // pointer to main program functions
    event_response_fn_t _event_on_respond;
    write_event_log_fn_t _write_event_on_log;
    write_event_log_fn_t _write_event_off_log;

    static AP_EventManager *_instance;

	// event properties
	struct Event_Properties{
    	bool     state;               // true:on, false:off
		AP_Int8  priority;            // 0-low, 255-high
		AP_Int8  method_on;           // user define
		AP_Int8  trigger_mode;        // 0:only once, 1:always
		uint64_t timestamp;
	} _event[MAX_EVENT_NUM+1];        // event[0] is current event

	AP_Int16 _current_id;             // for report and log, range: 1~256
	AP_Int8  _first_or_nearest;       // 0:first, 1:nearest
	int16_t  _should_be_responded_id;
};
