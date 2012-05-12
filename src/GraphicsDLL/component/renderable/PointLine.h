/**
 * \file
 * \author Adam Martin
 * \date 20120120
 * \brief A map drawn of different, regularly-sized, tiles.
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
#include <d3dx9.h>
#include <boost/unordered_map.hpp>
#include <vector>

// Local Includes
#include "../RenderableComponent.h"

// Forward Declarations

// Typedefs

// Constants

// Classes
namespace GraphicsCore {
	class PointLine : public RenderableComponent {
	public:
		PointLine(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9);
		virtual ~PointLine(void);

	private:
		void Render(LPDIRECT3DDEVICE9);
		LPD3DXLINE line;
	};
}
