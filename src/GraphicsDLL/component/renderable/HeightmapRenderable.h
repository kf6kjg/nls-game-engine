/**
 * \file
 * \author Ricky Curtice
 * \date 20110812
 * \brief Provides the interface class declaration for methods and objects for getting and setting a heightmap terrain
 */
#pragma once

#include <string>

#include <d3d9.h>
#include <d3dx9tex.h>

#include "../RenderableComponent.h"

class Grid;

namespace GraphicsCore {

	class HeightmapRenderable: public RenderableComponent {
	public:
		HeightmapRenderable(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9);
		virtual ~HeightmapRenderable();

		void Initialize(LPDIRECT3DDEVICE9);
		void Render(LPDIRECT3DDEVICE9);
		void CreateHeightmap(LPDIRECT3DDEVICE9);

		bool LoadHeightmapFromFile(const std::string&);
		bool LoadTextureFromFile(const std::string&);

	private:
		
		LPDIRECT3DDEVICE9 pd3dDevice;

		LPDIRECT3DTEXTURE9 texture;
		Grid* grid;
		
	};
}
