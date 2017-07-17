#include "Copter.h"
#if USE_EVENT_MANAGER == ENABLED
/*
 *       This event will be called when the failsafe changes
 *       boolean failsafe reflects the current state
 */
void Copter::event_response_update(void)
{
	if(event_manager.get_current_trigger_mode() != 0){
		event_on_response();
	}
}

void Copter::event_on_response()
{

	switch(event_manager.get_current_method_on()){
		case EM_EVENT_ON_DISABLE: {
#if AP_EVENTMANAGER_CONSOLE_DEBUG
			hal.console->printf("\n\n---response---\n");
			hal.console->printf("current id    : %d\n", event_manager.get_current_id());
			hal.console->printf("trigger mode  : %d\n\n"  , event_manager.get_current_trigger_mode());
#endif
			break;
		}
		case EM_EVENT_ON_ALWAYS_LOITER: {
		    if( !motors->armed() ) {
		        return;
		    }
		    if (should_disarm_on_failsafe()) {
		        init_disarm_motors();
		    } else {
		    	if(!set_mode(LOITER, get_event_set_mode_reason())){
		    		set_mode_land_with_pause(get_event_set_mode_reason());
		    	}
		    }
			break;
		}
		case EM_EVENT_ON_ALWAYS_RTL: {
		    if( !motors->armed() ) {
		        return;
		    }
		    if (should_disarm_on_failsafe()) {
		        init_disarm_motors();
		    } else {
		    	if(!set_mode(RTL, get_event_set_mode_reason())){
		    		set_mode_land_with_pause(get_event_set_mode_reason());
		    	}
		    }
			break;
		}
		case EM_EVENT_ON_ALWAYS_LAND: {
		    if( !motors->armed() ) {
		        return;
		    }
		    if (should_disarm_on_failsafe()) {
		        init_disarm_motors();
		    } else {
		    	set_mode_land_with_pause(get_event_set_mode_reason());
		    }
			break;
		}
		case EM_EVENT_ON_LOITER_EXCEPT_AUTO: {
		    if( !motors->armed() ) {
		        return;
		    }
		    if (should_disarm_on_failsafe()) {
		        init_disarm_motors();
		    } else if (control_mode != AUTO){
		    	if(!set_mode(LOITER, get_event_set_mode_reason())){
		    		set_mode_land_with_pause(get_event_set_mode_reason());
		    	}
		    }
			break;
		}
		case EM_EVENT_ON_RTL_EXCEPT_AUTO: {
		    if( !motors->armed() ) {
		        return;
		    }
		    if (should_disarm_on_failsafe()) {
		        init_disarm_motors();
		    } else if (control_mode != AUTO){
		    	if(!set_mode(RTL, get_event_set_mode_reason())){
		    		set_mode_land_with_pause(get_event_set_mode_reason());
		    	}
		    }
			break;
		}
		case EM_EVENT_ON_LAND_EXCEPT_AUTO: {
		    if( !motors->armed() ) {
		        return;
		    }
		    if (should_disarm_on_failsafe()) {
		        init_disarm_motors();
		    } else if (control_mode != AUTO){
		    	set_mode_land_with_pause(get_event_set_mode_reason());
		    }
			break;
		}
		case EM_EVENT_ON_TURN_ALT_HOLD: {
		    if( !motors->armed() ) {
		        return;
		    }
		    if (should_disarm_on_failsafe()) {
		        init_disarm_motors();
		    } else {
		    	if(!set_mode(ALT_HOLD, get_event_set_mode_reason())){
		    		set_mode_land_with_pause(get_event_set_mode_reason());
		    	}
		    }
			break;
		}
		default: {
			break;
		}
	}

    // if flight mode is already LAND ensure it's not the GPS controlled LAND
    if (control_mode == LAND && !position_ok()) {
        land_do_not_use_GPS();
    }
}

mode_reason_t Copter::get_event_set_mode_reason()
{
	mode_reason_t reason;

	switch(event_manager.get_current_id()){
	case EM_FAILSAFE_RADIO:
		reason = MODE_REASON_RADIO_FAILSAFE;
		break;
	case EM_FAILSAFE_BATTERY:
		reason = MODE_REASON_BATTERY_FAILSAFE;
		break;
	case EM_FAILSAFE_GCS:
		reason = MODE_REASON_GCS_FAILSAFE;
		break;
	case EM_FAILSAFE_EKF:
		reason = MODE_REASON_EKF_FAILSAFE;
		break;
	case EM_FAILSAFE_TERRAIN:
		reason = MODE_REASON_TERRAIN_FAILSAFE;
		break;
	case EM_FAILSAFE_ADSB:
		reason = MODE_REASON_AVOIDANCE;
		break;
	default:
		reason = MODE_REASON_UNKNOWN;
		break;
	}

	return reason;
}

void Copter::write_event_on_log(const uint8_t& id)
{
	switch(id){
	case EM_FAILSAFE_RADIO:
	    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_RADIO, ERROR_CODE_FAILSAFE_OCCURRED);
		break;
	case EM_FAILSAFE_BATTERY:
	    gcs().send_text(MAV_SEVERITY_WARNING,"Low battery");
	    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_BATT, ERROR_CODE_FAILSAFE_OCCURRED);
	    break;
	case EM_FAILSAFE_GCS:     // failsafe_gcs_check() done
		break;
	case EM_FAILSAFE_EKF:     // failsafe_ekf_event() done
		break;
	case EM_FAILSAFE_TERRAIN: // failsafe_terrain_on_event() done
		break;
	case EM_FAILSAFE_ADSB:    // AP_Avoidance_Copter::handle_avoidance() done
		break;
	default:
		break;
	}

}

void Copter::write_event_off_log(const uint8_t& id)
{
	switch(id){
	case EM_FAILSAFE_RADIO:
	    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_RADIO, ERROR_CODE_FAILSAFE_RESOLVED);
		break;
	case EM_FAILSAFE_BATTERY:
		break;
	case EM_FAILSAFE_GCS:     // failsafe_gcs_check() done
		break;
	case EM_FAILSAFE_EKF:     // failsafe_ekf_off_event() done
		break;
	case EM_FAILSAFE_TERRAIN: // failsafe_terrain_check() done
		break;
	case EM_FAILSAFE_ADSB:    // AP_Avoidance_Copter::handle_recovery() done
		break;
	default:
		break;
	}
}

void Copter::failsafe_radio_on_event()
{
#if 0
    // if motors are not armed there is nothing to do
    if( !motors->armed() ) {
        return;
    }

    if (should_disarm_on_failsafe()) {
        init_disarm_motors();
    } else {
        if (control_mode == AUTO && g.failsafe_throttle == FS_THR_ENABLED_CONTINUE_MISSION) {
            // continue mission
        } else if (control_mode == LAND && g.failsafe_battery_enabled == FS_BATT_LAND && failsafe.battery) {
            // continue landing
        } else {
            if (g.failsafe_throttle == FS_THR_ENABLED_ALWAYS_LAND) {
                set_mode_land_with_pause(MODE_REASON_RADIO_FAILSAFE);
            } else {
                set_mode_RTL_or_land_with_pause(MODE_REASON_RADIO_FAILSAFE);
            }
        }
    }

    // log the error to the dataflash
    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_RADIO, ERROR_CODE_FAILSAFE_OCCURRED);
#endif
}

// failsafe_off_event - respond to radio contact being regained
// we must be in AUTO, LAND or RTL modes
// or Stabilize or ACRO mode but with motors disarmed
void Copter::failsafe_radio_off_event()
{
#if 0
    // no need to do anything except log the error as resolved
    // user can now override roll, pitch, yaw and throttle and even use flight mode switch to restore previous flight mode
    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_RADIO, ERROR_CODE_FAILSAFE_RESOLVED);
#endif
}

void Copter::failsafe_battery_event(void)
{
#if 0
    // return immediately if low battery event has already been triggered
    if (failsafe.battery) {
        return;
    }

    // failsafe check
    if (g.failsafe_battery_enabled != FS_BATT_DISABLED && motors->armed()) {
        if (should_disarm_on_failsafe()) {
            init_disarm_motors();
        } else {
            if (g.failsafe_battery_enabled == FS_BATT_RTL || control_mode == AUTO) {
                set_mode_RTL_or_land_with_pause(MODE_REASON_BATTERY_FAILSAFE);
            } else {
                set_mode_land_with_pause(MODE_REASON_BATTERY_FAILSAFE);
            }
        }
    }

    // set the low battery flag
    set_failsafe_battery(true);

    // warn the ground station and log to dataflash
    gcs().send_text(MAV_SEVERITY_WARNING,"Low battery");
    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_BATT, ERROR_CODE_FAILSAFE_OCCURRED);
#endif
}

// failsafe_gcs_check - check for ground station failsafe
void Copter::failsafe_gcs_check()
{
    uint32_t last_gcs_update_ms;

    // return immediately if gcs failsafe is disabled, gcs has never been connected or we are not overriding rc controls from the gcs and we are not in guided mode
    // this also checks to see if we have a GCS failsafe active, if we do, then must continue to process the logic for recovery from this state.
    if ((!failsafe.gcs)&&(g.failsafe_gcs == FS_GCS_DISABLED || failsafe.last_heartbeat_ms == 0 || (!failsafe.rc_override_active && control_mode != GUIDED))) {
        return;
    }

    // calc time since last gcs update
    // note: this only looks at the heartbeat from the device id set by g.sysid_my_gcs
    last_gcs_update_ms = millis() - failsafe.last_heartbeat_ms;

    // check if all is well
    if (last_gcs_update_ms < FS_GCS_TIMEOUT_MS) {
        // check for recovery from gcs failsafe
        if (failsafe.gcs) {
            failsafe_gcs_off_event();
            set_failsafe_gcs(false);
        }
        return;
    }

    // do nothing if gcs failsafe already triggered or motors disarmed
    if (failsafe.gcs || !motors->armed()) {
        return;
    }

    // GCS failsafe event has occurred
    // update state, log to dataflash
    set_failsafe_gcs(true);
    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_GCS, ERROR_CODE_FAILSAFE_OCCURRED);

    // clear overrides so that RC control can be regained with radio.
    if(!failsafe.radio) hal.rcin->clear_overrides();
    failsafe.rc_override_active = false;
#if 0
    if (should_disarm_on_failsafe()) {
        init_disarm_motors();
    } else {
        if (control_mode == AUTO && g.failsafe_gcs == FS_GCS_ENABLED_CONTINUE_MISSION) {
            // continue mission
        } else if (g.failsafe_gcs != FS_GCS_DISABLED) {
            set_mode_RTL_or_land_with_pause(MODE_REASON_GCS_FAILSAFE);
        }
    }
#endif
}

// failsafe_gcs_off_event - actions to take when GCS contact is restored
void Copter::failsafe_gcs_off_event(void)
{
    // log recovery of GCS in logs?
    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_GCS, ERROR_CODE_FAILSAFE_RESOLVED);
}

// executes terrain failsafe if data is missing for longer than a few seconds
//  missing_data should be set to true if the vehicle failed to navigate because of missing data, false if navigation is proceeding successfully
void Copter::failsafe_terrain_check()
{
    // trigger with 5 seconds of failures while in AUTO mode
    bool valid_mode = (control_mode == AUTO || control_mode == GUIDED || control_mode == GUIDED_NOGPS || control_mode == RTL);
    bool timeout = (failsafe.terrain_last_failure_ms - failsafe.terrain_first_failure_ms) > FS_TERRAIN_TIMEOUT_MS;
    bool trigger_event = valid_mode && timeout;

    // check for clearing of event
    if (trigger_event != failsafe.terrain) {
        if (trigger_event) {
            failsafe_terrain_on_event();
        } else {
            Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_TERRAIN, ERROR_CODE_ERROR_RESOLVED);
            failsafe.terrain = false;
            event_manager.event_update(EM_FAILSAFE_TERRAIN, EM_EVENT_OFF);
        }
    }
}

// set terrain data status (found or not found)
void Copter::failsafe_terrain_set_status(bool data_ok)
{
    uint32_t now = millis();

    // record time of first and latest failures (i.e. duration of failures)
    if (!data_ok) {
        failsafe.terrain_last_failure_ms = now;
        if (failsafe.terrain_first_failure_ms == 0) {
            failsafe.terrain_first_failure_ms = now;
        }
    } else {
        // failures cleared after 0.1 seconds of persistent successes
        if (now - failsafe.terrain_last_failure_ms > 100) {
            failsafe.terrain_last_failure_ms = 0;
            failsafe.terrain_first_failure_ms = 0;
        }
    }
}

// terrain failsafe action
void Copter::failsafe_terrain_on_event()
{
    failsafe.terrain = true;
    gcs().send_text(MAV_SEVERITY_CRITICAL,"Failsafe: Terrain data missing");
    Log_Write_Error(ERROR_SUBSYSTEM_FAILSAFE_TERRAIN, ERROR_CODE_FAILSAFE_OCCURRED);
    event_manager.event_update(EM_FAILSAFE_TERRAIN, EM_EVENT_ON);
#if 0
    if (should_disarm_on_failsafe()) {
        init_disarm_motors();
    } else if (control_mode == RTL) {
        rtl_restart_without_terrain();
    } else {
        set_mode_RTL_or_land_with_pause(MODE_REASON_TERRAIN_FAILSAFE);
    }
#endif
}

// set_mode_RTL_or_land_with_pause - sets mode to RTL if possible or LAND with 4 second delay before descent starts
//  this is always called from a failsafe so we trigger notification to pilot
void Copter::set_mode_RTL_or_land_with_pause(mode_reason_t reason)
{
    // attempt to switch to RTL, if this fails then switch to Land
    if (!set_mode(RTL, reason)) {
        // set mode to land will trigger mode change notification to pilot
        set_mode_land_with_pause(reason);
    } else {
        // alert pilot to mode change
        AP_Notify::events.failsafe_mode_change = 1;
    }
}

bool Copter::should_disarm_on_failsafe() {
    if (ap.in_arming_delay) {
        return true;
    }

    switch(control_mode) {
        case STABILIZE:
        case ACRO:
            // if throttle is zero OR vehicle is landed disarm motors
            return ap.throttle_zero || ap.land_complete;
        case AUTO:
            // if mission has not started AND vehicle is landed, disarm motors
            return !ap.auto_armed && ap.land_complete;
        default:
            // used for AltHold, Guided, Loiter, RTL, Circle, Drift, Sport, Flip, Autotune, PosHold
            // if landed disarm
            return ap.land_complete;
    }
}

void Copter::update_events()
{
    ServoRelayEvents.update_events();
}
#endif //#if USE_EVENT_MANAGER == ENABLED
