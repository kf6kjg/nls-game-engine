/**
* \file
* \author Ricky Curtice
* \date 2011-12-28
* \brief A single object to represent everything inside the world.
*
* A entity is a link between different components that make it up.
* Anything inside the world is represented by an entity and its components.
* Entity's contain an id and location, rotation, scale that define the most
* common attributes of everything in the world.
*
*/

#include "Entity.h"

// Standard Includes

// Library Includes
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <boost/any.hpp>
#include <boost/foreach.hpp>

// Local Includes
#include "ComponentInterface.h"
#include "ModuleInterface.h"
#include "EventLogger.h"

// Forward Declarations

// Typedefs

// Methods

/**
* \param[in] name The name of the entity.
* \return A shared pointer to the created entity.
*/
EntitySPTR Entity::Factory(const std::string& name) {
	EntitySPTR entity(new Entity(name));
	return entity;
}

/**
* \param[out] address The address where the entity will be created.
*/
void Entity::FactoryAtAddress(void* address) {
	Entity::FactoryAtAddress(address, "");
}

/**
* \param[out] address The address where the entity will be created.
* \param[in] name The name of the entity.
*/
void Entity::FactoryAtAddress(void* address, const std::string& name) {
	new (address) EntitySPTR(new Entity(name));
}

/**
* \param[in] name The name of the entity.
*/
Entity::Entity(const std::string& name) :
	location(glm::vec3(0.0f, 0.0f, 0.0f)),
	rotation(glm::fquat(0.0f, 0.0f, 0.0f, 1.0f)),
	scale(1.0f),
	name(name),
	parent(nullptr)
	{
	LOG(LOG_PRIORITY::FLOW, "Entity '" + this->GetName() + "' created.");
}

Entity::~Entity() {
	LOG(LOG_PRIORITY::FLOW, "Entity '" + this->GetName() + "' destroyed.");
	
	this->ClearComponents();
	
	// Break from parent object.
	this->SetParent(nullptr);
}

/**
* \param[in] new_parent New parent entity.
*/
void Entity::SetParent(EntitySPTR new_parent) {
	//Threading::WriteLock w_lock(this->parentMutex);
	
	if (new_parent.get() != nullptr) {
		// Verify that a recursive relationship has not been established
		bool status = true;
		Entity* new_parent_p = new_parent.get();
		Entity* parent = this->parent.get();
		
		// Linearized recursive scan up the family tree.
		while (parent != nullptr && (status = (parent != new_parent_p))) {
			parent = parent->GetParent().get();
		}
		
		if (status) {
			this->parent = new_parent;
		}
		else {
			LOG(LOG_PRIORITY::CONFIG, "ERROR: Recursive parenting NOT allowed.  Attempted to parent '" + this->GetName() + "' to '" + new_parent->GetName() + "'.");
		}
	}
	else {
		this->parent.reset();
	}
}

/**
* \return The entity's parent.
*/
EntitySPTR Entity::GetParent(void) const {
	//Threading::ReadLock r_lock(this->parentMutex);
	
	EntitySPTR ent(this->parent);
	
	return ent;
}

/**
* \return The absolute psoition of the entity.
*/
glm::vec3 Entity::GetWorldPosition(void) const {
	glm::vec3 result = this->location;
	Entity* entity = this->GetParent().get();
	glm::vec3 scaled, rotated;
	
	
	/* N=3
	push
		[n-2].translate
		[n-2].scale
		[n-2].rotate
		push
			[n-1].translate
			[n-1].scale
			[n-1].rotate
			push
				[n-0].translate
			pop
		pop
	pop
	
	(([n-0].translate) * [n-1].scale * [n-1].rotate + [n-1].translate) * [n-2].scale * [n-2].rotate + [n-2].translate
	*/
	
	while (entity != nullptr) {
		//result = (result * entity->scale) * entity->rotation + entity->location;
		
		scaled = (result * entity->scale);
		
		rotated = glm::rotate(entity->rotation, scaled);
		
		result = rotated + entity->location;
		
		entity = entity->GetParent().get();
	}
	
	return result;
}

/**
* \return The absolute rotation of the entity.
*/
glm::fquat Entity::GetWorldRotation(void) const {
	glm::fquat result = this->rotation;
	Entity* entity = this->GetParent().get();
	
	// Linearized recursive accumulation of rotations.
	while (entity != nullptr) {
		result = entity->rotation * result; // Remember, quat multiplication is NOT commutative!
		
		entity = entity->GetParent().get();
	}
	
	return result;
}

/**
* \return The absolute scale of the entity.
*/
float Entity::GetWorldScale(void) const {
	float result = this->scale;
	Entity* entity = this->GetParent().get();
	
	// Linearized recursive accumulation of scales.
	while (entity != nullptr) {
		result *= entity->scale;
		
		entity = entity->GetParent().get();
	}
	
	return result;
}

/**
* \param[in] x, y, z The absolute position of the entity (separate components).
*/
void Entity::SetPosition(float x, float y, float z) {
	this->location = glm::vec3(x, y, z);
}


/**
* \param[in] rot The absolute rotation of the entity (combined components).
*/
void Entity::SetRotation(glm::fquat rot) {
	this->rotation = glm::normalize(rot);
	// *NOTE: If the normalize's sqrt call needs to be optimized away, there is a way (supposedly) to normalize quats without sqrt.
}

/**
* \param[in] yaw, pitch, roll The absolute rotation of the entity (separate components).
*/
void Entity::SetRotation(float yaw, float pitch, float roll) {
	glm::fquat rot(glm::vec3(yaw, pitch, roll));
	this->SetRotation(rot);
}

/**
* \param[in] scale The scale of the entity.
*/
void Entity::SetScale(float scale) {
	this->scale = scale;
}

/**
* \param[in] delta The relative amount to change to position (combined components).
*/
void Entity::ChangePosition(glm::vec3 delta) {
	this->location += delta;
}

/**
* \param[in] delta The relative amount to change to rotation (combined components).
*/
void Entity::ChangeRotation(glm::fquat delta) {
	this->rotation = this->rotation * glm::normalize(delta);
	// *NOTE: If the normalize's sqrt call needs to be optimized away, there is a way (supposedly) to normalize quats without sqrt.
}

/**
* \param[in] deltaYaw, deltaPitch, deltaRoll The absolute rotation of the entity (separate components).
*/
void Entity::ChangeRotation(float deltaYaw, float deltaPitch, float deltaRoll) {
	glm::fquat delta(glm::vec3(deltaYaw, deltaPitch, deltaRoll));
	this->ChangeRotation(delta);
}

/**
* \param[in] delta The relative amount to change to scale.
*/
void Entity::ChangeScale(float delta) {
	this->scale *= delta;
}

///*
//* param[in] name The new name for entity.
//*/
//void Entity::SetName(const std::string& name) {
//	this->name = name;
//}

/**
* \return The name of the entity.
*/
const std::string& Entity::GetName() const {
	return this->name;
}

void Entity::ClearComponents() {
	static std::set<ComponentInterface*> components;
	static ModuleInterface* module;

	{
		//Threading::ReadLock r_lock(this->componentsMutex);
		components = this->components;
	}

	// Remove the child components - each will unregister itself upon dtor.
	BOOST_FOREACH(ComponentInterface* component, components) {
		module = component->GetModule();
		if (module != nullptr) {
			if (module->RemoveComponent(component) == WHO_DELETES::CALLER) {
				delete component;
			}
		}
	}

	this->components.clear();
}

/**
* \param[in] component Component to add to the entity's set of components.
* \return True if the component was added.
*/
bool Entity::RegisterComponent(ComponentInterface* component) {
	//Threading::WriteLock w_lock(this->componentsMutex);
	
	if (this->components.find(component) == this->components.end()) {
		this->components.insert(component);
		return true;
	}
	
	return false;
}

/**
* \param[in] component Component to remove from the entity's set of components.
* \return True if the component was removed.
*/
bool Entity::UnregisterComponent(ComponentInterface* component) {
	//Threading::WriteLock w_lock(this->componentsMutex);

	if (this->components.find(component) != this->components.end()) {
		this->components.erase(component);
		return true;
	}

	return false;
}

/**
* \param[in] entity The removed entity
* \return True if the passed in entity was the parent of this entity.
*/
bool Entity::NotifyEntityRemoval(EntitySPTR entity) {
	if (entity.get() == nullptr) {
		return false;
	}
	if (entity == this->GetParent()) {
		this->SetParent(nullptr);
		
		return true;
	}
	
	return false;
}
