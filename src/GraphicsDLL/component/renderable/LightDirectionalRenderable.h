/**
 * \file
 * \author Christopher Schaber
 * \date 20110714
 * \brief Basic light render class and headers
 */
#pragma once

#include <d3d9.h>
#include "LightAmbientRenderable.h"

namespace GraphicsCore {
	
	class LightDirectionalRenderable: public LightAmbientRenderable {
		
	public:
		LightDirectionalRenderable(EntitySPTR, ModuleInterface*);
		virtual ~LightDirectionalRenderable();

		/**Get the Light Diffuse Color*/
		D3DXCOLOR GetDiffuseColor();

		/**Set the Diffuse Color of the Light*/
		bool SetDiffuseColor(D3DXCOLOR);

		/**Get the Light Specular Color*/
		D3DXCOLOR GetSpecularColor();

		/**Set the Specular Color of the Light*/
		bool SetSpecularColor(D3DXCOLOR);

		float GetRange(void);
		bool SetRange(float);

		void Render(LPDIRECT3DDEVICE9);

		D3DXVECTOR3 GetDirection(void);

		bool SetDirection(D3DXVECTOR3);

		inline int GetLightIndex() const {
			return this->lightIndex;
		}

	protected:
		static int lightCount; // Should be private and using a getter and setter...

	private:

		D3DXCOLOR diffuseColor;
		D3DXCOLOR specularColor;
		D3DXCOLOR ambientColor;
		D3DXVECTOR3 direction;

		int lightIndex;

		float range;
	};
}
