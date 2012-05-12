/**
 * \file
 * \author Adam Martin
 * \date 20120120
 * \brief A map drawn of different, regularly-sized, tiles.
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
#include <boost/unordered_map.hpp>
#include <vector>

// Local Includes
#include "../RenderableComponent.h"
#include "../../../sharedbase/Envelope.h"

// Forward Declarations

// Typedefs

// Constants
namespace GraphicsCore {
	static const unsigned int TILE_WIDTH = 32; // Width of the tile, in pixels
	static const unsigned int TILE_HEIGHT = 32; // Height of the tile, in pixels
}

// Classes
namespace GraphicsCore {
	struct Tile {
		float x, y; // location
		LPDIRECT3DTEXTURE9 texture; // Source texture.
		RECT srcRect; // Source location in the texture.
	};

	struct TileSet {
		std::string name;
		unsigned int across;
		unsigned int down;
		unsigned int firstid;
		unsigned int lastid;
	};

	class TiledMap : public RenderableComponent {
	public:
		TiledMap(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9);
		virtual ~TiledMap(void);

		void AddTile(float x, float y, std::string texName, int srcX, int srcY);
		bool LoadMapData(EnvelopeSPTR data);
	private:
		void Render(LPDIRECT3DDEVICE9);
		LPD3DXSPRITE sprite;
		boost::unordered_map<std::string, LPDIRECT3DTEXTURE9> textures; // Map of the various loaded textures.
		std::vector<Tile> tiles; // Vector of all tiles to be draw
	};
}
