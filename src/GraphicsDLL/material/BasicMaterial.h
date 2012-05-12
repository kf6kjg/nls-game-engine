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

// Standard Includes
#include <string>

// Library Includes
#include <d3d9.h>

// Local Includes
#include "MaterialInterface.h"

// Forward Declarations

// Typedefs

// Classes
namespace GraphicsCore {
	class BasicMaterial : public MaterialInterface {
	public:
		/// Create the material with default values.
		BasicMaterial();
		/// Create the material with predefined diffuse, ambient, specular, emissive, and power values.
		BasicMaterial(Color, Color, Color, Color, float);
		/// Copy another material.
		BasicMaterial(Material&);
		
		virtual ~BasicMaterial();
		
	private: // Inherited interface function implmentations should be private to help the compiler detect potential issues if the interface changes.
		void Apply(LPDIRECT3DDEVICE9);
		
	private: // Helper methods
		void Init(Material);
		
	private:
		Material material;
		
	};
}
