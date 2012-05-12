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
#pragma once

// System Library Includes
#include <d3d9.h>

// Application Library Includes

// Local Includes
#include "../RenderableComponent.h"
#include "../../material/MaterialsSystemInterface.h"
#include "../../texture/TexturesSystemInterface.h"

// Forward Declarations
namespace GraphicsCore {
	class MaterialInterface;
	class TextureInterface;
}

// Typedefs


namespace GraphicsCore {
	class RenderableHelicalTube: public RenderableComponent, public MaterialsSystemInterface, public TexturesSystemInterface {
	public:
		RenderableHelicalTube(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9);
		virtual ~RenderableHelicalTube();
		
		bool SetLength(float);
		bool SetRadius(float);
		bool SetPitch(float);
		bool SetSegmentCount(unsigned int);
		
		/// Updates the cached mesh geometry from the basic data.
		void UpdateGeometry();
		
		
	private: // Overridden methods from parent classes
		void Render(LPDIRECT3DDEVICE9);
		
		bool InsertMaterial(unsigned int, EnvelopeSPTR);
		bool RemoveMaterial(unsigned int);
		
		bool InsertTexture(unsigned int, EnvelopeSPTR);
		bool RemoveTexture(unsigned int);
		
		
	private: // Class properties
		float radius, length;
		float pitch;
		unsigned int numberOfSegments;
		
		MaterialInterface* material;
		TextureInterface* texture;
		
		LPDIRECT3DDEVICE9 d3dDevice; ///< For giving updating the geometry a fighting chance.  There may be bettwer ways though.
		
	private: // Class property cache - for performance
		LPD3DXMESH mesh;
	};
}
