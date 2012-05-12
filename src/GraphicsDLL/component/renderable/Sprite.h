/**
 * \file
 * \author Adam Martin
 * \date 20120120
 * \brief Simple sprite class for drawing basic sprites.
 *
 * <p>
 * 
 * </p>
 *
 */
#pragma once

// Standard Includes

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

// Local Includes
#include "../RenderableComponent.h"

// Forward Declarations

// Typedefs

// Constants

// Classes
namespace GraphicsCore {
	class Sprite : public RenderableComponent {
	public:
		Sprite(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9);
		virtual ~Sprite(void);

		void SetSpriteParams( float offX, float offY, std::string texName, unsigned int srcX, unsigned int srcY, unsigned int width, unsigned int height );
	private:
		void Render(LPDIRECT3DDEVICE9);
		LPD3DXSPRITE sprite;
		float offsetX, offsetY; // location
		LPDIRECT3DTEXTURE9 texture; // Source texture.
		RECT srcRect; // Source location in the texture.
	};
}
