/**
 * \file
 * \author Ricky Curtice
 * \date 20110814
 * \brief Provides simple global functions that can be used to help create objects.
 *
 */
#pragma once

#include <string>

#include "RenderModule.h"

/**
 * Loads a texture from the specified file into the texture pointer.  Supported file
 * types are: .bmp, .jpg, .png, and .tga
 *
 * \param file Input: The path and file name for a supported image file
 * \param texture Output: The resulting texture
 * \return whether or not the texture loaded successfully
 */
inline HRESULT CreateTexture(const std::string& file, LPDIRECT3DTEXTURE9 *texture) {
	return GraphicsCore::RenderModule::GetRenderModule()->TextureFactory(file, texture);
}

/**
 * Loads a mesh from the specified X-format mesh file.
 * Parameters are duplicates of the parameters for D3DXCreateMeshFromX(...)
 *
 * \param file Input: The path and file name for an X-format mesh
 * \param options Input:
 * \param ppAdjacency Output:
 * \param ppMaterials Output:
 * \param ppEffectInstances Output:
 * \param pNumMaterials Output:
 * \param ppMesh Output:
 * \return whether or not the mesh loaded successfully
 */
inline HRESULT CreateMesh(const std::string& file, DWORD options, LPD3DXBUFFER* ppAdjacency, LPD3DXBUFFER* ppMaterials, LPD3DXBUFFER* ppEffectInstances, DWORD* pNumMaterials, LPD3DXMESH* ppMesh) {
	return GraphicsCore::RenderModule::GetRenderModule()->MeshFactory(file, options, ppAdjacency, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
}
