#include "GCS.h"
#include "GCS_MAVLink.h"

extern const AP_HAL::HAL& hal;

void GCS_MAVLINK::handleMessage_sharescience(mavlink_message_t* msg)
{
    switch (msg->msgid) {

    case MAVLINK_MSG_ID_ELASTICITY:
    {
        mavlink_elasticity_t packet;
        mavlink_msg_elasticity_decode(msg, &packet);

        uint32_t data[8];

        memcpy(data, packet.data, MAVLINK_MSG_ID_ELASTICITY_LEN);
        mavlink_msg_elasticity_send(chan, data);

        break;
    }

    case MAVLINK_MSG_ID_STREAM_SPEED:
    {
    	mavlink_stream_speed_t packet;
    	mavlink_msg_stream_speed_decode(msg, &packet);

    	uint8_t datastreamSpeed[11];
    	datastreamSpeed[0] = (uint8_t)((packet.speed_1 & 0x0000000F)>>0);  // RAW_SENS
    	datastreamSpeed[1] = (uint8_t)((packet.speed_1 & 0x000000F0)>>4);  // EXT_STAT
    	datastreamSpeed[2] = (uint8_t)((packet.speed_1 & 0x00000F00)>>8);  // RC_CHAN
    	datastreamSpeed[3] = (uint8_t)((packet.speed_1 & 0x0000F000)>>12); // RAW_CTRL
    	datastreamSpeed[4] = (uint8_t)((packet.speed_1 & 0x000F0000)>>16); // POSITION
    	datastreamSpeed[5] = (uint8_t)((packet.speed_1 & 0x00F00000)>>20); // EXTRA1
    	datastreamSpeed[6] = (uint8_t)((packet.speed_1 & 0x0F000000)>>24); // EXTRA2
    	datastreamSpeed[7] = (uint8_t)((packet.speed_1 & 0xF0000000)>>28); // EXTRA3
    	datastreamSpeed[8] = (uint8_t)((packet.speed_2 & 0x0000000F)>>0);  // PARAMS
    	datastreamSpeed[9] = (uint8_t)((packet.speed_2 & 0x000000F0)>>4);  // ADSB
    	datastreamSpeed[10]= (uint8_t)((packet.speed_2 & 0x00000F00)>>8);  // PID

    	if(packet.bitmask & 1){
			AP_Param::set_and_save_by_name("SR0_RAW_SENS", datastreamSpeed[0]);
			AP_Param::set_and_save_by_name("SR0_EXT_STAT", datastreamSpeed[1]);
			AP_Param::set_and_save_by_name("SR0_RC_CHAN" , datastreamSpeed[2]);
			AP_Param::set_and_save_by_name("SR0_RAW_CTRL", datastreamSpeed[3]);
			AP_Param::set_and_save_by_name("SR0_POSITION", datastreamSpeed[4]);
			AP_Param::set_and_save_by_name("SR0_EXTRA1"  , datastreamSpeed[5]);
			AP_Param::set_and_save_by_name("SR0_EXTRA2"  , datastreamSpeed[6]);
			AP_Param::set_and_save_by_name("SR0_EXTRA3"  , datastreamSpeed[7]);
			AP_Param::set_and_save_by_name("SR0_PARAMS"  , datastreamSpeed[8]);
			AP_Param::set_and_save_by_name("SR0_ADSB"    , datastreamSpeed[9]);
			AP_Param::set_and_save_by_name("SR0_PID"     , datastreamSpeed[10]);
    	}

    	if(packet.bitmask & 2){
			AP_Param::set_and_save_by_name("SR1_RAW_SENS", datastreamSpeed[0]);
			AP_Param::set_and_save_by_name("SR1_EXT_STAT", datastreamSpeed[1]);
			AP_Param::set_and_save_by_name("SR1_RC_CHAN" , datastreamSpeed[2]);
			AP_Param::set_and_save_by_name("SR1_RAW_CTRL", datastreamSpeed[3]);
			AP_Param::set_and_save_by_name("SR1_POSITION", datastreamSpeed[4]);
			AP_Param::set_and_save_by_name("SR1_EXTRA1"  , datastreamSpeed[5]);
			AP_Param::set_and_save_by_name("SR1_EXTRA2"  , datastreamSpeed[6]);
			AP_Param::set_and_save_by_name("SR1_EXTRA3"  , datastreamSpeed[7]);
			AP_Param::set_and_save_by_name("SR1_PARAMS"  , datastreamSpeed[8]);
			AP_Param::set_and_save_by_name("SR1_ADSB"    , datastreamSpeed[9]);
			AP_Param::set_and_save_by_name("SR1_PID"     , datastreamSpeed[10]);
    	}

    	if(packet.bitmask & 4){
			AP_Param::set_and_save_by_name("SR2_RAW_SENS", datastreamSpeed[0]);
			AP_Param::set_and_save_by_name("SR2_EXT_STAT", datastreamSpeed[1]);
			AP_Param::set_and_save_by_name("SR2_RC_CHAN" , datastreamSpeed[2]);
			AP_Param::set_and_save_by_name("SR2_RAW_CTRL", datastreamSpeed[3]);
			AP_Param::set_and_save_by_name("SR2_POSITION", datastreamSpeed[4]);
			AP_Param::set_and_save_by_name("SR2_EXTRA1"  , datastreamSpeed[5]);
			AP_Param::set_and_save_by_name("SR2_EXTRA2"  , datastreamSpeed[6]);
			AP_Param::set_and_save_by_name("SR2_EXTRA3"  , datastreamSpeed[7]);
			AP_Param::set_and_save_by_name("SR2_PARAMS"  , datastreamSpeed[8]);
			AP_Param::set_and_save_by_name("SR2_ADSB"    , datastreamSpeed[9]);
			AP_Param::set_and_save_by_name("SR2_PID"     , datastreamSpeed[10]);
    	}

    	if(packet.bitmask & 8){
			AP_Param::set_and_save_by_name("SR3_RAW_SENS", datastreamSpeed[0]);
			AP_Param::set_and_save_by_name("SR3_EXT_STAT", datastreamSpeed[1]);
			AP_Param::set_and_save_by_name("SR3_RC_CHAN" , datastreamSpeed[2]);
			AP_Param::set_and_save_by_name("SR3_RAW_CTRL", datastreamSpeed[3]);
			AP_Param::set_and_save_by_name("SR3_POSITION", datastreamSpeed[4]);
			AP_Param::set_and_save_by_name("SR3_EXTRA1"  , datastreamSpeed[5]);
			AP_Param::set_and_save_by_name("SR3_EXTRA2"  , datastreamSpeed[6]);
			AP_Param::set_and_save_by_name("SR3_EXTRA3"  , datastreamSpeed[7]);
			AP_Param::set_and_save_by_name("SR3_PARAMS"  , datastreamSpeed[8]);
			AP_Param::set_and_save_by_name("SR3_ADSB"    , datastreamSpeed[9]);
			AP_Param::set_and_save_by_name("SR3_PID"     , datastreamSpeed[10]);
    	}

    	break;
    }

    default:
        break;
    }     // end switch
}
