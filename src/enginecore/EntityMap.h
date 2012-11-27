/**
* \file
* \author Adam Martin
* \date 2012-04-18
* \brief EntityMap declaration and the comparison class used when mapping string and entities in a std::map.
*/
#pragma once

// System Library Includes
#include <map>
#include <string>

// Application Library Includes

// Local Includes
#include "../sharedbase/Entity_fwd.h"

// Forward Declarations
struct strptrcmp;
class ScriptEngine;

// Typedefs

/**
* \brief Custom compare function struct used by std::map as a predicate to determine ordering.
*/
struct strptrcmp : std::binary_function<const std::string*, const std::string*, bool> {
	/**
	* \brief Comparison function taking 2 string* and using their predefined operator< to determine order.
	* \param[in] lhs, rhs The strings used in the comparison.
	* \return Whether lhs is < rhs.
	*/
	bool operator() (const std::string* lhs, const std::string* rhs) const {
		return *lhs < *rhs;
	}
};

/**
* \brief A typedef for the Entity::name to EntitySPTR map.
* \details Mapping of entity name to entity*. The key is a pointer to the entity's name member, so
* updating entity's name will update the key as well.
*/
typedef std::map<const std::string*, EntitySPTR, strptrcmp> NamedEntityMap;

/**
* \brief        List of all entities stored in a map with their name as the key.
* \details      A wrapper for a mapping of entities and their name (<string*, EntitySPTR>).
* Entities are stored in a map with their name as the key. Entities can be
* added, found, or removed from the list via the wrapper methods.
*/
class EntityMap {
public:
	EntityMap(void) { }
	~EntityMap(void) {	}

	/**
	* \brief Angelscript registration for EntityMap. Additionally calls the registration for
	* Entity as EntityMap requires Angelscript to know about Entity beforehand.
	*/
	void RegisterScriptEngine(ScriptEngine* const);
	
	/**
	* \brief Adds an entity to the list if an entity with the same name doesn't exist.
	*/
	bool AddEntity(EntitySPTR const);
	
	/**
	* \brief Finds an entity with the given name.
	*/
	EntitySPTR FindEntity(const std::string &);
	
	/**
	* \brief Removes an entity with the given name.
	*/
	bool RemoveEntity(const std::string &);
private:
	NamedEntityMap entities; /**< The map of Entity::name to EntitySPTR. */
};
