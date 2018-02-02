#include <DataFlash/DataFlash.h>
#include "DataFlash2.h"

DataFlash2 *DataFlash2::_instance;

extern const AP_HAL::HAL& hal;

void DataFlash2::Log_Write_FSM(uint16_t id, uint8_t state)
{
	DataFlash_Class::instance()->Log_Write(FINITE_STATE_MACHINE_NAME,
			                               FINITE_STATE_MACHINE_LABELS,
										   FINITE_STATE_MACHINE_FMT,
						                   AP_HAL::micros64(),
						                   id,
						                   state);
}

void DataFlash2::Log_Write_SCH(uint8_t i, const char *fun)
{
	char function[64] = {};
	strncpy(function, fun, sizeof(function));
	DataFlash_Class::instance()->Log_Write(SCHEDULER_TASK_NAME,
			                               SCHEDULER_TASK_LABELS,
			                               SCHEDULER_TASK_FMT,
						                   AP_HAL::micros64(),
						                   i,
										   function);
}
