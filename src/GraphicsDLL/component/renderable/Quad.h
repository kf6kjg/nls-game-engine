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
#pragma once

// Standard Includes

// Library Includes
#include <d3d9.h>

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

// Constants
namespace GraphicsCore {
	/// Namespaced enumerated type to specify the origin of an object - designed for use by the Quad class.
	namespace ORIGIN {
		enum TYPE {
			UPPER_LEFT = 1, ///< Origin in the upper-left corner of the object.
			CENTER ///< Origin in the center of the object.
		};
	}
	
	/// Namespaced enumerated type to specify the default orientation of an object - designed for use by the Quad class.
	namespace ORIENTATION {
		enum TYPE {
			WORLD = 1, ///< Keeps the orientation assigned to it by the parent entity.
			CAMERA ///< Automatically turns to follow the camera.
		};
	}
}

// Classes
namespace GraphicsCore {
	class Quad : public RenderableComponent, public MaterialsSystemInterface, public TexturesSystemInterface {
	public:
		Quad(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9);
		virtual ~Quad(void);
		
		bool SetWidth(float);
		bool SetHeight(float);
		bool SetOrigin(ORIGIN::TYPE);
		bool SetOrientation(ORIENTATION::TYPE);
		
	private: // Overridden methods from parent classes
		void Render(LPDIRECT3DDEVICE9);
		
		bool InsertMaterial(unsigned int, EnvelopeSPTR);
		bool RemoveMaterial(unsigned int);
		
		bool InsertTexture(unsigned int, EnvelopeSPTR);
		bool RemoveTexture(unsigned int);
		
	private: // Internal utility methods
		void UpdateInstanceCache();
		
	private: // Object property cache - for performance
		static LPD3DXMESH mesh;
		
	private: // Class properties
		float width, height; ///< Scalars for the unit quad
		
		ORIGIN::TYPE originLocation;
		ORIENTATION::TYPE orientation;
		
		MaterialInterface* material;
		TextureInterface* texture;
		
	private: // Instance property cache - for performance
		D3DMATRIX matrix;
	};
}
