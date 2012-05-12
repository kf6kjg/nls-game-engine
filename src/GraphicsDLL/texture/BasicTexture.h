/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a simple single-layer basic diffuse texture implementation.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes
#include <string>
#include <map>

// Library Includes
#include <d3dx9tex.h>

// Local Includes
#include "../RenderModule.h"
#include "TextureInterface.h"

// Forward Declarations

// Typedefs
namespace GraphicsCore {
	typedef std::map<std::string, Texture> TextureCacheMap;
	typedef TextureCacheMap::value_type TextureCacheMapValue;
}

// Class
namespace GraphicsCore {
	class BasicTexture : public TextureInterface {
	public:
		BasicTexture();
		BasicTexture(RenderModule*, std::string);
		~BasicTexture();
		
	private: // Inherited interface function implmentations should be private to help the compiler detect potential issues if the interface changes.
		void Apply(LPDIRECT3DDEVICE9);
		
	private:
		/// Provides facities for loading a texture from a given file.
		void LoadTextureFromFile(RenderModule*, std::string);
		
	private:
		static TextureCacheMap TextureCache;
		
	private:
		Texture texture;
		
	};
}
