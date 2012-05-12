/**
 * \file
 * \author Ricky Curtice
 * \date 20110724
 * \brief Provides a list of the global property key strings created by Graphics Core and their default values
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include <string>

#include "Types.h"

namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * */
	// Gloabal Properties
	/* * * * * * * * * * * * * * * * * * * * * */
	const std::string  GP_WINDOW_WIDTH("screenheight");
	const unsigned int GP_WINDOW_WIDTH_DEF = 1024;
	const std::string  GP_WINDOW_HEIGHT("screenwidth");
	const unsigned int GP_WINDOW_HEIGHT_DEF = 768;
	const std::string  GP_CLIENT_WIDTH("ClientWidth");
	const unsigned int GP_CLIENT_WIDTH_DEF = GP_WINDOW_WIDTH_DEF;
	const std::string  GP_CLIENT_HEIGHT("ClientHeight");
	const unsigned int GP_CLIENT_HEIGHT_DEF = GP_WINDOW_HEIGHT_DEF;
	const std::string  GP_CLIENT_ASPECT("ClientAspect");
	const float        GP_CLIENT_ASPECT_DEF = (float) GP_CLIENT_WIDTH_DEF / (float) GP_CLIENT_HEIGHT_DEF;
	const std::string  GP_CLIENT_ASPECT_INV("ClientAspectInv");
	const float        GP_CLIENT_ASPECT_INV_DEF = 1.0f / GP_CLIENT_ASPECT_DEF;
	
	const std::string  GP_GRAPHICS_PAUSE("GraphicsPause");
	const bool         GP_GRAPHICS_PAUSE_DEF = false;
	
	const std::string  GP_CLEAR_COLOR("ClearColor");
	const Color        GP_CLEAR_COLOR_DEF = Color(0.0, 0.0, 0.0, 1.0);;
	
	const std::string  GP_ANISOTROPIC_FILTERING("AnisotropicFiltering");
	const unsigned int GP_ANISOTROPIC_FILTERING_DEF = 0;
	
	const std::string  GP_FOG_ENABLED("FogEnabled");
	const bool         GP_FOG_ENABLED_DEF = false;
	const std::string  GP_FOG_COLOR("FogColor");
	const Color        GP_FOG_COLOR_DEF = Color(1.0, 1.0, 1.0, 1.0);
	const std::string  GP_FOG_MODE("FogMode");
	const unsigned int GP_FOG_MODE_DEF = 3; //  D3DFOG_NONE = 0, D3DFOG_EXP = 1, D3DFOG_EXP2 = 2, D3DFOG_LINEAR = 3
	const std::string  GP_FOG_RANGE_NEAR("FogRangeNear"); // Used in linear mode
	const float        GP_FOG_RANGE_NEAR_DEF = 0.5f;
	const std::string  GP_FOG_RANGE_FAR("FogRangeFar"); // Used in linear mode
	const float        GP_FOG_RANGE_FAR_DEF = 0.8f;
	const std::string  GP_FOG_RANGE_DENSITY("FogDensity"); // Used in either of the exponential modes
	const float        GP_FOG_RANGE_DENSITY_DEF = 0.66f;

	const std::string GP_HWND("hwnd");

}
