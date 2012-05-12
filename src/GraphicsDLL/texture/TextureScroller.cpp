/**
* \file
* \author Ricky Curtice
* \date 2012-01-02
* \brief Provides a simple UV scroller texture implementation.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

#include "TextureScroller.h"

// Standard Includes
#include <string>

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "../RenderModule.h"
#include "TexturesSystemInterface.h"

// Forward Declarations

// Typedefs

// Local Constants
namespace GraphicsCore {
	static const float TWO_PI = 6.283185307179586476925286766559f;
}


namespace GraphicsCore {
	TextureScroller::TextureScroller() {
		this->Init(0.0f, 0.0f, 0.0f);
	}
	
	TextureScroller::TextureScroller(TextureInterface* texture) {
		this->Init(0.0f, 0.0f, 0.0f);
		this->texture = texture;
	}
	
	TextureScroller::~TextureScroller() {
		if (this->texture != nullptr) {
			TexturesSystemInterface::DestroyTexture(this->texture);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureScroller::SetUVVelocity(float u_vel, float v_vel) {
		this->SetUVelocity(u_vel);
		this->SetVVelocity(v_vel);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureScroller::SetUVelocity(float u_vel) {
		this->velocity.u = u_vel;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureScroller::SetVVelocity(float v_vel) {
		this->velocity.v = v_vel;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureScroller::SetAngularVelocity(float angle_vel) {
		while (angle_vel > TWO_PI) {
			angle_vel -= TWO_PI;
		}
		while (angle_vel < -TWO_PI) {
			angle_vel += TWO_PI;
		}
		
		this->velocity.a = angle_vel;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureScroller::Apply(LPDIRECT3DDEVICE9 d3d_device) {
		d3d_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		d3d_device->SetTransform(D3DTS_TEXTURE0, &this->currentMatrix);
		if (this->texture != nullptr) {
			this->texture->Apply(d3d_device);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureScroller::Update(double dt) {
		// Translate the matrix
		this->currentMatrix._31 += (float) (this->velocity.u * dt);
		this->currentMatrix._32 += (float) (this->velocity.v * dt);
		
		// *TODO: the rotation parts of the matrix
		
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureScroller::Init(float u_vel, float v_vel, float angle_vel) {
		this->texture = nullptr;
		
		this->SetUVVelocity(u_vel, v_vel);
		this->SetAngularVelocity(angle_vel);
		
		D3DXMatrixIdentity(&this->currentMatrix);
	}
	
}
