/* ************************************************************ */
/* Test for DataFlash Log library                               */
/* ************************************************************ */
#pragma once
#include <stdint.h>
#include <AP_HAL/AP_HAL.h>

#define FINITE_STATE_MACHINE_NAME   "STAT"
#define FINITE_STATE_MACHINE_LABELS "TimeUS,id,stat"
#define FINITE_STATE_MACHINE_FMT    "QHB"

enum FiniteStateMachineID {
	MULTI_COPTER_MOTOR_SPOOL_MODE,
	ALT_HOLD_MODE_STATE,
    ID_NUM,
};

class DataFlash2
{
public:
    DataFlash2(){
		if (_instance != nullptr) {
			AP_HAL::panic("DataFlash2 must be singleton");
		}
		_instance = this;
	}

    // get singleton instance
    static DataFlash2 *instance(void) {
        return _instance;
    }

    void Log_Write_FSM(uint16_t id, uint8_t state);

private:
    static DataFlash2 *_instance;
};
