/**
* \file
* \author Ricky Curtice
* \date 20120129
* \brief Provides a common place to list all "basic" types.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes
#include <d3dx9.h>

// Library Includes

// Typedefs
namespace Physics {
	typedef D3DXVECTOR3 Vector3D;
}

// Constants
namespace Physics {
	const Vector3D ZERO_VECTOR_3D(0.0f, 0.0f, 0.0f);
}
