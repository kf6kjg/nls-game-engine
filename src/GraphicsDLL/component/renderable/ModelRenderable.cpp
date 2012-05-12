/**
 * \file
 * \author Justin Avignone
 * \date 20110715
 * \brief
 */

#include "ModelRenderable.h"

#include <cassert>

#include <d3d9.h>
#include <d3dx9tex.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <EngineConfig.h>

#include "../../../sharedbase/Entity.h"
#include "../../../sharedbase/QuatMath.h"
#include "../../../sharedbase/EventLogger.h"
#include "../../../sharedbase/Envelope.h"

#include "../../RenderModule.h"

#include "../../material/BasicMaterial.h"
#include "../../texture/BasicTexture.h"

#include "../../Materials.h"
#include "../../Textures.h"

namespace GraphicsCore {
	MeshCacheMap ModelComponent::MeshCache;
}

namespace GraphicsCore {
	ModelComponent::ModelComponent(EntitySPTR owner, ModuleInterface* renderModule) :
		RenderableComponent(owner, renderModule),
		mesh(nullptr),
		materialCount(0)
		{
	} //end of constructor

	ModelComponent::~ModelComponent() {
		long index;
		
		index = this->materialCount;
		while (--index >= 0) {
			this->RemoveMaterial(index);
			this->RemoveTexture(index);
		}
	} //end of destructor
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void ModelComponent::Render(LPDIRECT3DDEVICE9 d3d_device) {
		if (this->mesh != NULL) {
			//loop through the number of materials for the mesh
			for (unsigned long i = 0; i < this->materialCount; ++i) {

				//set the material for the mesh subset
				this->meshMaterials.at(i)->Apply(d3d_device);
				if (i < this->meshTextures.size()) {
					d3d_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
					this->meshTextures.at(i)->Apply(d3d_device);
				}

				//Draw the current mesh subset
				this->mesh->DrawSubset(i);

			} //end for loop
		}
	} //end Render()

	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelComponent::InsertMaterial(unsigned int index, EnvelopeSPTR envelope) {
		MaterialInterface* newMaterial = nullptr;
		
		// Make sure we're not higher than the model can handle.
		if (index < this->materialCount) {
			newMaterial = MaterialsSystemInterface::CreateMaterial(envelope);
			
			// Make sure a new material was created!
			if (newMaterial != nullptr) {
				// Remove any pre-existing material
				this->RemoveMaterial(index); // Return value doesn't need to be tested: if there was no material, we can continue.  If there was, this will remove it and we can continue.
				
				// Place the new material
				if (index >= this->meshMaterials.size()) {
					assert(index == this->meshMaterials.size()); // If this fails then we are trying to insert way too far ahead.
					this->meshMaterials.push_back(newMaterial);
				}
				else {
					this->meshMaterials.at(index) = newMaterial;
				}
				
				return true;
			}
			else {
				LOG(LOG_PRIORITY::INFO, "Material could not be created; not replacing existing material.");
			}
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Material index out of bounds: this ModelComponent only supports " + boost::lexical_cast < std::string > (this->materialCount) + " materials at indices 0 through " + boost::lexical_cast < std::string > (this->materialCount - 1) + "." );
		}
		
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelComponent::RemoveMaterial(unsigned int index) {
		// Make sure we're not higher than the model can handle.
		if (index < this->materialCount) {
			// Only do removals if the index actually exists in the list.  If it doesn't, then it didn't really need removal, as it doesn't exist!
			if (index < this->meshMaterials.size()) {
				
				MaterialsSystemInterface::DestroyMaterial(this->meshMaterials.at(index));
				
				this->meshMaterials.at(index) = nullptr;
			}
			
			return true;
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Material index of '" + boost::lexical_cast < std::string > (index) + "' out of bounds: this ModelComponent only supports " + boost::lexical_cast < std::string > (this->materialCount) + " materials at indices 0 through " + boost::lexical_cast < std::string > (this->materialCount - 1) + "." );
		}
		
		return false;
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelComponent::InsertTexture(unsigned int index, EnvelopeSPTR envelope) {
		TextureInterface* newTexture = nullptr;
		
		// Make sure we're not higher than the model can handle.
		if (index < this->materialCount) {
			newTexture = TexturesSystemInterface::CreateTexture(envelope, this->GetRenderModule());
			
			// Make sure a new texture was created!
			if (newTexture != nullptr) {
				// Remove any pre-existing texture
				this->RemoveTexture(index); // Return value doesn't need to be tested: if there was no texture, we can continue.  If there was, this will remove it and we can continue.
				
				// Place the new texture
				if (index >= this->meshTextures.size()) {
					assert(index == this->meshTextures.size()); // If this fails then we are trying to insert way too far ahead.
					this->meshTextures.push_back(newTexture);
				}
				else {
					this->meshTextures.at(index) = newTexture;
				}
				
				return true;
			}
			else {
				LOG(LOG_PRIORITY::INFO, "Texture could not be created; not replacing existing texture.");
			}
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Texture index out of bounds: this ModelComponent only supports " + boost::lexical_cast < std::string > (this->materialCount) + " textures at indices 0 through " + boost::lexical_cast < std::string > (this->materialCount - 1) + "." );
		}
		
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelComponent::RemoveTexture(unsigned int index) {
		// Make sure we're not higher than the model can handle.
		if (index < this->materialCount) {
			// Only do removals if the index actually exists in the list.  If it doesn't, then it didn't really need removal, as it doesn't exist!
			if (index < this->meshTextures.size()) {
				
				TexturesSystemInterface::DestroyTexture(this->meshTextures.at(index));
				
				this->meshTextures.at(index) = nullptr;
			}
			
			return true;
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Texture index of '" + boost::lexical_cast < std::string > (index) + "' out of bounds: this ModelComponent only supports " + boost::lexical_cast < std::string > (this->materialCount) + " textures at indices 0 through " + boost::lexical_cast < std::string > (this->materialCount - 1) + "." );
		}
		
		return false;
	}


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ModelComponent::LoadMeshFromFile(const std::string& file) {
		LPD3DXBUFFER materialBuffer;
		LPD3DXMESH mesh;
		
		RenderModule* renderModule = this->GetRenderModule();

		// Verify that we actually have a pointer to a RenderModule
		assert(renderModule != nullptr);
		
		MeshCacheMap::const_iterator mesh_cache_it = ModelComponent::MeshCache.find(file);
		if (mesh_cache_it != ModelComponent::MeshCache.end()) {
			LOG(LOG_PRIORITY::FLOW, "Mesh cache hit on file '" + file + "'!");
			
			this->materialCount = mesh_cache_it->second->materialCount;
			materialBuffer = mesh_cache_it->second->materialBuffer;
			mesh =  mesh_cache_it->second->mesh;
		}
		else {
			LOG(LOG_PRIORITY::FLOW, "Mesh cache miss on file '" + file + "', loading from disk and storing in cache.");
			HRESULT hr(renderModule->MeshFactory(NLS_ENGINE_DATA_PATH + file, D3DXMESH_SYSTEMMEM, NULL, &materialBuffer, NULL, &(this->materialCount), &mesh));
			
			ModelComponent::MeshCache.insert(MeshCacheMapValue(file, std::shared_ptr<MeshData>(new MeshData(mesh, this->materialCount, materialBuffer))));
			
			if (FAILED(hr)) {
				LOG(LOG_PRIORITY::INFO, "Error loading mesh '" + file + "' from disk!");
				return false;
			}
		}
		
		SetMaterials(materialBuffer);
		
		this->mesh = mesh;
		
		return true;
	} //end LoadMeshFromFile()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Good info at http://www.toymaker.info/Games/html/load_x_simply.html
	void ModelComponent::SetMaterials(LPD3DXBUFFER materialBuffer) {
		//pointer to the material buffer within the mesh
		D3DXMATERIAL* file_materials = (D3DXMATERIAL*) materialBuffer->GetBufferPointer();

		//Loop through the materials in the mesh and create a D3DMATERIAL for each one
		for (unsigned long index = 0; index < this->materialCount; ++index) {
			// Copy the materials.
			{
				// Override the Ambient color with the Diffuse color.  This is a workaround for invalid ambient values in the mesh file spec.
				file_materials[index].MatD3D.Ambient = file_materials[index].MatD3D.Diffuse;
				
				EnvelopeSPTR material(new Envelope());
				material->msgid = ID_MATERIAL_BASIC_USING_MAT;
				material->AddData(file_materials[index].MatD3D);
				
				this->InsertMaterial(index, material);
			}
			
			// Bring in the textures
			{
				EnvelopeSPTR texture(new Envelope());
				texture->msgid = ID_TEXTURE_BASIC;
				
				if (file_materials[index].pTextureFilename) {
					texture->AddData(ID_TEXTURE_PARAM_FILE);
					texture->AddData(std::string(file_materials[index].pTextureFilename));
				}
				
				this->InsertTexture(index, texture);
			}
		} //end for loop
	}
}
