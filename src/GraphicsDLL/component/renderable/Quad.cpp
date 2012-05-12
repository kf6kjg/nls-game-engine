/**
 * \file
 * \author Ricky Curtice
 * \date 20120314
 * \brief Textured quad class to provide custom sprite work.
 *
 * <p>
 * 
 * </p>
 *
 */
#include "Quad.h"

// System Library Includes

// Application Library Includes
#include <EngineConfig.h>

// Local Includes
#include "../../../sharedbase/EventLogger.h"
#include "../../material/MaterialInterface.h"
#include "../../texture/TextureInterface.h"

// Forward Declarations

// Typedefs and data structures
namespace GraphicsCore {
	struct QuadVertex {
		D3DXVECTOR3 pos;
		D3DXVECTOR3 norm;
		D3DXVECTOR2 texcoord;
	};
}

// Constants
namespace GraphicsCore {
	static const unsigned long QUAD_VERTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	static const D3DMATRIX UNIT_MATRIX = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

// Static members
namespace GraphicsCore {
	LPD3DXMESH Quad::mesh;
}

// Class methods
namespace GraphicsCore {
	Quad::Quad(EntitySPTR owner, ModuleInterface* render_module, LPDIRECT3DDEVICE9 d3d_device) :
		RenderableComponent(owner, render_module),
		width(1.0f),
		height(1.0f),
		originLocation(ORIGIN::CENTER),
		orientation(ORIENTATION::WORLD),
		material(nullptr),
		texture(nullptr),
		matrix(UNIT_MATRIX)
	{
		this->UpdateInstanceCache();
		
		if (Quad::mesh == nullptr) {
			std::vector<QuadVertex> vertices;
			std::vector<unsigned short> indices;
			
			// Create the vertices
			{
				QuadVertex vertex;
				
				vertex.pos      = D3DXVECTOR3( 0.0f,  0.0f,  0.0f);
				vertex.norm     = D3DXVECTOR3( 0.0f,  0.0f, -1.0f);
				vertex.texcoord = D3DXVECTOR2( 0.0f,  0.0f);
				vertices.push_back(vertex);
				
				vertex.pos      = D3DXVECTOR3( 1.0f,  0.0f,  0.0f);
				vertex.norm     = D3DXVECTOR3( 0.0f,  0.0f, -1.0f);
				vertex.texcoord = D3DXVECTOR2( 1.0f,  0.0f);
				vertices.push_back(vertex);
				
				vertex.pos      = D3DXVECTOR3( 1.0f, -1.0f,  0.0f);
				vertex.norm     = D3DXVECTOR3( 0.0f,  0.0f, -1.0f);
				vertex.texcoord = D3DXVECTOR2( 1.0f,  1.0f);
				vertices.push_back(vertex);
				
				vertex.pos      = D3DXVECTOR3( 0.0f, -1.0f,  0.0f);
				vertex.norm     = D3DXVECTOR3( 0.0f,  0.0f, -1.0f);
				vertex.texcoord = D3DXVECTOR2( 0.0f,  1.0f);
				vertices.push_back(vertex);
				
				if (vertices.size() < 3) {
					LOG(LOG_PRIORITY::ERR, "Parameters must be wrong because there are less than 3 vertices!");
					return;
				}
			}
			
			// Create the indices for the index buffer
			{
				indices.push_back(0);
				indices.push_back(1);
				indices.push_back(2);
				
				indices.push_back(2);
				indices.push_back(3);
				indices.push_back(0);
			}
			
			// Build the mesh
			{
				// Create the mesh itself
				{
					if (FAILED(D3DXCreateMeshFVF(
						indices.size() / 3, // Number of faces
						vertices.size(),    // Number of vertices
						D3DXMESH_MANAGED,   // Options
						QUAD_VERTEX_FVF,    // FVF
						d3d_device,
						&Quad::mesh
					))) {
						std::string err = "Mesh could not be initialized! " + std::string((Quad::mesh == nullptr) ? "Mesh was null, not sure what happened..." : "Mesh wasn't null.  Possible issue with needing to delete mesh before recreating.");
						LOG(LOG_PRIORITY::INFO, err);
						return;
					}
					
				}
				
				// Fill the vertex buffer
				{
					void* vertexBuffer;
					if (FAILED(Quad::mesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&vertexBuffer))) {
						LOG(LOG_PRIORITY::INFO, "Vertex buffer could not be locked!");
						return;
					}
					
					memcpy(vertexBuffer, &vertices.at(0), sizeof(QuadVertex) * vertices.size()); // *NOTE: This may not be the best technique, as memcpy with std::vector is a hazardous operation.
					
					Quad::mesh->UnlockVertexBuffer();
				}
				
				// Fill the index buffer
				{
					void* indexBuffer;
					if (FAILED(Quad::mesh->LockIndexBuffer(0, &indexBuffer))) {
						LOG(LOG_PRIORITY::INFO, "Index buffer could not be locked!");
						return;
					}
					
					memcpy(indexBuffer, &indices.at(0), sizeof(unsigned short) * indices.size()); // *NOTE: This may not be the best technique, as memcpy with std::vector is a hazardous operation.
					
					Quad::mesh->UnlockIndexBuffer();
				}
			}
			
			// Optimize the mesh
			// HACK: I'll write the mesh optimization line when I feel like it! ~Ricky
			// May also want to D3DXConvertMeshSubsetToSingleStrip
			
			
			// Send the mesh to disk for analysis - only when debugging the above algorithms
			//D3DXSaveMeshToX("../quad.x", Quad::mesh, NULL, NULL, NULL, 0, D3DXF_FILEFORMAT_TEXT);
		}
	}
	
	Quad::~Quad() {
		this->RemoveMaterial(0);
		
		this->RemoveTexture(0);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::SetWidth(float size) {
		if (size > 0.0f && size < 2000000.0f) { // sanity check
			this->width = size;
			
			this->UpdateInstanceCache();
			
			return true;
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::SetHeight(float size) {
		if (size > 0.0f && size < 2000000.0f) { // sanity check
			this->height = size;
			
			this->UpdateInstanceCache();
			
			return true;
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::SetOrigin(ORIGIN::TYPE origin) {
		this->originLocation = origin;
		
		this->UpdateInstanceCache();
		
		return true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::SetOrientation(ORIENTATION::TYPE orientation) {
		this->orientation = orientation;
		
		return true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void Quad::Render(LPDIRECT3DDEVICE9 d3d_device) {
		static D3DXMATRIX object_matrix;
		static D3DXMATRIX view_matrix;
		
		if (!(Quad::mesh == nullptr || (this->material == nullptr && this->texture == nullptr))) {
			if (this->material != nullptr) {
				this->material->Apply(d3d_device);
			}
			
			if (this->texture != nullptr) {
				this->texture->Apply(d3d_device);
			}
			
			// *NOTE: Currently assumes normal on -Z, might want to choose based on enum value at some point.
			if (this->orientation == ORIENTATION::CAMERA && this->GetCoordSpace() == CS_WORLD) {
				d3d_device->GetTransform(D3DTS_VIEW, &view_matrix);
				
				// Nullify the positional information so that the matrix can be used later to cancel the camera rotation
				view_matrix._41 = view_matrix._42 = view_matrix._43 = 0.0f;
				D3DXMatrixInverse(&view_matrix, nullptr, &view_matrix);
				
				// Get the current world transformation
				d3d_device->GetTransform(D3DTS_WORLD, &object_matrix);
				
				// Mix in the object transformation
				object_matrix *= this->matrix;
				
				// Take out the camera orientation
				object_matrix *= view_matrix;
				
				// Shove back into world transformation
				d3d_device->SetTransform(D3DTS_WORLD, &object_matrix);
			}
			else {
				d3d_device->MultiplyTransform(D3DTS_WORLD, &this->matrix);
			}
			
			Quad::mesh->DrawSubset(0);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::InsertMaterial(unsigned int index, EnvelopeSPTR envelope) {
		MaterialInterface* newMaterial = nullptr;
		
		// Quad only has one material.
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
			LOG(LOG_PRIORITY::INFO, "Material index out of bounds: Quad only supports 1 material at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::RemoveMaterial(unsigned int index) {
		// Quad only has one material.
		if (index == 0) {
			MaterialsSystemInterface::DestroyMaterial(this->material);
			
			this->material = nullptr;
			
			return true;
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Material index out of bounds: Quad only supports 1 material at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::InsertTexture(unsigned int index, EnvelopeSPTR envelope) {
		TextureInterface* newTexture = nullptr;
		
		// Quad only has one texture.
		if (index == 0) {
			newTexture = TexturesSystemInterface::CreateTexture(envelope, this->GetRenderModule());
			
			// Make sure a new texture was created!
			if (newTexture != nullptr) {
				// Remove any pre-existing texture
				this->RemoveTexture(index); // Return value doesn't need to be tested: if there was no texture, we can continue.  If there was, this will remove it and we can continue.
				
				// Place the new texture
				this->texture = newTexture;
				
				return true;
			}
			else {
				LOG(LOG_PRIORITY::INFO, "Texture could not be created; not replacing existing texture.");
			}
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Texture index out of bounds: Quad only supports 1 texture at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool Quad::RemoveTexture(unsigned int index) {
		// Quad only has one texture.
		if (index == 0) {
			TexturesSystemInterface::DestroyTexture(this->texture);
			
			this->texture = nullptr;
			
			return true;
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Texture index out of bounds: Quad only supports 1 texture at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void Quad::UpdateInstanceCache() {
		float offset_x = 0.0f;
		float offset_y = 0.0f;
		
		// Determine the needed offset
		switch (this->originLocation) {
			case ORIGIN::CENTER:
				offset_x = this->width / -2.0f;
				offset_y = this->height / 2.0f;
			break;
		}
		
		// Update the matrix cache
		this->matrix._11 = this->width;
		this->matrix._22 = this->height;
		this->matrix._41 = offset_x;
		this->matrix._42 = offset_y;
	}
}
