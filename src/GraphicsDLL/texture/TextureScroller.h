/**
* \file
* \author Ricky Curtice
* \date 2012-01-02
* \brief Provides a simple UV scroller texture implementation.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes
#include <string>

// Library Includes
#include <d3dx9tex.h>

// Local Includes
#include "TextureInterface.h"
#include "../InterfaceUpdateable.h"

// Forward Declarations

// Typedefs/types
namespace GraphicsCore {
	struct UVA {
		float u; ///< U coordinate
		float v; ///< V coordinate
		float a; ///< Rotation angle (radians)
	};
}

namespace GraphicsCore {
	class RenderModule;

	class TextureScroller : public TextureInterface, public InterfaceUpdateable {
	public:
		/// Create a simple, does-not-a-lot version
		TextureScroller();
		/// Create a texture scroller wrapping another texture.  When this is called, this class takes ownership of the pointer.
		TextureScroller(TextureInterface*);
		~TextureScroller();
		
		void SetUVVelocity(float, float);
		void SetUVelocity(float);
		void SetVVelocity(float);
		void SetAngularVelocity(float);
		
	private: // Inherited interface function implementations should be private to help the compiler detect potential issues if the interface changes.
		void Apply(LPDIRECT3DDEVICE9);
		
		void Update(double);
		
	private:
		void Init(float, float, float);
		
	private:
		TextureInterface* texture;
		
		UVA velocity;
		D3DXMATRIX currentMatrix;
	};
}
