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

#include "EntityList.h"

// System Library Includes
#include <cassert>

// Application Library Includes
#include <angelscript.h>

// Local Includes
#include "../sharedbase/EventLogger.h"
#include "../sharedbase/Entity.h"

// Static class member initialization

// Class methods in the order they are defined within the class header

/**
* \param[in] engine A pointer to the Angelscript engine instance
*/
void EntityList::Register( asIScriptEngine* const engine ) {
	assert(engine != nullptr);
	Entity::Register(engine); // Call to register Entity for Angelscript
	int ret = 0;
	ret = engine->RegisterObjectType("entlist", 0, asOBJ_REF | asOBJ_NOHANDLE); assert(ret >= 0);
	ret = engine->RegisterGlobalProperty("entlist EntityList", this); assert(ret >= 0);
	ret = engine->RegisterObjectMethod("entlist", "bool AddEntity(const Entity)", asMETHODPR(EntityList, AddEntity, (EntitySPTR const), bool), asCALL_THISCALL); assert(ret >= 0);
	ret = engine->RegisterObjectMethod("entlist", "Entity FindEntity(const string &in)", asMETHODPR(EntityList, FindEntity, (const std::string &), EntitySPTR), asCALL_THISCALL); assert(ret >= 0);
	ret = engine->RegisterObjectMethod("entlist", "bool RemoveEntity(const string &in)", asMETHODPR(EntityList, RemoveEntity, (const std::string &), bool), asCALL_THISCALL); assert(ret >= 0);
}

/**
* \param[in] entity A SPTR to the entity being added.
* \return Returns true if the entity was added, or false if there was an issue with entity or an entity
* with the same name already exists.
*/
bool EntityList::AddEntity( EntitySPTR const entity ) {
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
EntitySPTR EntityList::FindEntity( const std::string& name ) {
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
bool EntityList::RemoveEntity( const std::string& name ) {
	NamedEntityMap::const_iterator entitymap_it = this->entities.find(&name);
	if (entitymap_it != this->entities.end()) {
		LOG(LOG_PRIORITY::FLOW, "Removing entity '" + name + "' and deleting it.");

		EntitySPTR ent(entitymap_it->second);

		this->entities.erase(entitymap_it);

		ent->ClearComponents();

		return true;
	}

	LOG(LOG_PRIORITY::CONFIG, "Entity '" + name +  "' not found. Unable to remove and delete.  Did you add it to the EntityList?");
	return false;
}
