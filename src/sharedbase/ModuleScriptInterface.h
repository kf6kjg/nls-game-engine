/**
* \file
* \author Ricky Curtice
* \date 2012-04-01
* \brief Interface to define how modules register with scripting.
*
*/
#pragma once

// Standard Includes

// Library Includes
#include <angelscript.h>

// Local Includes

// Forward Declarations

// Typedefs

class ModuleScriptInterface {
public:
	virtual ~ModuleScriptInterface() {}

	/**
	 * Called during initialization of module so that it can provide services to scripting.
	 * 
	 * <p>
	 * Common courtesy suggests that everything be registered under a name that is based on the module's name.  This helps prevent naming conflicts among modules.  It is recommended to use script namespaces to accomplish this, however if that is not an option then using a script class to contain, or even just prepending to the names of, the registered items will accomplish the same goal.  Just don't forget to document!
	 * </p>
	 */
	virtual void Register(asIScriptEngine* const) = 0;
};
