/**
* \file
* \author Ricky Curtice
* \date 2011-07-31
* \brief Provides some simple helper functions that seem to be missing in DX in regards to quaternions.
*/
#pragma once

// System Library Includes

// Application Library Includes
#include <d3dx9math.h>

// Local Includes

// Forward Declarations

// Typedefs

/**
* \brief Rotates a vector by a quaternion.
*/
void RotateVectorByQuaternion(D3DXVECTOR3* result, const D3DXVECTOR3* vec, const D3DXQUATERNION* rot);

/**
* \brief Rotates a vector by a quaternion.
* \overload
*/
void RotateVectorByQuaternion(D3DXVECTOR3* result, const D3DXQUATERNION* rot);
