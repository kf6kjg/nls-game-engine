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
#include "../../../sharedbase/QuatMath.h"

#include "LightDirectionalRenderable.h"

namespace GraphicsCore {
	int LightDirectionalRenderable::lightCount = 0;

	LightDirectionalRenderable::LightDirectionalRenderable(EntitySPTR owner, ModuleInterface* renderModule) :
			LightAmbientRenderable(owner, renderModule) {
		this->SetEnableStatus(true);
		this->SetAmbientColor(D3DCOLOR_XRGB(10, 10, 10));
		this->SetDiffuseColor(D3DCOLOR_XRGB(255, 255, 255));
		this->SetSpecularColor(D3DCOLOR_XRGB(255, 255, 255));
		this->SetRange(sqrtf(FLT_MAX));
		this->SetDirection(D3DXVECTOR3(0.0f, -1.0f, 0.0f)); // Facing downwards by default.
		
		this->lightIndex = LightDirectionalRenderable::lightCount++;
	}

	LightDirectionalRenderable::~LightDirectionalRenderable() {
	}

	/** Get the Light Diffuse Color */
	D3DXCOLOR LightDirectionalRenderable::GetDiffuseColor() {
		return this->diffuseColor;
	}

	/** Set the Diffuse Color of the Light */
	bool LightDirectionalRenderable::SetDiffuseColor(D3DXCOLOR diffuseColor) {
		this->diffuseColor = diffuseColor;
		return true;
	}

	/** Get the Light Specular Color */
	D3DXCOLOR LightDirectionalRenderable::GetSpecularColor() {
		return this->specularColor;
	}

	/** Set the Specular Color of the Light */
	bool LightDirectionalRenderable::SetSpecularColor(D3DXCOLOR specularColor) {
		this->specularColor = specularColor;
		return true;
	}

	D3DXVECTOR3 LightDirectionalRenderable::GetDirection() {
		return this->direction;
	}

	bool LightDirectionalRenderable::SetDirection(D3DXVECTOR3 direction) {
		D3DXVec3Normalize(&(this->direction), &direction);
		return true;
	}

	float LightDirectionalRenderable::GetRange(void) {
		return this->range;
	}

	bool LightDirectionalRenderable::SetRange(float range) {
		this->range = range;
		return true;
	}

	void LightDirectionalRenderable::Render(LPDIRECT3DDEVICE9 pd3dDevice) {
		if (this->GetEnableStatus()) {
			D3DLIGHT9 light;
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Diffuse = this->GetDiffuseColor();
			light.Specular = this->GetSpecularColor();
			light.Ambient = this->GetAmbientColor();
			D3DXVECTOR3 dir = this->GetDirection();
			RotateVectorByQuaternion(&dir, &this->owner->GetWorldRotation());
			light.Direction = dir;
			light.Range = this->GetRange();
			light.Attenuation0 = light.Attenuation1 = light.Attenuation2 = 0.0f;
			
			int lightIndex = this->GetLightIndex();
			
			if (lightIndex <= 7) {
				pd3dDevice->SetLight(lightIndex, &light);
				pd3dDevice->LightEnable(lightIndex, TRUE);
			}
		}

	}
}
