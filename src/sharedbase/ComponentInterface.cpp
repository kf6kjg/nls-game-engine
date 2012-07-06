/**
* \file
* \author Ricky Curtice
* \date 2012-01-23
* \brief ComponentInterface class definitions.
*/

#include "ComponentInterface.h"

// System Library Includes

// Application Library Includes

// Local Includes
#include "Entity.h"

// Typedefs

/**
\param[in] owner The entity that owns this component.
\param[in] mod The module this component belongs to (the one that created it most likely).
*/
ComponentInterface::ComponentInterface(EntitySPTR owner, ModuleInterface* mod) : owner(owner), module(mod) {
	this->owner->RegisterComponent(this);
};

ComponentInterface::~ComponentInterface(void) {
	this->owner->UnregisterComponent(this);
}

/**
\param[in] newOner The new entity that owns this component.
*/
void ComponentInterface::SetOwner( EntitySPTR newOwner ) {
	this->owner = newOwner;
}
/**
\return newOner The entity that owns this component.
*/
EntitySPTR ComponentInterface::GetOwner( void ) const  {
	EntitySPTR sptr(this->owner); return sptr;
}

/**
\return newOner The module that this component belongs to.
*/
ModuleInterface* ComponentInterface::GetModule( void ) const {
	return this->module;
}
