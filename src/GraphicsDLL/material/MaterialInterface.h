/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a common interface for all material types.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes

// Library Includes
#include <d3dx9tex.h>

// Local Includes
#include "../Types.h"

// Forward Declarations

// Typedefs

// Local Constants

// Classes
namespace GraphicsCore {
	class MaterialInterface {
	public:
		virtual ~MaterialInterface() {}
		
		virtual void Apply(LPDIRECT3DDEVICE9) = 0;
		
	private:
		MaterialInterface& operator=(const MaterialInterface&); ///< Assignment operator disabled for pure interface class
	};
}
