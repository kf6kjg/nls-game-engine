/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a common interface for all texture types.
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

namespace GraphicsCore {
	class TextureInterface {
	public:
		virtual ~TextureInterface() {}
		
		virtual void Apply(LPDIRECT3DDEVICE9) = 0;
		
	private:
		TextureInterface& operator=(const TextureInterface&); ///< Assignment operator disabled for pure interface class
	};
}
