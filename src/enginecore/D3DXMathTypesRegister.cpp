/**
* \file
* \author Adam Martin
* \date 2012-04-22
* \brief DirectX math structure registration. Used mainly by Entity.
*
*/

#include "ScriptEngine.h"

// System Library Includes

// Application Library Includes
#include <d3dx9math.h>
#include <cassert>

// Local Includes
#include "../sharedbase/QuatMath.h"

// Static class member initialization

// Class methods in the order they are defined within the class header

/**
* \brief The placement factory for D3DXQUATERNION.
* \param[out] address The address where the D3DXQUATERNION will be created.
*/
void QuatFactory(void* address) {
	new(address) D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
}

/**
* \brief The placement factory for D3DXQUATERNION with parameters.
* \param[in] x,y,z,w The starting values for the quaternion.
* \param[out] address The address where the D3DXQUATERNION will be created.
*/
void QuatFactory(const float& x, const float& y, const float& z, const float& w, void* address) {
	new(address) D3DXQUATERNION(x, y, z, w);
}

/**
* \author Ricky Curtice
* \brief Wrapper to convert calling convention from Win32's silly stdcall to a normal cdecl.
* \param[in,out] pOut The resultant vector.
* \param[in] Yaw,Pitch,Roll The components of the rotation to create.
* \return The resultant quaternion rotation.
*/
D3DXQUATERNION* MyQuaternionRotationYawPitchRoll(
  __inout  D3DXQUATERNION *pOut,
  __in     FLOAT Yaw,
  __in     FLOAT Pitch,
  __in     FLOAT Roll
) {
	return D3DXQuaternionRotationYawPitchRoll(pOut, Yaw, Pitch, Roll);
}

/**
* \brief The placement factory for D3DXVECTOR3.
* \param[out] address The address where the D3DXVECTOR3 will be created.
*/
void VectorFactory(void* address) {
	new(address) D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

/**
* \brief The placement factory for D3DXVECTOR3 with parameters.
* \param[in] x,y,z The starting values for the vector.
* \param[out] address The address where the D3DXVECTOR3 will be created.
*/
void VectorFactory(const float& x, const float& y, const float& z, void* address) {
	new(address) D3DXVECTOR3(x, y, z);
}

/**
* \brief The placement factory for D3DXVECTOR3 with parameters.
* \param[in] left The vector to rotate.
* \param[in] right The vector to rotate by.
* \return The resultant vector.
*/
D3DXVECTOR3 VectorApplyRotation(const D3DXVECTOR3& left, const D3DXQUATERNION& right) {
	D3DXVECTOR3 out(left);
	RotateVectorByQuaternion(&out, &right);
	return out;
}

/**
* \brief Adds 2 vectors.
* \param[in] left,right The 2 vectors to add.
* \return The resultant vector.
*/
D3DXVECTOR3 AddVectors(const D3DXVECTOR3& left, const D3DXVECTOR3& right) {
	return left + right;
}

/**
* \brief Subtracts 2 vectors.
* \param[in] left,right The 2 vectors to subtract.
* \return The resultant vector.
*/
D3DXVECTOR3 SubtractVectors(const D3DXVECTOR3& left, const D3DXVECTOR3& right) {
	return left - right;
}

/**
* \brief Gets the magnitude of the vector (squared).
* \param[in] vector The vector to find the magnitude of.
* \return The magnitude of the vector.
*/
float VectorMagnitudeSq(const D3DXVECTOR3& vector) {
	return D3DXVec3LengthSq(&vector);
}

/**
* \brief Gets the magnitude of the vector (square-root).
* \param[in] vector The vector to find the magnitude of.
* \return The square-root of the magnitude of the vector.
*/
float VectorMagnitude(const D3DXVECTOR3& vector) {
	return sqrt(VectorMagnitudeSq(vector));
}

/**
* \brief Returns a normalized vector.
* \param[in] vector The vector to normalize.
* \return The normalized vector.
*/
D3DXVECTOR3 VectorNormalize(const D3DXVECTOR3& vector) {
	D3DXVECTOR3 out(vector);
	D3DXVec3Normalize(&out, &out);
	return out;
}

/**
* \brief The placement factory for D3DXCOLOR.
* \param[out] address The address where the D3DXCOLOR will be created.
*/
void ColorFactory(void* address) {
	new(address) D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

/**
* \brief The placement factory for D3DXCOLOR with parameters.
* \param[in] r,g,b,a The values used to create the color.
* \param[out] address The address where the D3DXCOLOR will be created.
*/
void ColorFactory(const float& r, const float& g, const float& b, const float& a, void* address) {
	new(address) D3DXCOLOR(r, g, b, a);
}

/**
* \param[in] engine A pointer to the Angelscript engine instance. Already checked for nullptr in EntityList::Register
*/
void ScriptEngine::RegisterD3DXMathTypes(asIScriptEngine* const engine) {
	int ret = 0;
	
	// Quaternion rotations
	ret = engine->RegisterObjectType("Rotation", sizeof(D3DXQUATERNION), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert(ret >= 0);
	ret = engine->RegisterObjectBehaviour("Rotation", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(QuatFactory, (void*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
	ret = engine->RegisterObjectBehaviour("Rotation", asBEHAVE_CONSTRUCT, "void f(const float &in, const float &in, const float &in, const float &in)", asFUNCTIONPR(QuatFactory, (const float&, const float&, const float&, const float&, void*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Rotation", "float x", offsetof(D3DXQUATERNION, x)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Rotation", "float y", offsetof(D3DXQUATERNION, y)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Rotation", "float z", offsetof(D3DXQUATERNION, z)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Rotation", "float w", offsetof(D3DXQUATERNION, w)); assert(ret >= 0);
	ret = engine->RegisterObjectMethod("Rotation", "Rotation SetYawPitchRoll(float, float, float)", asFUNCTION(MyQuaternionRotationYawPitchRoll), asCALL_CDECL_OBJFIRST); assert(ret >= 0);
	
	// 3D Vectors
	ret = engine->RegisterObjectType("Vector", sizeof(D3DXVECTOR3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert(ret >= 0);
	ret = engine->RegisterObjectBehaviour("Vector", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(VectorFactory, (void*), void), asCALL_CDECL_OBJFIRST); assert(ret >= 0);
	ret = engine->RegisterObjectBehaviour("Vector", asBEHAVE_CONSTRUCT, "void f(const float &in, const float &in, const float &in)", asFUNCTIONPR(VectorFactory, (const float&, const float&, const float&, void*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Vector", "float x", offsetof(D3DXVECTOR3, x)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Vector", "float y", offsetof(D3DXVECTOR3, y)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Vector", "float z", offsetof(D3DXVECTOR3, z)); assert(ret >= 0);
	ret = engine->RegisterObjectMethod("Vector", "Vector opMul(const Rotation &in) const",   asFUNCTION(VectorApplyRotation), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
	ret = engine->RegisterObjectMethod("Vector", "Vector opMul_r(const Rotation &in) const", asFUNCTION(VectorApplyRotation), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
	ret = engine->RegisterObjectMethod("Vector", "Vector opAdd(const Vector &in) const", asFUNCTION(AddVectors), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
	ret = engine->RegisterObjectMethod("Vector", "Vector opSub(const Vector &in) const", asFUNCTION(SubtractVectors), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
	ret = engine->RegisterObjectMethod("Vector", "float MagnitudeSq(const Vector &in) const", asFUNCTION(VectorMagnitudeSq), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
	ret = engine->RegisterObjectMethod("Vector", "float Magnitude(const Vector &in) const", asFUNCTION(VectorMagnitude), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
	ret = engine->RegisterObjectMethod("Vector", "Vector Normalize(const Vector &in) const", asFUNCTION(VectorNormalize), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
	
	// Color
	ret = engine->RegisterObjectType("Color", sizeof(D3DXCOLOR), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert(ret >= 0);
	ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(ColorFactory, (void*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
	ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f(const float &in, const float &in, const float &in, const float &in)", asFUNCTIONPR(ColorFactory, (const float&, const float&, const float&, const float&, void*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Color", "float r", offsetof(D3DXCOLOR, r)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Color", "float g", offsetof(D3DXCOLOR, g)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Color", "float b", offsetof(D3DXCOLOR, b)); assert(ret >= 0);
	ret = engine->RegisterObjectProperty("Color", "float a", offsetof(D3DXCOLOR, a)); assert(ret >= 0);
}
