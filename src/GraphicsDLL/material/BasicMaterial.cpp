/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a simple single-layer basic diffuse material implementation.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

#include "BasicMaterial.h"

// Standard Includes
#include <string>

// Library Includes
#include <d3d9.h>

// Local Includes

// Forward Declarations

// Typedefs

namespace GraphicsCore {
	BasicMaterial::BasicMaterial() {
		this->Init(BLANK_MATERIAL);
	}
	
	BasicMaterial::BasicMaterial(Color diffuse, Color ambient, Color specular, Color emissive, float power) {
		Material material = {diffuse, ambient, specular, emissive, power};
		this->Init(material);
	}
	
	BasicMaterial::BasicMaterial(Material& material) {
		this->Init(material);
	}
	
	BasicMaterial::~BasicMaterial() {
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void BasicMaterial::Apply(LPDIRECT3DDEVICE9 pd3d_device) {
		pd3d_device->SetMaterial(&(this->material));
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void BasicMaterial::Init(Material material) {
		// *NOTE: This winds up doing a form of deep copy because the parameter is a pass-by-value.  This is wanted, and if it doesn't happen there may be consequences.
		this->material = material;
	}
}
