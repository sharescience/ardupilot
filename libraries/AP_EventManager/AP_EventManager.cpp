#include <AP_HAL/AP_HAL.h>
#include <AP_GPS/AP_GPS.h>
#include "AP_EventManager.h"

AP_EventManager *AP_EventManager::_instance;

extern const AP_HAL::HAL& hal;

const AP_Param::GroupInfo AP_EventManager::var_info[] = {
	// @Param: CURRENT
	// @DisplayName: current event id (Automatically detected, do not set manually)
	// @User: Standard
	AP_GROUPINFO("CURRENT"     ,  0, AP_EventManager, _current_id,            0),

	// @Param: FORN
	// @DisplayName: Respond first event or nearest event when priority equally.
	// @Values: 0:First,1:Nearest
	// @User: Standard
	AP_GROUPINFO("FORN"        ,  1, AP_EventManager, _first_or_nearest,      0),

	// @Param: CURR_P
	// @DisplayName: Current event's priority((Automatically detected, do not set manually))
	// @User: Standard
	AP_GROUPINFO("0_PRIORITY"  ,  2, AP_EventManager, _event[0].priority,     0),

	// @Param: CURR_M
	// @DisplayName: Current event's method (Automatically detected, do not set manually)
	// @Values: 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO,7:turn to ALT_HOLD
	// @User: Standard
	AP_GROUPINFO("CURR_M"      ,  3, AP_EventManager, _event[0].method_on, 0),

	// @Param: CURR_T
	// @DisplayName: Current event's trigger mode (Automatically detected, do not set manually)
	// @Values: 0:Only once,1:Always
	// @User: Standard
	AP_GROUPINFO("CURR_T"      ,  4, AP_EventManager, _event[0].trigger_mode, 0),

    // @Param: FS_RADIO_P
	// @DisplayName: Failsafe radio event priority("0-low, 255-high" or "0<127<-128<-1")
    // @User: Standard
    AP_GROUPINFO("FS_RADIO_P"  ,  5, AP_EventManager, _event[1].priority,     PRIORITY_DEFAULT),
    
    // @Param: FS_RADIO_M
	// @DisplayName: Failsafe radio event method.
	// @Values: 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO,7:turn to ALT_HOLD
    // @User: Standard
    AP_GROUPINFO("FS_RADIO_M"  ,  6, AP_EventManager, _event[1].method_on,    METHOD_ON_DEFAULT),

	// @Param: FS_RADIO_T
	// @DisplayName: Failsafe radio event trigger mode.
	// @Values: 0:Only once,1:Always
	// @User: Standard
	AP_GROUPINFO("FS_RADIO_T"  ,  7, AP_EventManager, _event[1].trigger_mode, TRIGGER_MODE_DEFAULT),

    // @Param: FS_BATT_P
	// @DisplayName: Failsafe battery event priority("0-low, 255-high" or "0<127<-128<-1")
    // @User: Standard
    AP_GROUPINFO("FS_BATT_P"   ,  8, AP_EventManager, _event[2].priority,     PRIORITY_DEFAULT),

    // @Param: FS_BATT_M
	// @DisplayName: Failsafe battery event method.
	// @Values: 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO,7:turn to ALT_HOLD
    // @User: Standard
    AP_GROUPINFO("FS_BATT_M"   ,  9, AP_EventManager, _event[2].method_on,    METHOD_ON_DEFAULT),

	// @Param: FS_BATT_T
	// @DisplayName: Failsafe battery event trigger mode.
	// @Values: 0:Only once,1:Always
	// @User: Standard
	AP_GROUPINFO("FS_BATT_T"   , 10, AP_EventManager, _event[2].trigger_mode, TRIGGER_MODE_DEFAULT),

    // @Param: FS_GCS_P
	// @DisplayName: Failsafe GCS event priority("0-low, 255-high" or "0<127<-128<-1")
    // @User: Standard
    AP_GROUPINFO("FS_GCS_P"    , 11, AP_EventManager, _event[3].priority,     PRIORITY_DEFAULT),

    // @Param: FS_GCS_M
	// @DisplayName: Failsafe GCS event method.
	// @Values: 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO,7:turn to ALT_HOLD
    // @User: Standard
    AP_GROUPINFO("FS_GCS_M"    , 12, AP_EventManager, _event[3].method_on,    METHOD_ON_DEFAULT),

	// @Param: FS_GCS_T
	// @DisplayName: Failsafe GCS event trigger mode.
	// @Values: 0:Only once,1:Always
	// @User: Standard
	AP_GROUPINFO("FS_GCS_T"    , 13, AP_EventManager, _event[3].trigger_mode, TRIGGER_MODE_DEFAULT),

    // @Param: FS_EKF_P
	// @DisplayName: Failsafe EKF event priority("0-low, 255-high" or "0<127<-128<-1")
    // @User: Standard
    AP_GROUPINFO("FS_EKF_P"    , 14, AP_EventManager, _event[4].priority,     PRIORITY_DEFAULT),

    // @Param: FS_EKF_M
	// @DisplayName: Failsafe EKF event method.
	// @Values: 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO,7:turn to ALT_HOLD
    // @User: Standard
    AP_GROUPINFO("FS_EKF_M"    , 15, AP_EventManager, _event[4].method_on,    METHOD_ON_DEFAULT),

	// @Param: FS_EKF_T
	// @DisplayName: Failsafe EKF event trigger mode.
	// @Values: 0:Only once,1:Always
	// @User: Standard
	AP_GROUPINFO("FS_EKF_T"    , 16, AP_EventManager, _event[4].trigger_mode, TRIGGER_MODE_DEFAULT),

    // @Param: FS_TER_P
	// @DisplayName: Failsafe terrain event priority("0-low, 255-high" or "0<127<-128<-1")
    // @User: Standard
    AP_GROUPINFO("FS_TER_P"    , 17, AP_EventManager, _event[5].priority,     PRIORITY_DEFAULT),

    // @Param: FS_TER_M
	// @DisplayName: Failsafe terrain event method.
	// @Values: 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO,7:turn to ALT_HOLD
    // @User: Standard
    AP_GROUPINFO("FS_TER_M"    , 18, AP_EventManager, _event[5].method_on,    METHOD_ON_DEFAULT),

	// @Param: FS_TER_T
	// @DisplayName: Failsafe terrain event trigger mode.
	// @Values: 0:Only once,1:Always
	// @User: Standard
	AP_GROUPINFO("FS_TER_T"    , 19, AP_EventManager, _event[5].trigger_mode, TRIGGER_MODE_DEFAULT),

    // @Param: FS_ADSB_P
	// @DisplayName: Failsafe ADSB event priority("0-low, 255-high" or "0<127<-128<-1")
    // @User: Standard
    AP_GROUPINFO("FS_ADSB_P"   , 20, AP_EventManager, _event[6].priority,     PRIORITY_DEFAULT),

    // @Param: FS_ADSB_M
	// @DisplayName: Failsafe ADSB event method (Must disable, because it's decided by mavlink)
	// @Values: 0:Disable,1:always LOITER,2:always RTL,3:always LAND,4:LOITER except AUTO,5:RTL except AUTO,6:LAND except AUTO,7:turn to ALT_HOLD
    // @User: Standard
    AP_GROUPINFO("FS_ADSB_M"  , 21, AP_EventManager, _event[6].method_on,    METHOD_ON_DEFAULT),

	// @Param: FS_ADSB_T
	// @DisplayName: Failsafe ADSB event trigger mode.
	// @Values: 0:Only once,1:Always
	// @User: Standard
	AP_GROUPINFO("FS_ADSB_T"   , 22, AP_EventManager, _event[6].trigger_mode, TRIGGER_MODE_DEFAULT),

    AP_GROUPEND
};

// Constructor
AP_EventManager::AP_EventManager(event_response_fn_t  event_on_response,
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

uint64_t AP_EventManager::tnow()
{
	return (_gps->time_epoch_usec() == 0) ? AP_HAL::micros64() : _gps->time_epoch_usec();
}

bool AP_EventManager::is_valid_event(int16_t id)
{
	if(id<1 || id>256){
		return false;
	}

	return true;
}

void AP_EventManager::event_update(int16_t id, bool state)
{
	if(is_valid_event(id)){
		if(state){
			if(_bits.check_bit(id) == 0){  // if the bit of this id is empty

				_bits.occupy_bit(id);
				_event[id].state     = state;
				_event[id].timestamp = tnow();

				// update current id by priority
				if((uint8_t)_event[id].priority > (uint8_t)_event[0].priority){
					_current_id = id;
					_event[0]   = _event[id];
					_event_on_respond();
				}else if((uint8_t)_event[id].priority == (uint8_t)_event[0].priority){
					if(_event[id].timestamp > _event[0].timestamp){
						_current_id = id;
						_event[0]   = _event[id];
						_event_on_respond();
					}
				}

				// write event on log
				_write_event_on_log(id);
			}
		}else{
			if(_bits.check_bit(id) == 3){  // if the bit of this id is occupied

				_bits.clear_bit(id);
				_event[id].state     = state;
				_event[id].timestamp = 0;

				find_should_be_responded_id(id);

				if(id == _current_id){
					memset(&_event[0], 0, sizeof(_event[0]));
				}

				if(!_event[0].state){
					if(is_valid_event(_should_be_responded_id)){
						_current_id = _should_be_responded_id;
						_event[0]   = _event[_should_be_responded_id];
						_event_on_respond();
						_should_be_responded_id = 0;
					}else{
						_current_id = 0;
					}
				}

				// write event off log
				_write_event_off_log(id);
			}
		}
	}
}

void AP_EventManager::find_should_be_responded_id(int16_t id)
{

	Event_Properties temp_event;
	uint8_t count_bits = _bits.count_occupied_bits();
	memset(&temp_event, 0, sizeof(temp_event));

	// get _should_be_responded_id
	for (uint8_t i=0; i<count_bits; i++){
		if(_bits.get_id(i) != id){
			if( _event[_bits.get_id(i)].priority.get() > temp_event.priority.get()){
				_should_be_responded_id = _bits.get_id(i);
				temp_event              = _event[_bits.get_id(i)];
			}

			if(_first_or_nearest == 1){
				if(   _event[_bits.get_id(i)].priority.get() == temp_event.priority.get()
				   && _event[_bits.get_id(i)].timestamp      >  temp_event.timestamp)
				{
					_should_be_responded_id = _bits.get_id(i);
					temp_event              = _event[_bits.get_id(i)];
				}
			}
		}
	}
}

//void AP_EventManager::send_mavlink_event_status(mavlink_channel_t chan)
//{
//	mavlink_msg_event_status_send(
//	chan,
//	_bits[0],
//	_bits[1],
//	_bits[2],
//	_bits[3],
//	_bits[4],
//	_bits[5],
//	_bits[6],
//	_bits[7]);
//}
