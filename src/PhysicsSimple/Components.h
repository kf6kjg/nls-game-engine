/**
 * \file
 * \author Ricky Curtice
 * \date 20120129
 * \brief Provides a list of component name strings for creating components
 *
 * <p>
 * Provides a list of component name strings for creating components
 * </p>
 *
 */
#pragma once

// System Library Includes
#include <string>
#include <vector>

// Application Library Includes

// Local Includes
#include "Types.h"

// Forward Declarations

// Components listing
namespace Physics {
	const std::string               LINEAR_VELOCITY  = "linearvelocity";
		const std::string LINEAR_VELOCITY__VELOCITY = "velocity";
		typedef Vector3D  LINEAR_VELOCITY__VELOCITY_TYPE;
	
	const std::string   SPHERE_COLLIDER = "spherecollider";
		const std::string SPHERE_COLLIDER__RADIUS = "radius";
		typedef float     SPHERE_COLLIDER__RADIUS_TYPE;
}
