/**
 * \file
 * \author Ricky Curtice
 * \date 20110805
 * \brief Provides a list of the message IDs used and constant handles to reference them by.
 *
 * <p>
 * </p>
 */
#pragma once

namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * */
	// Messages
	/* * * * * * * * * * * * * * * * * * * * * */
	const unsigned int ACK_LOAD_GRAPHICS = 1001; ///< Message sent from the graphics module to indicate that the module has loaded
	
	const unsigned int MSG_START_GRAPHICS = 1002; ///< Message sent to the graphics module to start it up
	const unsigned int ACK_START_GRAPHICS = 1003; ///< Message sent from the graphics module with a status indicator
	
	const unsigned int MSG_STOP_GRAPHICS = 1098; ///< Message sent to the graphics module to shut it down
	const unsigned int ACK_STOP_GRAPHICS = 1099; ///< Message sent from the graphics module with a status indicator
	
	const unsigned int MSG_SET_RENDER_RATE = 1100;
	const unsigned int ACK_SET_RENDER_RATE = 1101;
	
	const unsigned int MSG_SET_RENDER_DELAY = 1102;
	const unsigned int ACK_SET_RENDER_DELAY = 1103;
	
	const unsigned int MSG_GET_RENDER_RATE = 1110;
	const unsigned int ACK_GET_RENDER_RATE = 1111;
	
	const unsigned int MSG_GET_RENDER_DELAY = 1112;
	const unsigned int ACK_GET_RENDER_DELAY = 1113;
	
	const unsigned int MSG_SET_FILL_MODE = 1500;
	const unsigned int ACK_SET_FILL_MODE = 1501;
	
	const unsigned int MSG_SET_CURRENT_CAMERA = 1600;
	const unsigned int ACK_SET_CURRENT_CAMERA = 1601;
	
	// External messages that have no other header
	const unsigned int MSG_SYS_WINDOW_CHANGED = 9999;
}
