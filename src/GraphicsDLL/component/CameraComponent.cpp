/**
 * \file
 * \author Brett Ramsey
 * \date 20110715
 * \brief Provides basic camera functions for the rendering engine.
 *
 * <p>
 * For there to be a functioning rendering engine, an efficient camera system needs to be used.
 * This is what this file is used for, to provide the basic functionalities of the camera system.
 * </p>
 *
 */

#include <d3dx9.h>
#include <windows.h>

#include "CameraComponent.h"

namespace GraphicsCore {
	const float DEFAULT_NEAR_CLIP = 10.0f;
	const float DEFAULT_FAR_CLIP = 1000.0f;
	const float DEFAULT_FOV = D3DXToRadian(60.0f);
	const D3DXVECTOR3 DEFAULT_OFFSET = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DXQUATERNION DEFAULT_ROTATION = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	CameraComponent::CameraComponent(EntitySPTR owner, ModuleInterface* renderModule) :
			ComponentInterface(owner, renderModule) {
		this->SetClipRanges(DEFAULT_NEAR_CLIP, DEFAULT_FAR_CLIP);
		this->SetFov(DEFAULT_FOV);
		this->SetOffset(DEFAULT_OFFSET);
		this->SetRotation(DEFAULT_ROTATION);
	}

	CameraComponent::~CameraComponent() {
	}

	/* * * * * * * * * * */
	/* Field of view
	/* * * * * * * * * * */

	float CameraComponent::GetFov() {
		return this->fov;
	}
	;

	bool CameraComponent::SetFov(float fov) {
		bool result = false;
		
		if (fov > 0.0f && fov <= D3DX_PI) {
			this->fov = fov;
			result = true;
		}
		
		return result;
	}

	/* * * * * * * * * * */
	/* Near and Far clip ranges
	/* * * * * * * * * * */

	float CameraComponent::GetNearClipRange() {
		return this->nearClip;
	}

	float CameraComponent::GetFarClipRange() {
		return this->farClip;
	}

	bool CameraComponent::SetClipRanges(float nearClip, float farClip) {
		bool result = false;
		
		if (nearClip >= 0.0f && farClip >= 0.0f && nearClip < farClip) {
			this->nearClip = nearClip;
			this->farClip = farClip;
			result = true;
		}
		
		return result;
	}

	/* * * * * * * * * * */
	/* Rotation
	/* * * * * * * * * * */

	D3DXQUATERNION CameraComponent::GetRotation() {
		return this->rotation;
	}

	// Rotation by rot
	bool CameraComponent::SetRotation(D3DXQUATERNION rot) {
		D3DXQuaternionNormalize(&rot, &rot);
		this->rotation = rot;
		return true;
	}

	// Rotate by yaw, pitch, and roll in radians
	bool CameraComponent::SetRotation(float yaw, float pitch, float roll) {
		D3DXQUATERNION rot;
		D3DXQuaternionRotationYawPitchRoll(&rot, yaw, pitch, roll);
		this->SetRotation(rot);
		return true;
	}

	bool CameraComponent::SetOffset(D3DXVECTOR3 offset) {
		this->offset = offset;
		return true;
	}

	D3DXVECTOR3 CameraComponent::GetOffset() {
		return this->offset;
	}

	// Offset by delta
	bool CameraComponent::ChangeOffset(D3DXVECTOR3 delta) {
		this->offset += delta;
		return true;
	}

	// Rotation by delta
	bool CameraComponent::ChangeRotation(D3DXQUATERNION delta) {
		D3DXQuaternionNormalize(&delta, &delta);
		this->rotation *= delta;
		return true;
	}

	// Rotate by delta yaw, pitch, and roll in radians
	bool CameraComponent::ChangeRotation(float deltaYaw, float deltaPitch, float deltaRoll) {
		D3DXQUATERNION delta;
		D3DXQuaternionRotationYawPitchRoll(&delta, deltaYaw, deltaPitch, deltaRoll);
		this->ChangeRotation(delta);
		return true;
	}
}
