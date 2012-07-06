/**
 * \file
 * \author Adam Martin
 * \date 2011-07-20
 * \brief Contains the declaration for ComponentInterface.
 */
#pragma once

// System Library Includes

// Application Library Includes

// Local Includes
#include "Entity_fwd.h"

// Forward Declarations
class ModuleInterface;

// Typedefs

/**
 * \brief A common base for all components.
 *
 * The use of a common interface allows all components to be acted on in the same manner, and provides
 * a guaranteed way of interacting with that component.
 */
class ComponentInterface {
public:
	/**
	\brief Sets the owning entity and registers this component with the module it will belong to.
	*/
	ComponentInterface(EntitySPTR owner, ModuleInterface* mod);

	virtual ~ComponentInterface(void);

	/**
	\brief Sets this components owning entity.
	*/
	void SetOwner(EntitySPTR newOwner);

	/**
	\brief Gets this components owning entity.
	*/
	EntitySPTR GetOwner(void) const;
	
	/**
	\brief Gets the module this component belongs to.
	*/
	ModuleInterface* GetModule(void) const;

protected:
	EntitySPTR owner; /**< The owning entity. */
	ModuleInterface* module; /**< The module this component belongs to. */
};
