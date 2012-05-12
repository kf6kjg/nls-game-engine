/**
 * \file
 * \author Ricky Curtice
 * \date 2012-01-16
 * \brief Provides a list of the texture IDs used and constant handles to reference them by.
 *
 * <p>
 * </p>
 */
#pragma once

namespace GraphicsCore {
	// Texture types
	const unsigned int ID_TEXTURE_BASIC = 0; // Followed by: optional file path.
	const unsigned int ID_TEXTURE_SCROLL_ANIM = 1; // Followed by, in any order: optional file path, optional texture linear velocity, optional texture angular velocity.
	const unsigned int ID_TEXTURE_ADJUSTABLE = 2; // Followed by, in any order: required file path, optional message type id.
	
	const unsigned int ID_TEXTURE_PARAM_FILE = 0; // Followed by: string.
	const unsigned int ID_TEXTURE_PARAM_UV_VEL = 1; // Followed by: float, float.
	const unsigned int ID_TEXTURE_PARAM_A_VEL = 2; // Followed by: float.
	const unsigned int ID_TEXTURE_PARAM_MESSAGE_TYPE = 3; // Followed by: unsigned int.
}
