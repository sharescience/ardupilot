#include "Copter.h"
#if USE_EVENT_MANAGER == ENABLED
// ---------------------------------------------
void Copter::set_auto_armed(bool b)
{
    // if no change, exit immediately
    if( ap.auto_armed == b )
        return;

    ap.auto_armed = b;
    if(b){
        Log_Write_Event(DATA_AUTO_ARMED);
    }
}

// ---------------------------------------------
/**
 * Set Simple mode
 *
 * @param [in] b 0:false or disabled, 1:true or SIMPLE, 2:SUPERSIMPLE
 */
void Copter::set_simple_mode(uint8_t b)
{
    if (ap.simple_mode != b) {
        switch (b) {
            case 0:
                Log_Write_Event(DATA_SET_SIMPLE_OFF);
                gcs().send_text(MAV_SEVERITY_INFO, "SIMPLE mode off");
                break;
            case 1:
                Log_Write_Event(DATA_SET_SIMPLE_ON);
                gcs().send_text(MAV_SEVERITY_INFO, "SIMPLE mode on");
                break;
            case 2:
            default:
                // initialise super simple heading
                update_super_simple_bearing(true);
                Log_Write_Event(DATA_SET_SUPERSIMPLE_ON);
                gcs().send_text(MAV_SEVERITY_INFO, "SUPERSIMPLE mode on");
                break;
        }
        ap.simple_mode = b;
    }
}

// ---------------------------------------------
void Copter::set_failsafe_radio(bool b)
{
    // only act on changes
    // -------------------
    if(failsafe.radio != b) {

        // store the value so we don't trip the gate twice
        // -----------------------------------------------
        failsafe.radio = b;

        if (failsafe.radio == false) {
            // We've regained radio contact
            // ----------------------------
        	event_manager.event_update(EM_FAILSAFE_RADIO, EM_EVENT_OFF);
        }else{
            // We've lost radio contact
            // ------------------------
        	event_manager.event_update(EM_FAILSAFE_RADIO, EM_EVENT_ON);
        }

        // update AP_Notify
        AP_Notify::flags.failsafe_radio = b;
    }
}


// ---------------------------------------------
void Copter::set_failsafe_battery(bool b)
{
	if(failsafe.battery != b){
        failsafe.battery = b;

        if(failsafe.battery == false){
        	event_manager.event_update(EM_FAILSAFE_BATTERY, EM_EVENT_OFF);
        }else{
        	event_manager.event_update(EM_FAILSAFE_BATTERY, EM_EVENT_ON);
        }

        AP_Notify::flags.failsafe_battery = b;
	}
}

// ---------------------------------------------
void Copter::set_failsafe_gcs(bool b)
{
	if(failsafe.gcs != b){
		failsafe.gcs = b;

        if(failsafe.gcs == false){
        	event_manager.event_update(EM_FAILSAFE_GCS, EM_EVENT_OFF);
        }else{
        	event_manager.event_update(EM_FAILSAFE_GCS, EM_EVENT_ON);
        }
	}
}

// ---------------------------------------------

void Copter::update_using_interlock()
{
#if FRAME_CONFIG == HELI_FRAME
    // helicopters are always using motor interlock
    ap.using_interlock = true;
#else
    // check if we are using motor interlock control on an aux switch or are in throw mode
    // which uses the interlock to stop motors while the copter is being thrown
    ap.using_interlock = check_if_auxsw_mode_used(AUXSW_MOTOR_INTERLOCK);
#endif
}

void Copter::set_motor_emergency_stop(bool b)
{
    if(ap.motor_emergency_stop != b) {
        ap.motor_emergency_stop = b;
    }

    // Log new status
    if (ap.motor_emergency_stop){
        Log_Write_Event(DATA_MOTORS_EMERGENCY_STOPPED);
    } else {
        Log_Write_Event(DATA_MOTORS_EMERGENCY_STOP_CLEARED);
    }
}
#endif //#if USE_EVENT_MANAGER == ENABLED

