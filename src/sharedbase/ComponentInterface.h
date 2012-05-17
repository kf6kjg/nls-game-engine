/**
 * \file
 * \author Adam Martin
 * \date 2011-07-20
 */
#pragma once

// Standard Includes
#include <memory>

// Library Includes

// Local Includes
#include "Entity_fwd.h"

// Forward Declarations
class ModuleInterface;

// Typedefs

// Classes
/**
 * \brief  ComponentInterface class used as a common base for all components.
 *
 * The use of a common interface allows all components to be acted on in the same manner, and provides
 * a guaranteed way of interacting with that component.
 */
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
