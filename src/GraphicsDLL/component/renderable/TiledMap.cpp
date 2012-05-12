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

#include "TiledMap.h"

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
	TiledMap::TiledMap(EntitySPTR owner, ModuleInterface* renderModule, LPDIRECT3DDEVICE9 d3dDevice) : RenderableComponent(owner, renderModule) {
		D3DXCreateSprite(d3dDevice, &this->sprite);
	}

	TiledMap::~TiledMap( void ) {

	}

	void TiledMap::AddTile( float x, float y, std::string texName, int srcX, int srcY ) {
		LPDIRECT3DTEXTURE9 tex;
		CreateTexture(texName, &tex);
		RECT srcRect = {srcX, srcY, TILE_WIDTH, TILE_HEIGHT};
		Tile t = {x, y, tex};
		t.srcRect = srcRect;
		this->tiles.push_back(t);
	}

	void TiledMap::Render( LPDIRECT3DDEVICE9 ) {
		float offX = this->owner->GetWorldPosition().x;
		float offY = this->owner->GetWorldPosition().y;
		unsigned int viewportWidth = this->owner->GetParent()->properties.GetPropertyValue<unsigned int>("viewportWidth");
		unsigned int viewportHeight = this->owner->GetParent()->properties.GetPropertyValue<unsigned int>("viewportHeight");

		D3DXVECTOR3 pos;
		pos.z = 1.0f; // Always 0

		LPDIRECT3DTEXTURE9 tex;
		RECT src;
		this->sprite->Begin(D3DXSPRITE_ALPHABLEND);
		int tilewidth = this->owner->properties.GetPropertyValue<unsigned int>("tileWidth") * -1;
		int tileheight = this->owner->properties.GetPropertyValue<unsigned int>("tileHeight") * -1;
		for (std::vector<Tile>::iterator itr = this->tiles.begin(); itr != this->tiles.end(); ++itr) {
			tex = itr->texture;
			src = itr->srcRect;
			pos.x = itr->x - offX; // Subtract the owner's offset
			pos.y = itr->y - offY; // Subtract the owner's offset

			if ((pos.x >= tilewidth && pos.x < viewportWidth) && (pos.y >= tileheight && pos.y < viewportHeight)) {
				this->sprite->Draw(tex, &src, NULL, &pos, 0xFFFFFFFF);
			}
		}
		this->sprite->End();
	}

	bool TiledMap::LoadMapData( EnvelopeSPTR data ) {
		EnvelopeSPTR tsetsEnv(data.get()->GetDataEnvelopeSPTR(0)); // First envelope is tilesets
		std::vector<TileSet> tilesets;
		for (unsigned int i = 0; i < tsetsEnv.get()->GetCount(); ++i) { // Loop through each tileset
			EnvelopeSPTR tset(tsetsEnv.get()->GetDataEnvelopeSPTR(i)); // Get each tileset
			TileSet temptset;
			temptset.name = tset.get()->GetDataString(0);	// 0 - Name
			temptset.across = tset.get()->GetDataUInt(1);	// 1 - Across
			temptset.down = tset.get()->GetDataUInt(2);		// 2 - Down
			temptset.firstid = tset.get()->GetDataUInt(3);	// 3 - Firstid
			temptset.lastid = tset.get()->GetDataUInt(4);	// 4 - Lastid
			tilesets.push_back(temptset);

			LPDIRECT3DTEXTURE9 tex;
			CreateTexture(NLS_ENGINE_DATA_PATH + "/maps/" + tilesets[i].name, &tex);
			this->textures[tilesets[i].name] = tex;
		}

		unsigned int tileWidth = this->owner->properties.GetPropertyValue<unsigned int>("tileWidth");
		unsigned int tileHeight = this->owner->properties.GetPropertyValue<unsigned int>("tileHeight");

		EnvelopeSPTR layersEnv(data.get()->GetDataEnvelopeSPTR(1)); // Second envelope is layers
		for (unsigned int i = 0; i < layersEnv.get()->GetCount(); ++i) { // Loop through each layer
			EnvelopeSPTR layer(layersEnv.get()->GetDataEnvelopeSPTR(i)); // Get each layer
			for (unsigned int n = 0; n < layer.get()->GetCount(); n = n + 3) { // Loop through each tile
				unsigned int r = layer.get()->GetDataUInt(n);			// 0 - row
				unsigned int c = layer.get()->GetDataUInt(n + 1);		// 1 - column
				unsigned int tileid = layer.get()->GetDataUInt(n + 2);	// 2 - tileid

				TileSet temptset;
				for (auto itr = tilesets.begin(); itr != tilesets.end(); ++itr) {
					if ((tileid >= itr->firstid) && (tileid < itr->lastid)) {
						temptset = (*itr);
					}
				}

				Tile t;
				t.texture = this->textures[temptset.name];
				t.x = c * tileWidth;
				t.y = r * tileHeight;

				// Determine the row and column offsets for finding the source locations
				int tileOffset = tileid - temptset.firstid; // We need to normalize the offset
				int rowOffset = tileOffset / temptset.across; // Next, we determine how many rows are in the id
				int colOffset = tileOffset - (temptset.across * rowOffset); // Finally, get the column by subtracting the rows.

				t.srcRect.left = colOffset * tileWidth;
				t.srcRect.top = rowOffset * tileHeight;
				t.srcRect.right = t.srcRect.left + tileWidth;
				t.srcRect.bottom = t.srcRect.top + tileHeight;

				this->tiles.push_back(t);
			}
		}

		return true;
	}
}
