/**
 * \file
 * \author Ricky Curtice
 * \date 20110717
 * \brief Defines the class methods for the render engine core
 *
 * <p>
 * </p>
 *
 */

#include <d3d9.h>
#include <Dxerr.h>

#include "RenderModule.h"
#include "../sharedbase/EventLogger.h"

namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	HRESULT RenderModule::TextureFactory(const std::string& file, LPDIRECT3DTEXTURE9 *texture) {
		if (this->pd3dDevice == NULL) {
			LOG(LOG_PRIORITY::ERR, "Cannot load texture from file '" + file + "' because the DirectX device was not activated.");
			return -1;
		}
		
		HRESULT hr;
		
		// Load texture from designated file string
		hr = D3DXCreateTextureFromFileEx(this->pd3dDevice, //Our D3D Device
		    file.c_str(), //Filename of our texture
		    D3DX_DEFAULT, //Width:D3DX_DEFAULT = Take from file
		    D3DX_DEFAULT, //Height:D3DX_DEFAULT = Take from file
		    1, //MipLevels
		    0, //Usage, Is this to be used as a Render Target? 0 == No
		    D3DFMT_A8R8G8B8, //32-bit with Alpha, everything should support this
		    D3DPOOL_MANAGED, //Pool, let D3D Manage our memory
		    D3DX_DEFAULT, //Filter:Default filtering
		    D3DX_DEFAULT, //MipFilter, used for filtering mipmaps
		    0, //Disable ColourKey
		    NULL, //SourceInfo, returns extra info if we want it (we don't)
		    NULL, //Palette:We're not using one
		    texture // Our texture goes here.
		    );
		
		if (FAILED(hr)) {
			LOG(LOG_PRIORITY::MISSRESS, "Failed to load texture from file: " + file);
		}
		
		return hr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	HRESULT RenderModule::MeshFactory(const std::string& file, DWORD options, LPD3DXBUFFER* ppAdjacency, LPD3DXBUFFER* ppMaterials, LPD3DXBUFFER* ppEffectInstances, DWORD* pNumMaterials,
	    LPD3DXMESH* ppMesh) {
		if (this->pd3dDevice == NULL) {
			LOG(LOG_PRIORITY::ERR, "Cannot load mesh from file '" + file + "' because the DirectX device was not activated.");
			return -1;
		}
		
		HRESULT hr;
		
		// Load mesh from designated file string
		hr = D3DXLoadMeshFromX(file.c_str(), options, this->pd3dDevice, ppAdjacency, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
		
		if (FAILED(hr)) {
			LOG(LOG_PRIORITY::MISSRESS, "Failed to load mesh from file: " + file);
		}
		
		return hr;
	}
}
