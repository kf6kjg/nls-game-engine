/**
 * \file
 * \author Adam Martin
 * \date July 20 2011
 */
#pragma once

// Standard Includes
#include <string>
#include <map>

// Library Includes
#include <boost/any.hpp>
#include <boost/function.hpp>

// Local Includes
#include "Entity_fwd.h"

// Forward Declarations
class ComponentInterface;
class asIScriptEngine;

// Typedefs

// Classes
/**
 * \brief ModuleInterface class used as a common base for all modules.
 */
class ModuleInterface { // All systems derive from this
public:
	virtual ~ModuleInterface(void) {};

	virtual void Update(double dt) = 0; // Called each game update with change in time (dt) in milliseconds since last update

	virtual WHO_DELETES::TYPE RemoveComponent(ComponentInterface*) = 0;

	virtual void Register(asIScriptEngine* const) { } // Called to register the given module with Angelscript.

protected:
	ModuleInterface() {};
};
