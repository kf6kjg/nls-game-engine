#pragma once

/**
* \file
* \author Adam Martin
* \date 2011-07-20
* \brief  ComponentInterface class used as a common base for all components.
*
* The use of a common interface allows all components to be acted on in the same manner, and provides
* a guaranteed way of interacting with that component.
*/

// Standard Includes
#include <memory>

// Library Includes

// Local Includes
#include "Entity_fwd.h"

// Forward Declarations
class ModuleInterface;

// Typedefs

class ComponentInterface {
public:
	ComponentInterface(EntitySPTR owner, ModuleInterface* mod);
	virtual ~ComponentInterface(void);

	void SetOwner(EntitySPTR newOwner) { this->owner = newOwner; }
	EntitySPTR GetOwner(void) const { EntitySPTR sptr(this->owner); return sptr; }
	ModuleInterface* GetModule(void) const { return this->module; }
protected:
	EntitySPTR owner;
	ModuleInterface* module;
};
