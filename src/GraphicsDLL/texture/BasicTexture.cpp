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

#include "BasicTexture.h"

// Standard Includes
#include <string>

// Library Includes
#include <EngineConfig.h>

// Local Includes
#include "../../sharedbase/EventLogger.h"

// Forward Declarations

// Typedefs

// Local Constants

namespace GraphicsCore {
	TextureCacheMap BasicTexture::TextureCache;
}

namespace GraphicsCore {
	BasicTexture::BasicTexture() :
		texture(nullptr)
		{
	}
	
	BasicTexture::BasicTexture(RenderModule* renderModule, std::string file) {
		this->LoadTextureFromFile(renderModule, file);
	}
	
	BasicTexture::~BasicTexture() {
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void BasicTexture::Apply(LPDIRECT3DDEVICE9 pd3dDevice) {
		// NOTE: null textures are allowed: they are the only way to clear out a previous texture!
		pd3dDevice->SetTexture(0, this->texture);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void BasicTexture::LoadTextureFromFile(RenderModule* renderModule, std::string file) {
		if (file.length()) {
			TextureCacheMap::const_iterator tex_cache_it = BasicTexture::TextureCache.find(file);
			if (tex_cache_it != BasicTexture::TextureCache.end()) {
				LOG(LOG_PRIORITY::FLOW, "Texture cache hit on file '" + file + "'!");
				
				this->texture = tex_cache_it->second;
			}
			else {
				LOG(LOG_PRIORITY::FLOW, "Texture cache miss on file '" + file + "', loading from disk and storing in cache.");
				
				HRESULT result = renderModule->TextureFactory(NLS_ENGINE_DATA_PATH + file, &this->texture);
				
				BasicTexture::TextureCache.insert(TextureCacheMapValue(file, this->texture));
				
				if (FAILED(result)) {
					std::string error;
					switch (result) {
						case D3DERR_NOTAVAILABLE:
							error = "Direct3D device requested is not availiable.";
						break;
						case D3DERR_OUTOFVIDEOMEMORY:
							error = "There isn't enough video memory to create Direct3D device.";
						break;
						case D3DERR_INVALIDCALL:
							error = "Invalid Direct3D device creation call.";
						break;
						case D3DXERR_INVALIDDATA:
							error = "Invalid Direct3D data call.";
						break;
						case E_OUTOFMEMORY:
							error = "Out of System memory.";
						break;
						default:
							error = "Unexpected error; do some debugging.";
						break;
					}
					
					LOG(LOG_PRIORITY::INFO, error + "  Could not create texture from file: " + file);
				}
			}
		}
		else {
			this->texture = nullptr;
		}
	}
}
