/**
 * \file
 * \author Christopher Schaber
 * \date 20110714
 * \brief Basic ambient light render class and headers
 */
#pragma once

#include <d3d9.h>
#include "../RenderableComponent.h"

namespace GraphicsCore {
	class LightAmbientRenderable: public RenderableComponent {
	public:
		LightAmbientRenderable(EntitySPTR, ModuleInterface*);
		virtual ~LightAmbientRenderable();

		/**
		 Get the Light Ambient Color
		 **/
		D3DXCOLOR GetAmbientColor();

		/**
		 Set the Ambient Color of the Light
		 **/
		bool SetAmbientColor(D3DXCOLOR);

		void Render(LPDIRECT3DDEVICE9);

		void SetEnableStatus(bool);

		bool GetEnableStatus(void);

	private:

		D3DXCOLOR ambientColor;

		bool isEnabled;
	};
}
