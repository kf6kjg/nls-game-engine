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
#pragma once

// Standard Includes

// Library Includes
//#include <d3d9.h>

// Local Includes
#include "../RenderableComponent.h"
#include "../../material/MaterialsSystemInterface.h"

// Forward Declarations

// Typedefs

// Constants

// Classes
namespace GraphicsCore {
	class CubeRenderable: public RenderableComponent, public MaterialsSystemInterface {
	public:
		CubeRenderable(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9);
		virtual ~CubeRenderable();

	private: // Overridden/implemented methods of parent classes
		void Render(LPDIRECT3DDEVICE9);

		bool InsertMaterial(unsigned int, EnvelopeSPTR);
		bool RemoveMaterial(unsigned int);

	private:
		LPD3DXMESH mesh;
		MaterialInterface* material;
	};
}
