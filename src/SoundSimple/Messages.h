/**
 * \file
 * \author Ricky Curtice
 * \date 20120211
 * \brief Provides a list of the message IDs used and constant handles to reference them by.
 *
 * <p>
 * </p>
 */
#pragma once

namespace Sound {
	/* * * * * * * * * * * * * * * * * * * * * */
	// Messages
	/* * * * * * * * * * * * * * * * * * * * * */
	const unsigned int ACK_MODULE_LOADED = 4001; ///< Message sent from the module to indicate that the it has loaded
	
	const unsigned int MSG_MODULE_START = 4002; ///< Message sent to the module to start it up
	const unsigned int ACK_MODULE_START = 4003; ///< Message sent from the module with a status indicator bool that indicate whether the module sucessfully loaded
	
	const unsigned int MSG_LOAD_XACT_SETTINGS_FILE = 4008;
	const unsigned int ACK_LOAD_XACT_SETTINGS_FILE = 4009;
	
	const unsigned int MSG_LOAD_IN_MEMORY_WAVEBANK = 4010;
	const unsigned int ACK_LOAD_IN_MEMORY_WAVEBANK = 4011;
	
	const unsigned int MSG_LOAD_STREAMING_WAVEBANK = 4012;
	const unsigned int ACK_LOAD_STREAMING_WAVEBANK = 4013;
	
	const unsigned int MSG_LOAD_SOUNDBANK          = 4020;
	const unsigned int ACK_LOAD_SOUNDBANK          = 4021;
	
	const unsigned int MSG_GET_CUE_KEY             = 4100;
	const unsigned int ACK_GET_CUE_KEY             = 4101;
	
	const unsigned int MSG_PREPARE_CUE             = 4110;
	const unsigned int ACK_PREPARE_CUE             = 4111;
	
	const unsigned int MSG_PREPARE_3D_CUE          = 4112;
	const unsigned int ACK_PREPARE_3D_CUE          = 4113;
	
	const unsigned int MSG_PLAY_CUE                = 4120;
	//const unsigned int ACK_PLAY_CUE                = 4121;
	
	const unsigned int MSG_PLAY_3D_CUE             = 4122;
	//const unsigned int ACK_PLAY_3D_CUE             = 4123;
	
	const unsigned int MSG_PLAY_CUE_AT             = 4124;
	//const unsigned int ACK_PLAY_CUE_AT             = 4125;
	
	const unsigned int MSG_STOP_CUE                = 4190;
	const unsigned int ACK_STOP_CUE                = 4191;
}
