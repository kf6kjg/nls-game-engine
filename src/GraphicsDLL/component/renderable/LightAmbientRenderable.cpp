/**
 * \file
 * \author Christopher Schaber
 * \date 20110715
 * \brief Basic ambient light render class and headers
 *
 */

#include <d3dx9.h>
#include <windows.h>

#include "LightAmbientRenderable.h"

namespace GraphicsCore {
	LightAmbientRenderable::LightAmbientRenderable(EntitySPTR owner, ModuleInterface* renderModule) :
			RenderableComponent(owner, renderModule) {
		this->SetEnableStatus(true);
		this->SetAmbientColor(D3DCOLOR_XRGB(255, 255, 255));
	}

	LightAmbientRenderable::~LightAmbientRenderable() {
	}

	void LightAmbientRenderable::Render(LPDIRECT3DDEVICE9 pd3dDevice) {
		if (this->isEnabled) {
			pd3dDevice->SetRenderState(D3DRS_AMBIENT, this->ambientColor);
		}
	}

	D3DXCOLOR LightAmbientRenderable::GetAmbientColor() {
		return this->ambientColor;
	}

	bool LightAmbientRenderable::SetAmbientColor(D3DXCOLOR color) {
		this->ambientColor = color;
		return true;
	}

	bool LightAmbientRenderable::GetEnableStatus() {
		return this->isEnabled;
	}

	void LightAmbientRenderable::SetEnableStatus(bool isEnabled) {
		this->isEnabled = isEnabled;
	}
}

