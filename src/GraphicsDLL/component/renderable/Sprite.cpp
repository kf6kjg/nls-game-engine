/**
 * \file
 * \author Ricky Curtice
 * \date 20120112
 * \brief A dynamic helical tube mesh
 *
 * <p>
 * </p>
 *
 */

#include "Sprite.h"

// System Library Includes

// Application Library Includes
#include <EngineConfig.h>

// Local Includes
#include "../../../sharedbase/Entity.h"
#include "../../AssetCreationHelpers.h"

// Forward Declarations

// Typedefs and data structures

// Constants

// Class methods
namespace GraphicsCore {
	Sprite::Sprite(EntitySPTR owner, ModuleInterface* renderModule, LPDIRECT3DDEVICE9 d3dDevice) : RenderableComponent(owner, renderModule) {
		D3DXCreateSprite(d3dDevice, &this->sprite);
	}

	Sprite::~Sprite( void ) {

	}

	void Sprite::SetSpriteParams( float offX, float offY, std::string texName, unsigned int srcX, unsigned int srcY, unsigned int width, unsigned int height ) 	{
		CreateTexture(NLS_ENGINE_DATA_PATH + texName, &this->texture);
		this->srcRect.left = srcX;
		this->srcRect.top = srcY;
		this->srcRect.right = width + srcX;
		this->srcRect.bottom = height + srcY;
		this->offsetX = offX;
		this->offsetY = offY;
	}

	void Sprite::Render( LPDIRECT3DDEVICE9 ) {
		D3DXVECTOR3 pos;
		unsigned int viewportWidth = 800;
		unsigned int viewportHeight = 768;
		if (this->owner->GetParent().get() != nullptr) {
			pos.x = this->owner->location.x + this->offsetX - this->owner->GetParent()->GetWorldPosition().x; // Add the sprites offset to our entity's location
			pos.y = this->owner->location.y + this->offsetY - this->owner->GetParent()->GetWorldPosition().y; // Add the sprites offset to our entity's location

			viewportWidth = this->owner->GetParent()->properties.GetPropertyValue<unsigned int>("viewportWidth");
			viewportHeight = this->owner->GetParent()->properties.GetPropertyValue<unsigned int>("viewportHeight");
		} else {
			pos.x = this->owner->location.x + this->offsetX;
			pos.y = this->owner->location.y + this->offsetY;
		}
		pos.z = this->owner->location.z;

		int sprite_width = this->srcRect.right - this->srcRect.left;
		int sprite_height = this->srcRect.bottom - this->srcRect.top;
		if (((pos.x >= (sprite_width * -1)) && pos.x < viewportWidth) && ((pos.y >= (sprite_height * -1)) && pos.y < viewportHeight)) {
			this->sprite->Begin(D3DXSPRITE_ALPHABLEND);
			this->sprite->Draw(this->texture, &this->srcRect, NULL, &pos, 0xFFFFFFFF);
			this->sprite->End();
		}
	}
}
