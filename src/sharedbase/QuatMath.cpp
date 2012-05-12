/**
* \file
* \author Ricky Curtice
* \date 2011-07-31
* \brief Provides some simple helper functions that seem to be missing in DX in regards to quaternions.
*/

#include "QuatMath.h"

// System Library Includes

// Application Library Includes

// Local Includes

// Static class member initialization

// Class methods in the order they are defined within the class header

/**
* \param[out] result The resultant vector.
* \param[in] vec The vector to rotate.
* \param[in] rot The rotation to apply to result.
*/
void RotateVectorByQuaternion(D3DXVECTOR3* result, const D3DXVECTOR3* vec, const D3DXQUATERNION* rot) {
	D3DXQUATERNION rotInverse;
	D3DXQUATERNION pureQuat (vec->x, vec->y, vec->z, 0.0f);
	D3DXQuaternionInverse(&rotInverse, rot);
	
	pureQuat = rotInverse * pureQuat * *rot;
	
	result->x = pureQuat.x;
	result->y = pureQuat.y;
	result->z = pureQuat.z;
}

/**
* \param[in,out] result The vector to rotate, also the resultant vector.
* \param[in] rot The rotation to apply to result.
*/
void RotateVectorByQuaternion(D3DXVECTOR3* result, const D3DXQUATERNION* rot) {
	RotateVectorByQuaternion(result, result, rot);
}
