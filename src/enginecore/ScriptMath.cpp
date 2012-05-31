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
		//ret = as_engine->RegisterObjectType("Rotation",   sizeof(glm::), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		//ret = as_engine->RegisterObjectType("Quaternion", sizeof(glm::), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); assert(ret >= 0);
		
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
			char* list[] = {"Vector", "Vector3"};
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
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float x", offsetof(glm::vec3, x)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float y", offsetof(glm::vec3, y)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float z", offsetof(glm::vec3, z)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float r", offsetof(glm::vec3, r)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float g", offsetof(glm::vec3, g)); assert(ret >= 0);
				ret = as_engine->RegisterObjectProperty(type.c_str(), "float b", offsetof(glm::vec3, b)); assert(ret >= 0);
				
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
				
				// Unary operators
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opNeg(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::detail::operator-, (const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				
				// Binary operators
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opAdd(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::detail::operator+, (const glm::vec3 &, const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opSub(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::detail::operator-, (const glm::vec3 &, const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), ("float opMul(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::dot, (const glm::vec3 &, const glm::vec3 &), float), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opMod(const " + type + " &in) const").c_str(), asFUNCTIONPR(glm::cross, (const glm::vec3 &, const glm::vec3 &), glm::vec3), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				//ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opMul(const Rotation &in) const").c_str(),   asFUNCTION(VectorApplyRotation), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
				//ret = as_engine->RegisterObjectMethod(type.c_str(), (type + " opMul_r(const Rotation &in) const").c_str(), asFUNCTION(VectorApplyRotation), asCALL_CDECL_OBJFIRST); assert( ret >= 0 );
			}
		}
		
		{ // *TODO: Vector3i
		}
		
		{ // *TODO: Vector4
			
		}
		
		{ // *TODO: Quaternion
			
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
* \param[inout] vec The vector to normallize.
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
* \param[inout] left The vector to apply the cross product operation to.
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
