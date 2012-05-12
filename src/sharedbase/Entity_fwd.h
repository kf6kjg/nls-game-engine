/**
* \file
* \author Ricky Curtice
* \date 2012-02-25
* \brief A simple header for easy forward declaration of EntitySPTRs
*/
#pragma once

// System Library Includes
#include <memory>

// Library Includes

// Local Includes

// Forward Declarations
class Entity;

// Typedefs

/**
* \brief A typedef for of Entity as a shared_ptr.
*/
typedef std::shared_ptr<Entity> EntitySPTR;

/// Namespaced enumerated type to specify who is assigned the task of deleting an object.
namespace WHO_DELETES {
	/**
	* \brief Who deleted the entity.
	*/
	enum TYPE {
		CALLEE, /**< Callee (the function called) will/has deleted the object. */
		CALLER /**< Caller will/has deleted the the object. */
	};
}
