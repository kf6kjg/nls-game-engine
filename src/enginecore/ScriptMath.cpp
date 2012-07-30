/**
 * \file
 * \author Ricky Curtice
 * \date 2012-05-18
 * \brief 
 * 
 */

#include "ScriptEngine.h"

// System Library Includes
#include <cassert>

// Application Library Includes
#include <angelscript/scriptmath.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Local Includes
#include "../sharedbase/EventLogger.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Helper function prototypes
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void VectorFactory(glm::vec3*);
void VectorFactory(const glm::vec3&, glm::vec3*);
void VectorFactory(const float&, const float&, const float&, glm::vec3*);
glm::vec3 VectorCast(const glm::vec3&);
float Vec3MagSq(const glm::vec3&);
float Vec3DistSq(const glm::vec3&, const glm::vec3&);
void Vec3Normalize(glm::vec3&);
void Vec3Cross(glm::vec3&, const glm::vec3&);
void Vec3ApplyRotation(glm::vec3&, const glm::quat&);
glm::vec3 Vec3ApplyRotationCopy(const glm::vec3&, const glm::quat&);
void Vec3ApplyRotationConj(glm::vec3&, const glm::quat&);
glm::vec3 Vec3ApplyRotationConjCopy(const glm::vec3&, const glm::quat&);

void QuaternionFactory(glm::quat*);
void QuaternionFactory(const glm::quat&, glm::quat*);
void QuaternionFactory(const float&, const float&, const float&, const float&, glm::quat*);
void QuaternionFactory(const float&, const float&, const float&, glm::quat*);
void QuaternionFactory(const glm::vec3&, glm::quat*);
void QuaternionFactory(const glm::vec3&, const float&, glm::quat*);
glm::quat QuaternionCast(const glm::quat&);
glm::vec3 ToEuler(const glm::quat&);
glm::vec3 ToAxis(const glm::quat&);
float ToAngle(const glm::quat&);
void Slerp(glm::quat&, const glm::quat&, const float&);
glm::quat SlerpCopy(const glm::quat&, const glm::quat&, const float&);
float AngleBetween(const glm::quat&, const glm::quat&);
glm::quat operator/(const glm::quat&, const glm::quat&);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// The actual registration function
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief A simple function to cast a string type to a Target type
* \param[in] src The source string.
* \return The lexical_cast of string to Target, or a default constructed instance of Target.
*/
void ScriptEngine::RegisterMathTypes(asIScriptEngine* const as_engine) {
	int ret = 0;

	ret = as_engine->SetDefaultNamespace("Math"); assert(ret >= 0);

	RegisterScriptMath(as_engine);

	// All these types are under the Engine namespace
	ret = as_engine->SetDefaultNamespace("Engine"); assert(ret >= 0);
	
	{ // Register the types
		/*
		*HACK: It would have been better to have been able to
		RegisterTypedef("Vector3","Vector"), but this cannot be done due to a
		limitation of AS: "Currently typedefs can only be registered for built-in
		primitive types."  So instead we have multiple types that are freely
		castable between them, and could cause havoc in the "any" type system...
		*/
		
		// *TODO: Vector2
		
		// *TODO: Vector2i
		
		// Vector3/Vector
		ret = as_engine->RegisterObjectType("Vector",  sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		ret = as_engine->RegisterObjectType("Vector3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		
		// *TODO: Vector3i
		
		// Vector4
		
		// Quaternion/Rotation
		ret = as_engine->RegisterObjectType("Rotation",   sizeof(glm::quat), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		ret = as_engine->RegisterObjectType("Quaternion", sizeof(glm::quat), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		
		// ColorRGB, ColorRGBA/Color
		//ret = as_engine->RegisterObjectType("ColorRGB", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		//ret = as_engine->RegisterObjectType("ColorRGBA", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		//ret = as_engine->RegisterObjectType("Color",     sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
	}
	
	{ // Register the methods and operators
		{ // *TODO: Vector2
		}
		
		{ // *TODO: Vector2i
		}
		
		{ // Vector3
			const char* list[] = {"Vector", "Vector3"};
			for (unsigned int index = 0; index < 2; ++index) {
				std::string type(list[index]);
				
				// General purpose ctors
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(VectorFactory, (glm::vec3*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f(const float &in, const float &in, const float &in)", asFUNCTIONPR(VectorFactory, (const float&, const float&, const float&, glm::vec3*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				
				// Copy ctor
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, ("void f(const " + type + " &in)").c_str(), asFUNCTIONPR(VectorFactory, (const glm::vec3&, glm::vec3*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				
				// Casting operators
				if (std::string(type) != "Vector") {
					ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_IMPLICIT_VALUE_CAST, "Vector f() const",   asFUNCTIONPR(VectorCast, (const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); assert(ret >= 0);
					// Having both implicit and explicit casts between the same types doesn't seem to be supported yet.  However, the implicit seems to also give explict. ~Ricky 20120528, AngelScript 2.23.1
				}
				if (std::string(type) != "Vector3") {
					ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_IMPLICIT_VALUE_CAST, "Vector3 f() const",  asFUNCTIONPR(VectorCast, (const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); assert(ret >= 0);
					// Having both implicit and explicit casts between the same types doesn't seem to be supported yet.  However, the implicit seems to also give explict. ~Ricky 20120528, AngelScript 2.23.1
				}
				
				// Properties
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float x", asOFFSET(glm::vec3, x)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float y", asOFFSET(glm::vec3, y)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float z", asOFFSET(glm::vec3, z)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float r", asOFFSET(glm::vec3, r)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float g", asOFFSET(glm::vec3, g)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float b", asOFFSET(glm::vec3, b)); assert(ret >= 0);
				
				// Methods
				// Note that there is no function in GLM for the square of the magnitude (aka "length") of a vector, hence the custom functions.
				ret = as_engine->RegisterObjectMethod(type.c_str(), "float MagnitudeSq() const", asFUNCTIONPR(Vec3MagSq, (const glm::vec3 &), float), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "float Magnitude() const", asFUNCTIONPR(glm::length, (const glm::vec3 &), float), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("float DistanceSq(const " + type + " &in) const").c_str(), asFUNCTIONPR(Vec3DistSq, (const glm::vec3 &, const glm::vec3 &), float), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("float Distance(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::distance, (const glm::vec3 &, const glm::vec3 &), float), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "void Normalize()", asFUNCTIONPR(Vec3Normalize, (glm::vec3 &), void), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " NormalizedCopy() const").c_str(), asFUNCTIONPR(glm::normalize, (const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("float Dot(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::dot, (const glm::vec3 &, const glm::vec3 &), float), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("void Cross(const " + type + " &in)").c_str(), asFUNCTIONPR(Vec3Cross, (glm::vec3 &, const glm::vec3 &), void), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " CrossCopy(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::cross, (const glm::vec3 &, const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "void ApplyRotation(const Rotation &in) const",   asFUNCTION(Vec3ApplyRotation), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "void ApplyRotation(const Quaternion &in) const", asFUNCTION(Vec3ApplyRotation), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " ApplyRotationCopy(const Rotation &in) const").c_str(),   asFUNCTION(Vec3ApplyRotationCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " ApplyRotationCopy(const Quaternion &in) const").c_str(), asFUNCTION(Vec3ApplyRotationCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "void ApplyRotationInv(const Rotation &in) const",   asFUNCTION(Vec3ApplyRotationConj), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "void ApplyRotationInv(const Quaternion &in) const", asFUNCTION(Vec3ApplyRotationConj), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " ApplyRotationInvCopy(const Rotation &in) const").c_str(),   asFUNCTION(Vec3ApplyRotationConjCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " ApplyRotationInvCopy(const Quaternion &in) const").c_str(), asFUNCTION(Vec3ApplyRotationConjCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				
				// Unary operators
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opNeg(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::detail::operator-, (const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				
				// Binary operators
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opAdd(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::detail::operator+, (const glm::vec3 &, const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opSub(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::detail::operator-, (const glm::vec3 &, const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("float opMul(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::dot, (const glm::vec3 &, const glm::vec3 &), float), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opMod(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::cross, (const glm::vec3 &, const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opMul(const Rotation &in) const").c_str(),   asFUNCTION(Vec3ApplyRotationCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opMul(const Quaternion &in) const").c_str(), asFUNCTION(Vec3ApplyRotationCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opDiv(const Rotation &in) const").c_str(),   asFUNCTION(Vec3ApplyRotationConjCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opDiv(const Quaternion &in) const").c_str(), asFUNCTION(Vec3ApplyRotationConjCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
			}
		}
		
		{ // *TODO: Vector3i
		}
		
		{ // *TODO: Vector4
			
		}
		
		{ // Quaternion
			const char* list[] = {"Rotation", "Quaternion"};
			for (unsigned int index = 0; index < 2; ++index) {
				std::string type(list[index]);
				
				// General purpose ctors
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(QuaternionFactory, (glm::quat*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f(const float &in, const float &in, const float &in, const float &in)", asFUNCTIONPR(QuaternionFactory, (const float&, const float&, const float&, const float&, glm::quat*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				
				// Euler conversion ctors
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f(const float &in, const float &in, const float &in)", asFUNCTIONPR(QuaternionFactory, (const float&, const float&, const float&, glm::quat*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f(const Vector &in)", asFUNCTIONPR(QuaternionFactory, (const glm::vec3&, glm::quat*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				
				// Axis-angle conversion ctor
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f(const Vector &in, const float &in)", asFUNCTIONPR(QuaternionFactory, (const glm::vec3&, const float&, glm::quat*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				
				// Copy ctor
				ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, ("void f(const " + type + " &in)").c_str(), asFUNCTIONPR(QuaternionFactory, (const glm::quat&, glm::quat*), void), asCALL_CDECL_OBJLAST); assert(ret >= 0);
				
				// Casting operators
				if (type != "Rotation") {
					ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_IMPLICIT_VALUE_CAST, "Rotation   f() const", asFUNCTIONPR(QuaternionCast, (const glm::quat&), glm::quat), asCALL_CDECL_OBJLAST); assert(ret >= 0);
					// Having both implicit and explicit casts between the same types doesn't seem to be supported yet.  However, the implicit seems to also give explict. ~Ricky 20120528, AngelScript 2.23.1
				}
				if (type != "Quaternion") {
					ret = as_engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_IMPLICIT_VALUE_CAST, "Quaternion f() const", asFUNCTIONPR(QuaternionCast, (const glm::quat&), glm::quat), asCALL_CDECL_OBJLAST); assert(ret >= 0);
					// Having both implicit and explicit casts between the same types doesn't seem to be supported yet.  However, the implicit seems to also give explict. ~Ricky 20120528, AngelScript 2.23.1
				}
				
				// Properties
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float x", asOFFSET(glm::quat, x)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float y", asOFFSET(glm::quat, y)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float z", asOFFSET(glm::quat, z)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float s", asOFFSET(glm::quat, w)); assert(ret >= 0);
				
				// Methods
				ret = as_engine->RegisterObjectMethod(type.c_str(), "Vector ToEuler() const", asFUNCTION(ToEuler), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "Vector ToAxis() const", asFUNCTION(ToAxis), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), "float ToAngle() const", asFUNCTION(ToAngle), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("void Slerp(const " + type + " &in, const float &in) const").c_str(), asFUNCTION(Slerp), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " SlerpCopy(const " + type + " &in, const float &in) const").c_str(), asFUNCTION(SlerpCopy), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("float AngleTo(const " + type + " &in) const").c_str(), asFUNCTION(AngleBetween), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				
				// Binary operators
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opMul(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::detail::operator*, (const glm::quat &, const glm::quat &), glm::quat), asCALL_CDECL_OBJLAST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opDiv(const " + type + " &in) const").c_str(), asFUNCTIONPR(operator/, (const glm::quat &, const glm::quat &), glm::quat), asCALL_CDECL_OBJLAST); assert( ret >= 0 );
			}
		}
		
	}

	// Clean up after myself
	ret = as_engine->SetDefaultNamespace(""); assert(ret >= 0);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Helper functions
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for a vector.
* \param[out] address The address where the vector will be created.
*/
void VectorFactory(glm::vec3* address) {
	new(address) glm::vec3();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for a vector.
* \param[out] address The address where the vector will be created.
*/
void VectorFactory(const glm::vec3& other, glm::vec3* address) {
	new(address) glm::vec3(other);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for the vector with parameters.
* \param[in] x,y,z The starting values for the vector.
* \param[out] address The address where the vector will be created.
*/
void VectorFactory(const float& x, const float& y, const float& z, glm::vec3* address) {
	new(address) glm::vec3(x, y, z);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief A simple tool to allow casting operators for the equivalent data types in AS.
* \param[in] other The vector to cast from.
* \return A copy of the passed vector.
*/
glm::vec3 VectorCast(const glm::vec3& other) {
	return other; // As the return type isn't a reference or pointer, an implicit copy is done here.
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Calculates the squared magnitude (otherwise known as the "length") of a given vector.
* \param[in] vec The vector to get the squared magnitude from.
* \return The squared magnitude of the vector.
* 
* This is often useful when doing comparisons as there is no need for the added complexity of the sqrt function.
* 
* Note that the math library (GLM currently) does not natively provide this functionality, hence its creation here.
*/
float Vec3MagSq(const glm::vec3& vec) {
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Calculates the squared distance between two vectors.
* \param[in] left The first vector.
* \param[in] right The second vector.
* \return The squared distance between the vectors as magSq(left - right)
* 
* This is often useful when doing comparisons as there is no need for the added complexity of the sqrt function.
* 
* Note that the math library (GLM currently) does not natively provide this functionality, hence its creation here.
*/
float Vec3DistSq(const glm::vec3& left, const glm::vec3& right) {
	return Vec3MagSq(left - right);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Normalizes the passed vector
* \param[in,out] vec The vector to normallize.
*/
void Vec3Normalize(glm::vec3& vec) {
	float mag = glm::length(vec);
	
	vec.x /= mag;
	vec.y /= mag;
	vec.z /= mag;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Applies the cross product operation to the first vector.
* \param[in,out] left The vector to apply the cross product operation to.
* \param[in] right The second vector of the operation.
*/
void Vec3Cross(glm::vec3& left, const glm::vec3& right) {
	float x = left.y * right.z - left.z * right.y;
	float y = left.z * right.x - left.x * right.z;
	float z = left.x * right.y - left.y * right.x;
	
	left.x = x;
	left.y = y;
	left.z = z;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Takes the rotation and applies it to the vector.
* \param[in,out] vec The vector to rotate.
* \param[in] rot The rotation to apply.
*/
void Vec3ApplyRotation(glm::vec3& vec, const glm::quat& rot) {
/*
	//glm::quat result(rot * glm::quat(0.0f, vec.x, vec.y, vec.z) * glm::conjugate(rot));
	
	What follows is mathematically identical to the above, just it involves less intermediate objects and possibly less operations.
*/

	float rw = - rot.x * vec.x - rot.y * vec.y - rot.z * vec.z;
	float rx =   rot.w * vec.x + rot.y * vec.z - rot.z * vec.y;
	float ry =   rot.w * vec.y + rot.z * vec.x - rot.x * vec.z;
	float rz =   rot.w * vec.z + rot.x * vec.y - rot.y * vec.x;
	
	float newx = rx * rot.w - ry * rot.z + rz * rot.y - rw * rot.x;
	float newy = ry * rot.w - rz * rot.x + rx * rot.z - rw * rot.y;
	float newz = rz * rot.w - rx * rot.y + ry * rot.x - rw * rot.z;
	
	vec.x = newx;
	vec.y = newy;
	vec.z = newz;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Takes the rotation and applies it to a copy of the vector.
* \param[in] vec The vector to rotate.
* \param[in] rot The rotation to apply.
* \return A new vector that is the rotated edition of the passed vector.
*/
glm::vec3 Vec3ApplyRotationCopy(const glm::vec3& vec, const glm::quat& rot) {
/*
	//glm::quat result(rot * glm::quat(0.0f, vec.x, vec.y, vec.z) * glm::conjugate(rot));
	//return glm::vec3(result.x, result.y, result.z);
	
	What follows is mathematically identical to the above, just it involves less intermediate objects and possibly less operations.
*/

	float rw = - rot.x * vec.x - rot.y * vec.y - rot.z * vec.z;
	float rx =   rot.w * vec.x + rot.y * vec.z - rot.z * vec.y;
	float ry =   rot.w * vec.y + rot.z * vec.x - rot.x * vec.z;
	float rz =   rot.w * vec.z + rot.x * vec.y - rot.y * vec.x;
	
	float newx = rx * rot.w - ry * rot.z + rz * rot.y - rw * rot.x;
	float newy = ry * rot.w - rz * rot.x + rx * rot.z - rw * rot.y;
	float newz = rz * rot.w - rx * rot.y + ry * rot.x - rw * rot.z;
	
	return glm::vec3(newx, newy, newz);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Takes the conjugate of the rotation and applies it to the vector.
* \param[in,out] vec The vector to rotate.
* \param[in] rot The rotation to conjugate and apply.
*/
void Vec3ApplyRotationConj(glm::vec3& vec, const glm::quat& rot) {
	return Vec3ApplyRotation(vec, glm::conjugate(rot));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Takes the conjugate of the rotation and applies it to a copy of the vector.
* \param[in] vec The vector to rotate.
* \param[in] rot The rotation to conjugate and apply.
* \return A new vector that is the rotated edition of the passed vector.
*/
glm::vec3 Vec3ApplyRotationConjCopy(const glm::vec3& vec, const glm::quat& rot) {
	return Vec3ApplyRotationCopy(vec, glm::conjugate(rot));
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for a quaternion rotation.
* \param[out] address The address where the quaternion rotation will be created.
*/
void QuaternionFactory(glm::quat* address) {
	new(address) glm::quat();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for a quaternion rotation.
* \param[out] address The address where the quaternion rotation will be created.
*/
void QuaternionFactory(const glm::quat& other, glm::quat* address) {
	new(address) glm::quat(other);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for the quaternion rotation with parameters.
* \param[in] x,y,z,s The starting values for the quaternion rotation.
* \param[out] address The address where the quaternion rotation will be created.
*/
void QuaternionFactory(const float& x, const float& y, const float& z, const float& s, glm::quat* address) {
	new(address) glm::quat(s, x, y, z);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for the quaternion rotation with literal Euler parameters.
* \param[in] yaw,pitch,roll The starting values for the quaternion rotation, in radian Z,Y,X values.
* \param[out] address The address where the quaternion rotation will be created.
*/
void QuaternionFactory(const float& yaw, const float& pitch, const float& roll, glm::quat* address) {
	// Using modified (for our coordinate space) formulae from http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
	float heading_2 = yaw / 2.0f;
	float attitude_2 = pitch / 2.0f;
	float bank_2 = roll / 2.0f;
	
	float c1 = cos(heading_2);
	float c2 = cos(attitude_2);
	float c3 = cos(bank_2);
	float s1 = sin(heading_2);
	float s2 = sin(attitude_2);
	float s3 = sin(bank_2);
	
	new(address) glm::quat(
		c1 * c2 * c3 - s1 * s2 * s3, // s,
		s1 * s2 * c3 + c1 * c2 * s3, // x,
		c1 * s2 * c3 - s1 * c2 * s3, // y,
		s1 * c2 * c3 + c1 * s2 * s3  // z
	);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for the quaternion rotation with a Eluer vector parameter.
* \param[in] euler The starting values for the quaternion rotation, in a vector containing radian Z,Y,X rotation values.
* \param[out] address The address where the quaternion rotation will be created.
*/
void QuaternionFactory(const glm::vec3& euler, glm::quat* address) {
	QuaternionFactory(euler.z, euler.y, euler.x, address);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The placement factory for the quaternion rotation with axis-angle parameters.
* \param[in] axis The direction vector.  Note that the magnitude is unimportant, only the direction is of interest.
* \paran[in] angle The angle around the axis.
* \param[out] address The address where the quaternion rotation will be created.
*
* For some reason, GLM's GLX axisAngle function did NOT produce anything even near the expected result.  So this does the math directly.
*/
void QuaternionFactory(const glm::vec3& axis, const float& angle, glm::quat* address) {
	glm::vec3 norm_axis(glm::normalize(axis));
	
	float half_angle = angle * 0.5f;
	float sin_half_angle = sin(half_angle);
	
	new(address) glm::quat(
		cos(half_angle),
		norm_axis.x * sin_half_angle,
		norm_axis.y * sin_half_angle,
		norm_axis.z * sin_half_angle
	);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief A simple tool to allow casting operators for the equivalent data types in AS.
* \param[in] other The quaternion rotation to cast from.
* \return A copy of the passed quaternion rotation.
*/
glm::quat QuaternionCast(const glm::quat& other) {
	return other; // As the return type isn't a reference or pointer, an implicit copy is done here.
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Quaternion to Euler vector conversion.
* \param[in] rot The quaternion to convert.
* \return A vector representing the Euler rotation that matches the given quaternion.
* 
* STUB! As this is a rarely-used function, it is only currently only stubbed in: it returns a zero vector for all inputs.  If needed, please research and implement - the script unit test is already written and only needs to be uncommented.
*/
glm::vec3 ToEuler(const glm::quat& rot) { // *TODO: Implement this function.
	return glm::vec3();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Unit-magnitude Axis part of the Quaternion to Axis-Angle conversion. Use in conjunction with ToAngle.
* \param[in] rot The quaternion to convert.
* \return The rotation axis represented by rot.
*/
glm::vec3 ToAxis(const glm::quat& rot) {
	glm::vec3 axis(rot.x, rot.y, rot.z);
	Vec3Normalize(axis);
	return axis * (float) (1 | -(rot.w < 0.0f));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief The radian-based Angle part of the Quaternion to Axis-Angle conversion. Use in conjunction with ToAxis.
* \param[in] rot The quaternion to convert.
* \return A positive angle <= PI radians - aka the unsigned minimum angle.
*/
float ToAngle(const glm::quat& rot) {
	return acos(abs(rot.w) / sqrt(rot.x * rot.x + rot.y * rot.y + rot.z * rot.z + rot.w * rot.w)) * 2.0f;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Slerp is shorthand for spherical linear interpolation, introduced by Ken Shoemake.  Use this to find a rotation between two rotations - possibly for interpolating smoothly between the rotations.
* \param[in,out] rot The quaternion to interpolate from.  Note this is also the output rotation.
* \param[in] other The quaternion to interpolate to.
* \param[in] mix The amount of interpolation.  If mix == 0.0f, then the resulting rotation is the same as the current rotation; if mix == 1.0f, then the resulting rotation is the same as the other rotation; if mix == 0.5f, then the resulting rotation is halfway between the two.  Extreme values, eg. mix < 0.0f or mix > 1.0f, simply continues the scale past the given rotations.
*
* <p>
* Many thanks to the work done by individuals over in the <a href="http://forums-archive.secondlife.com/54/3b/50692/1.html">Second Life Forum, and especially Strife Onizuka,</a> from whom I converted the body of this code from LSL to C++.
* </p>
*/
void Slerp(glm::quat& rot, const glm::quat& other, const float& mix) {
	glm::quat other_rotated(glm::conjugate(rot) * other);
	
	// Combined the code from the axis-angle placement factory function QuaternionFactory(const glm::vec3&, const float&, glm::quat*) as it provides the Axis-Angle to Quat functionality.
	glm::vec3 norm_axis(glm::normalize(ToAxis(other_rotated)));
	
	float half_angle = mix * ToAngle(other_rotated) * 0.5f;
	float sin_half_angle = sin(half_angle);
	
	glm::quat intermed(
		cos(half_angle),
		norm_axis.x * sin_half_angle,
		norm_axis.y * sin_half_angle,
		norm_axis.z * sin_half_angle
	);
	
	glm::quat result(rot * intermed);
	
	rot.x = result.x;
	rot.y = result.y;
	rot.z = result.z;
	rot.w = result.w;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Slerp is shorthand for spherical linear interpolation, introduced by Ken Shoemake.  Use this to find a rotation between two rotations - possibly for interpolating smoothly between the rotations.
* \param[in,out] rot The quaternion to interpolate from.
* \param[in] other The quaternion to interpolate to.
* \param[in] mix The amount of interpolation.  If mix == 0.0f, then the resulting rotation is the same as the current rotation; if mix == 1.0f, then the resulting rotation is the same as the other rotation; if mix == 0.5f, then the resulting rotation is halfway between the two.  Extreme values, eg. mix < 0.0f or mix > 1.0f, simply continues the scale past the given rotations.
* \return The resulting rotation.
*/
glm::quat SlerpCopy(const glm::quat& rot, const glm::quat& other, const float& mix) {
	glm::quat result(rot);
	Slerp(result, other, mix);
	return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Find the shortest angle between two rotations.
* \param[in] rot The origin rotation.
* \param[in] other The destination rotation.
* \return A positive angle <= PI radians - aka the unsigned minimum angle.
* 
* <p>
* Calcuating the shortest distance angle between two rotations is apparently quite the exercise - primarily due to floating-point rounding errors.
* This function uses the algorithm <a href="https://jira.secondlife.com/browse/SVC-2424?focusedCommentId=321445&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-321445">"AngleTo5" by Moon Metty</a> as it looks very promising.
* </p>
*/
float AngleBetween(const glm::quat& rot, const glm::quat& other) {
	glm::quat r = other / rot; // calculate the rotation between the two arguments as quaternion
	
	float w2 = r.w * r.w; // square of the w-element
	float v2 = r.x * r.x + r.y * r.y + r.z * r.z; // sum of the squares of the v-elements
	
	if (w2 + v2 < 0.000001f) { // one or both arguments are scaled too small to be meaningful, return zero
		return 0.0f;
	}
	
	if (w2 < v2) { // compare the s-component to the v-component
		return 2.0f * acos(sqrt(w2 / (w2 + v2))); // use arccos if the v-component is dominant
	}
	else {
		return 2.0f * asin(sqrt(v2 / (w2 + v2))); // use arcsin if the w-component is dominant
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
* \brief Quaternion division.
* \param[in] q The divisor.
* \param[in] p The dividend.
* \return The result of multiplying p by the conjugate of q.
*/
glm::quat operator/ (const glm::quat& q, const glm::quat& p) {
	return glm::conjugate(q) * p; // Passes test, but is test correct?  SecondLife's LSL rotation division operator might not be the sanest implmentation... (IIRC there are bugs in it at this level...) ~Ricky 20120530
}
