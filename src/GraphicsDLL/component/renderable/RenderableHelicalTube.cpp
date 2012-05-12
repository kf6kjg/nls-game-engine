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

#include "RenderableHelicalTube.h"

// System Library Includes
#include <d3dx9.h>
#include <cmath>
#include <vector>
#include <windows.h>

// Application Library Includes
#include <boost/foreach.hpp>
#include <EngineConfig.h>

// Local Includes
#include "../../../sharedbase/EventLogger.h"
#include "../../material/MaterialInterface.h"
#include "../../texture/TextureInterface.h"

// Forward Declarations

// Typedefs and data structures
namespace GraphicsCore {
	struct HelixVertex {
		D3DXVECTOR3 pos;
		D3DXVECTOR3 norm;
		D3DXVECTOR2 texcoord;
	};
}

// Constants
namespace GraphicsCore {
	static const float PI        = 3.1415926535897932384626433832795f;
	static const float TWO_PI    = 6.283185307179586476925286766559f;
	
	static const unsigned long HELIX_VERTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
}

// Class methods
namespace GraphicsCore {
	
	RenderableHelicalTube::RenderableHelicalTube(EntitySPTR owner, ModuleInterface* renderModule, LPDIRECT3DDEVICE9 d3dDevice) :
			RenderableComponent(owner, renderModule) {
		
		this->d3dDevice = d3dDevice;
		
		this->material = nullptr;
		
		this->texture = nullptr;
		
		this->SetLength(100.0);
		this->SetRadius(1.0);
		this->SetPitch(5.0);
		this->SetSegmentCount(24);
		
		this->mesh = nullptr;
	}
	
	RenderableHelicalTube::~RenderableHelicalTube() {
		this->RemoveMaterial(0);
		
		this->RemoveTexture(0);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::SetLength(float length) {
		if (length > 0.0 && length < 2000000) { // sanity check
			this->length = length;
			
			return true;
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::SetRadius(float radius) {
		if (radius > 0.0 && radius < 2000000) { // sanity check
			this->radius = radius;
			
			return true;
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::SetPitch(float pitch) {
		if (pitch > 0.0 && pitch < 2000000) { // sanity check
			this->pitch = pitch;
			
			return true;
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::SetSegmentCount(unsigned int segments) {
		if (segments < 1000000) { // sanity check
			this->numberOfSegments = segments;
			
			return true;
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void RenderableHelicalTube::Render(LPDIRECT3DDEVICE9 pd3dDevice) {
		if (this->mesh != nullptr) {
			if (this->material != nullptr) {
				this->material->Apply(pd3dDevice);
			}
			
			if (this->texture != nullptr) {
				this->texture->Apply(pd3dDevice);
			}
			
			this->mesh->DrawSubset(0);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::InsertMaterial(unsigned int index, EnvelopeSPTR envelope) {
		MaterialInterface* newMaterial = nullptr;
		
		// Helical tube only has one material.
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
			LOG(LOG_PRIORITY::INFO, "Material index out of bounds: RenderableHelicalTube only supports 1 material at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::RemoveMaterial(unsigned int index) {
		// Helical tube only has one material.
		if (index == 0) {
			MaterialsSystemInterface::DestroyMaterial(this->material);
			
			this->material = nullptr;
			
			return true;
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Material index out of bounds: RenderableHelicalTube only supports 1 material at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::InsertTexture(unsigned int index, EnvelopeSPTR envelope) {
		TextureInterface* newTexture = nullptr;
		
		// Helical tube only has one texture.
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
			LOG(LOG_PRIORITY::INFO, "Texture index out of bounds: RenderableHelicalTube only supports 1 texture at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderableHelicalTube::RemoveTexture(unsigned int index) {
		// Helical tube only has one texture.
		if (index == 0) {
			TexturesSystemInterface::DestroyTexture(this->texture);
			
			this->texture = nullptr;
			
			return true;
		}
		else {
			LOG(LOG_PRIORITY::INFO, "Texture index out of bounds: RenderableHelicalTube only supports 1 texture at index 0." );
		}
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void RenderableHelicalTube::UpdateGeometry() {
		std::vector<HelixVertex> vertices;
		std::vector<unsigned short> indices;
		
		// Create the vertices
		{
			HelixVertex vertex;
			float cur_angle, cur_length;
			
			float angle_step = TWO_PI / this->numberOfSegments;
			float length_step = this->pitch / (TWO_PI * this->numberOfSegments);
			
			float uv_repeat = 1.0f; // *BUG: the texture reversal that the below edge split operation is supposed to fix still occurs if this is higher than 1.
			
			float u_step = uv_repeat / (this->radius * TWO_PI); // Set to keep the texture square realative to the v.
			float v_step = uv_repeat / TWO_PI;
			
			cur_angle = 0.0;
			cur_length = 0.0;
			
			// Go the full length, plus whatever it takes to finish the turn.
			while (cur_length < this->length || cur_angle < TWO_PI) {
				while (cur_angle > TWO_PI) {
					// Correct the angle.
					cur_angle -= TWO_PI;
				}
				vertex.texcoord.x = cur_length * u_step;
				vertex.texcoord.y = cur_angle * v_step;
				
				cur_length += length_step;
				
				vertex.pos.x = this->radius * cos(cur_angle);
				vertex.pos.y = this->radius * sin(cur_angle);
				vertex.pos.z = cur_length;
				
				vertex.norm.x = -vertex.pos.x;
				vertex.norm.y = -vertex.pos.y;
				vertex.norm.z = 0.0;
				D3DXVec3Normalize(&vertex.norm, &vertex.norm);
				
				vertices.push_back(vertex);
				
				// Are we on a boundary vertex?
				if (cur_angle + angle_step >= TWO_PI) {
					// Add a duplicate vertex to create an edge split for the start of the next series with the corrected UV coordinate.
					// This is to work around the texture reversals that happen when the V coordinate goes from near 1.0 to near 0.0 in a single step.
					vertex.texcoord.y -= 1.0f;
					
					vertices.push_back(vertex);
				}
				
				cur_angle += angle_step;
			}
			
			if (vertices.size() < 3) {
				LOG(LOG_PRIORITY::ERR, "Parameters must be wrong because there are less than 3 vertices!");
				return;
			}
		}
		
		// Create the indices for the index buffer
		{
			unsigned short vertex_index = vertices.size();
			
			unsigned short row_count = this->numberOfSegments + 1;
			
			//*BUG: This is generating degenerate extra triangles at the split seam created above by the duplicate vertices.
			while (--vertex_index >= row_count) {
				// Triangle 1
				indices.push_back(vertex_index - row_count);
				indices.push_back(vertex_index - 1);
				indices.push_back(vertex_index);
				
				// Triangle 2
				indices.push_back(vertex_index - row_count + 1);
				indices.push_back(vertex_index - row_count);
				indices.push_back(vertex_index);
			}
		}
		
		// Build the mesh
		{
			// Create the mesh itself
			{
				// *NOTE: I'm not sure if creating a mesh over a previous mesh is correct or if the old one should be deleted somehow first... ~Ricky
				if (FAILED(D3DXCreateMeshFVF(
					indices.size() / 3, // Number of faces
					vertices.size(),    // Number of vertices
					D3DXMESH_MANAGED,   // Options
					HELIX_VERTEX_FVF,   // FVF
					this->d3dDevice,
					&this->mesh
				))) {
					std::string err = "Mesh could not be initialized! " + std::string((this->mesh == nullptr) ? "Mesh was null, not sure what happened..." : "Mesh wasn't null.  Possible issue with needing to delete mesh before recreating.");
					LOG(LOG_PRIORITY::ERR, err);
					return;
				}
				
			}
			
			
			// Fill the vertex buffer
			{
				void* vertexBuffer;
				if (FAILED(this->mesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&vertexBuffer))) {
					LOG(LOG_PRIORITY::ERR, "Vertex buffer could not be locked!");
					return;
				}
				
				memcpy(vertexBuffer, &vertices.at(0), sizeof(HelixVertex) * vertices.size()); // *NOTE: This may not be the best technique, as memcpy with std::vector is a hazardous operation.
				
				this->mesh->UnlockVertexBuffer();
			}
			
			// Fill the index buffer
			{
				void* indexBuffer;
				if (FAILED(this->mesh->LockIndexBuffer(0, &indexBuffer))) {
					LOG(LOG_PRIORITY::ERR, "Index buffer could not be locked!");
					return;
				}
				
				memcpy(indexBuffer, &indices.at(0), sizeof(unsigned short) * indices.size()); // *NOTE: This may not be the best technique, as memcpy with std::vector is a hazardous operation.
				
				this->mesh->UnlockIndexBuffer();
			}
		}
		
		// Optimize the mesh
		// HACK: I'll write the mesh optimization line when I feel like it! ~Ricky
		// May also want to D3DXConvertMeshSubsetToSingleStrip
		
		
		// Send the mesh to disk for analysis - only when debugging the above algorithms
		//D3DXSaveMeshToX("../tube.x", this->mesh, NULL, NULL, NULL, 0, D3DXF_FILEFORMAT_TEXT);
	}
}
