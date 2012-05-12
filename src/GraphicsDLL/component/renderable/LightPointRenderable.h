/**
 * \file
 * \author Christopher Schaber
 * \date 20110714
 * \brief Basic light render class and headers
 */
#pragma once

#include <d3d9.h>
#include "LightDirectionalRenderable.h"

namespace GraphicsCore {
	class LightPointRenderable: public LightDirectionalRenderable {

	public:
		
		/*Constructor for Light Point*/
		LightPointRenderable(EntitySPTR, ModuleInterface*);

		/*Destructor for Light Point*/
		virtual ~LightPointRenderable();

		void Render(LPDIRECT3DDEVICE9);

	private:
		D3DLIGHT9 light;
	};
}
