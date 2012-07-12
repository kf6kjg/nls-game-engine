/**
* \file
* \author       Adam Martin
* \date         2012-04-18
* \brief        List of all entities stored in a map with their name as the key.
* \details      A wrapper for a mapping of entities and their name (<string*, EntitySPTR>).
* Entities are stored in a map with their name as the key. Entities can be
* added, found, or removed from the list via the wrapper methods.
* 
*/

#include "EntityMap.h"

// System Library Includes
#include <cassert>

// Application Library Includes

// Local Includes
#include "../sharedbase/EventLogger.h"
#include "../sharedbase/Entity.h"

// Static class member initialization

// Class methods in the order they are defined within the class header

/**
* \param[in] as_engine A pointer to the Angelscript engine instance
*/
void EntityMap::Register( asIScriptEngine* const as_engine ) {
	assert(as_engine != nullptr);
	Entity::Register(as_engine); // Call to register Entity for Angelscript
	int ret = 0;
	ret = as_engine->SetDefaultNamespace("Engine"); assert(ret >= 0);
	
	ret = as_engine->RegisterObjectType("EntityMap", 0, asOBJ_REF | asOBJ_NOHANDLE); assert(ret >= 0);
	ret = as_engine->RegisterGlobalProperty("EntityMap gEntMap", this); assert(ret >= 0); // *TODO: Remove this global property and make EntityMap a full-on type similar to the spec's GenericMap type, just specialized ONLY for the Entity type.  When done this method should become static so main() doesn't have to instanciate the class.
	ret = as_engine->RegisterObjectMethod("EntityMap", "bool AddEntity(const Entity)", asMETHODPR(EntityMap, AddEntity, (EntitySPTR const), bool), asCALL_THISCALL); assert(ret >= 0);
	ret = as_engine->RegisterObjectMethod("EntityMap", "Entity FindEntity(const string &in)", asMETHODPR(EntityMap, FindEntity, (const std::string &), EntitySPTR), asCALL_THISCALL); assert(ret >= 0);
	ret = as_engine->RegisterObjectMethod("EntityMap", "bool RemoveEntity(const string &in)", asMETHODPR(EntityMap, RemoveEntity, (const std::string &), bool), asCALL_THISCALL); assert(ret >= 0);
	
	// Clean up after myself
	ret = as_engine->SetDefaultNamespace("Engine"); assert(ret >= 0);
}

/**
* \param[in] entity A SPTR to the entity being added.
* \return Returns true if the entity was added, or false if there was an issue with entity or an entity
* with the same name already exists.
*/
bool EntityMap::AddEntity( EntitySPTR const entity ) {
	if (entity.get() != nullptr) {
		if (entity->GetName() == "" || entity->GetName().length() <= 0) {
			LOG(LOG_PRIORITY::CONFIG, "ERROR: Entity must have a name to be added!");
			return false;
		}

		if (this->entities.find(&entity->GetName()) == this->entities.end()) {
			EntitySPTR ent(entity);
			this->entities[&ent->GetName()] = ent;

			LOG(LOG_PRIORITY::FLOW, "Adding entity: '" + entity->GetName() + "'");
			return true;
		}
		else {
			LOG(LOG_PRIORITY::CONFIG, "ERROR: Entity '" + entity->GetName() + "', already exists.");
			return false;
		}
	}
	else {
		LOG(LOG_PRIORITY::CONFIG, "ERROR: Adding a null entity is an invalid operation.");

		return false;
	}
}

/**
* \param[in] name The name of the entity to find.
* \return A SPTR to the entity, or a nullptr if no entity exists with the given name.
*/
EntitySPTR EntityMap::FindEntity( const std::string& name ) {
	NamedEntityMap::const_iterator entitymap_it = this->entities.find(&name);
	if (entitymap_it != this->entities.end()) {
		EntitySPTR found_entity(entitymap_it->second);

		return found_entity;
	}
	else {
		LOG(LOG_PRIORITY::CONFIG, "Entity '" + name +  "' not found.  Did you forget to name it?  Or have you already removed it?");
		return nullptr;
	}
}

/**
* \param[in] name The name of the entity to remove.
* \return True if the entity was removed. False if no entity with the given name exists.
*/
bool EntityMap::RemoveEntity( const std::string& name ) {
	NamedEntityMap::const_iterator entitymap_it = this->entities.find(&name);
	if (entitymap_it != this->entities.end()) {
		LOG(LOG_PRIORITY::FLOW, "Removing entity '" + name + "' and deleting it.");

		EntitySPTR ent(entitymap_it->second);

		this->entities.erase(entitymap_it);

		ent->ClearComponents();

		return true;
	}

	LOG(LOG_PRIORITY::CONFIG, "Entity '" + name +  "' not found. Unable to remove and delete.  Did you add it to the EntityMap?");
	return false;
}
