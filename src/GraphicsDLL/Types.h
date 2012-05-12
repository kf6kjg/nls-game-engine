/**
* \file
* \author Ricky Curtice
* \date 2012-01-20
* \brief Provides a common place to list all "basic" types.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes

// Library Includes
#include <d3dx9tex.h>

// Typedefs
namespace GraphicsCore {
	typedef D3DXCOLOR Color;
	typedef D3DMATERIAL9 Material;
	typedef LPDIRECT3DTEXTURE9 Texture;
	typedef D3DXVECTOR3 Vector3;
}

// Constants
namespace GraphicsCore {
	const Material BLANK_MATERIAL = {
		Color(1.0f, 1.0f, 1.0f, 1.0f), // Diffuse
		Color(1.0f, 1.0f, 1.0f, 1.0f), // Ambient
		Color(1.0f, 1.0f, 1.0f, 1.0f), // Specular
		Color(0.0f, 0.0f, 0.0f, 1.0f), // Emissive
		1.0f // Power (ie: the sharpness of the specular highlight)
	};
}
