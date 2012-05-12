/**
* \file
* \author Ricky Curtice
* \date 2011-12-27
* \brief Provides a common interface for all objects that require updates.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

// Standard Includes

// Library Includes

// Local Includes
#include "../sharedbase/ComponentInterface.h"

// Forward Declarations

// Typedefs

namespace Physics {
	class InterfaceUpdateable : public ComponentInterface {
	public:
		virtual ~InterfaceUpdateable() {}
		
		virtual void Update(double = 0.0) = 0;
	protected:
		InterfaceUpdateable(EntitySPTR owner, ModuleInterface* module) : ComponentInterface(owner, module) {}
	private:
		InterfaceUpdateable& operator=(const InterfaceUpdateable&); ///< Assignment operator disabled for pure interface class
	};
}
