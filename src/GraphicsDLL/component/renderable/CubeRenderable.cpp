/**
 * \file
 * \author Ricky Curtice
 * \date 20110715
 * \brief A simple cube
 *
 * <p>
 * Provides a good example how a basic renderable is created.
 * </p>
 *
 */

#include "CubeRenderable.h"

// Standard Includes

// Library Includes
#include <d3d9.h>

// Local Includes
#include "../../../sharedbase/EventLogger.h"

#include "../../material/MaterialInterface.h"
#include "../../material/BasicMaterial.h"

// Typedefs

// Constants

// Class Methods
namespace GraphicsCore {
	CubeRenderable::CubeRenderable(EntitySPTR owner, ModuleInterface* renderModule, LPDIRECT3DDEVICE9 d3dDevice) :
			RenderableComponent(owner, renderModule) {
		D3DXCreateBox(d3dDevice, 1.0, 1.0, 1.0, &(this->mesh), NULL);
		
		material = nullptr;
	}
	
	CubeRenderable::~CubeRenderable() {
		this->RemoveMaterial(0);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void CubeRenderable::Render(LPDIRECT3DDEVICE9 d3dDevice) {
		if (this->mesh != nullptr) {
			if (this->material != nullptr) {
				this->material->Apply(d3dDevice);
			}
			
			this->mesh->DrawSubset(0);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool CubeRenderable::InsertMaterial(unsigned int index, EnvelopeSPTR envelope) {
		MaterialInterface* newMaterial = nullptr;
		
		// Cube only has one material.
		if (index == 0) {
			newMaterial = MaterialsSystemInterface::CreateMaterial(envelope);
			
			// Make sure a new material was created!
			if (newMaterial != nullptr) {
				// Remove any pre-existing material
				this->RemoveMaterial(index); // Return value doesn't need to be tested: if there was no material, we can continue.  If there was, this will remove it and we can continue.
				
				// Place the new material
				this->material = newMaterial;
				
				return true;
			}
			else {
				LOG(LOG_PRIORITY::INFO, "Material could not be created; not replacing existing material.");
			}
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Material index out of bounds: CubeRenderable only supports 1 material at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool CubeRenderable::RemoveMaterial(unsigned int index) {
		// Cube only has one material.
		if (index == 0) {
			MaterialsSystemInterface::DestroyMaterial(this->material);
			
			this->material = nullptr;
			
			return true;
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Material index out of bounds: CubeRenderable only supports 1 material at index 0." );
		}
		
		return false;
	}
}
