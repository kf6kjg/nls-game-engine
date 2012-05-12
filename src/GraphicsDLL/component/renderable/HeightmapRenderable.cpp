/**
 * \file
 * \author Ricky Curtice
 * \date 20110812
 * \brief Provides the interface class declaration for methods and objects for getting and setting a height-map terrain
 */
#pragma once

#include <cassert>

#include <EngineConfig.h>
#include <CImg-1.5.0_beta/CImg.h>

#include "../../../sharedbase/EventLogger.h"

#include "../../RenderModule.h"

#include "GRID.H"
#include "HeightmapRenderable.h"

namespace GraphicsCore {

	HeightmapRenderable::HeightmapRenderable(EntitySPTR owner, ModuleInterface* core, LPDIRECT3DDEVICE9 pd3dDevice) :
			RenderableComponent(owner, core) {
		this->pd3dDevice = pd3dDevice;
		
		this->grid = NULL;
		
		this->texture = NULL;
	}
	
	HeightmapRenderable::~HeightmapRenderable() {

	}

	void HeightmapRenderable::Render(LPDIRECT3DDEVICE9 pd3dDevice) {
		if (this->texture != NULL) {
			pd3dDevice->SetTexture(0, this->texture);
		}
		
		if (this->grid != NULL) {
			this->grid->DrawDirectXMesh(pd3dDevice);
		}
	}

	bool HeightmapRenderable::LoadHeightmapFromFile(const std::string& file) {
		float width, height;
		
		width = height = 256.0f;
		
		D3DXVECTOR3 v0(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 v1(width, 0.0f, 0.0f);
		D3DXVECTOR3 v2(width, 40.0f, height);
		D3DXVECTOR3 v3(0.0f, 0.0f, height);
		
		cimg_library::CImg<float> image;
		try {
			image = cimg_library::CImg<float>((NLS_ENGINE_DATA_PATH + file).c_str());
		}
		catch (cimg_library::CImgIOException&) {
			LOG(LOG_PRIORITY::MISSRESS, "Heightmap failed to load!");
			return false;
		}

		if (image.data() == NULL) {
			LOG(LOG_PRIORITY::MISSRESS, "Heightmap is empty.");
			return false;
		}

		MAP map;
		map.width = 128; // this is the biggest a 16 bit mesh can handle unfortunately.
		map.height = 128;
		map.array = new float[map.width * map.height];
		
		float value;
		unsigned int stepi = image.width() / map.width; // Compensate for scaling issues between the mesh and the image.
		unsigned int stepj = image.height() / map.height;
		
		for (unsigned int y = 0, k = 0, i = 0; y < map.height; ++y, i += stepi) {
			for (unsigned int x = 0, j = 0; x < map.width; ++x, ++k, j += stepj) {
				value = (image(i, j, 0, 0) + image(i, j, 0, 1) + image(i, j, 0, 2)) / 3.0f; // Use the average value of the channels (IE: greyscale value)
				map.array[k] = value / 255.0f; // Assuming a range of [0,255] for the input values, wanting [0.0f,1.0f].
			}
		}

		Grid* grid = new Grid(map, &v0, &v1, &v2, &v3);
		
		grid->setTexScale(1.0f / float(map.width));
		
		grid->CreateDirectXMesh(this->pd3dDevice);
		
		this->grid = grid;
		
		return true;
	}

	bool HeightmapRenderable::LoadTextureFromFile(const std::string& file) {
		HRESULT result;
		RenderModule* renderModule = this->GetRenderModule();
		
		// Verify that we actually have a pointer to a RenderModule
		assert(renderModule != NULL);
		
		result = renderModule->TextureFactory(NLS_ENGINE_DATA_PATH + file, &texture);
		
		return SUCCEEDED(result);
	}

}
