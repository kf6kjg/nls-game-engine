/**
* \file
* \author Ricky Curtice
* \date 2012-01-23
* \brief ComponentInterface class used as a common base for all components.
*
* The use of a common interface allows all components to be acted on in the same manner, and provides
* a guaranteed way of interacting with that component.
*/

#include "ComponentInterface.h"

// Standard Includes

// Library Includes

// Local Includes
#include "Entity.h"
#include "EventLogger.h"

// Typedefs

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ComponentInterface::ComponentInterface(EntitySPTR owner, ModuleInterface* mod) : owner(owner), module(mod) {
	this->owner->RegisterComponent(this);
	//LOG(LOG_PRIORITY::FLOW, "Component for entity '" + this->owner->GetName() + "' created.");
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ComponentInterface::~ComponentInterface(void) {
	this->owner->UnregisterComponent(this);
	//LOG(LOG_PRIORITY::FLOW, "Component for entity '" + this->owner->GetName() + "' destroyed.");
}
