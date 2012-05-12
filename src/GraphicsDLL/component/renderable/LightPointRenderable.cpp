/**
 * \file
 * \author Christopher Schaber
 * \date 20110715
 * \brief Basic light render class and headers
 *
 */

#include <d3dx9.h>
#include <windows.h>

#include "../../../sharedbase/Entity.h"

#include "LightPointRenderable.h"

namespace GraphicsCore {
	LightPointRenderable::LightPointRenderable(EntitySPTR owner, ModuleInterface* renderModule) :
			LightDirectionalRenderable(owner, renderModule) {
		this->SetEnableStatus(true);
		this->SetAmbientColor(D3DCOLOR_XRGB(10, 10, 10));
		this->SetDiffuseColor(D3DCOLOR_XRGB(255, 255, 255));
		this->SetSpecularColor(D3DCOLOR_XRGB(255, 255, 255));
		this->SetRange(1000.0f);
	}

	LightPointRenderable::~LightPointRenderable() {
	}
	
	void LightPointRenderable::Render(LPDIRECT3DDEVICE9 pd3dDevice) {
		if (this->GetEnableStatus()) {
			this->light.Type = D3DLIGHT_POINT;
			this->light.Diffuse = this->GetDiffuseColor();
			this->light.Specular = this->GetSpecularColor();
			this->light.Ambient = this->GetAmbientColor();
			this->light.Position = this->owner->GetWorldPosition();
			this->light.Range = this->GetRange();
			this->light.Attenuation0 = 0.0f;
			this->light.Attenuation1 = 1.0f;
			this->light.Attenuation2 = 0.0f;
			
			int lightIndex = this->GetLightIndex();
			
			if (lightIndex <= 7) {
				pd3dDevice->SetLight(lightIndex, &this->light);
				pd3dDevice->LightEnable(lightIndex, TRUE);
			}
		}

	}
}
