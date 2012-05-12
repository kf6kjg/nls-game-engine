/**
 * \file
 * \author Brett Ramsey
 * \date 20110713
 * \brief Provides basic camera functions for the rendering engine.
 *
 * <p>
 * For there to be a functioning rendering engine, an efficient camera system needs to be used.
 * This is what this file is used for, to provide the basic functionalities of the camera system.
 * </p>
 *
 */
#pragma once

#include "../../sharedbase/ComponentInterface.h"

namespace GraphicsCore {
	class CameraComponent: public ComponentInterface {
	public:
		CameraComponent(EntitySPTR owner, ModuleInterface* renderModule);

		virtual ~CameraComponent();

		float GetFov();
		bool SetFov(float fov);

		float GetFarClipRange();
		float GetNearClipRange();
		bool SetClipRanges(float nearClip, float farClip);

		D3DXQUATERNION GetRotation();
		bool SetRotation(D3DXQUATERNION rot);
		bool SetRotation(float yaw, float pitch, float roll);
		bool SetOffset(D3DXVECTOR3 offset);
		D3DXVECTOR3 GetOffset();
		bool ChangeOffset(D3DXVECTOR3 delta);
		bool ChangeRotation(D3DXQUATERNION delta);
		bool ChangeRotation(float deltaYaw, float deltaPitch, float deltaRoll);

	private:

		// Relative offset
		D3DXVECTOR3 offset;

		// Relative rotation
		D3DXQUATERNION rotation;

		float fov, nearClip, farClip;
	};
}
