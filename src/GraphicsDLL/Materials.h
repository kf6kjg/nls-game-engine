/**
 * \file
 * \author Ricky Curtice
 * \date 2012-01-04
 * \brief Provides a list of the material IDs used and constant handles to reference them by.
 *
 * <p>
 * </p>
 */
#pragma once

namespace GraphicsCore {
	// Material types
	const unsigned int ID_MATERIAL_BASIC           = 0; // Followed by: any of the Color channel identifiers listed below, in any order.
	const unsigned int ID_MATERIAL_BASIC_USING_MAT = 1; // Followed by: BOOST::Any<Material> a material to copy.
	
	// Color channel identifier
	const unsigned int ID_MATERIAL_COLOR_AMBIENT  = 0; // Followed by: float red, float green, float blue, float alpha
	const unsigned int ID_MATERIAL_COLOR_DIFFUSE  = 1; // Followed by: float red, float green, float blue, float alpha
	const unsigned int ID_MATERIAL_COLOR_SPECULAR = 2; // Followed by: float red, float green, float blue, float alpha
	const unsigned int ID_MATERIAL_COLOR_EMISSIVE = 3; // Followed by: float red, float green, float blue, float alpha
	const unsigned int ID_MATERIAL_COLOR_POWER    = 4; // Followed by: float power
}
