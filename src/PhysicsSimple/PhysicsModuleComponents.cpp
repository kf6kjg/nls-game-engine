/**
 * \file
 * \author Ricky Curtice
 * \date 20120127
 * \brief Provides the class declaration for the physics engine
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include "PhysicsModule.h"

// System Library Includes

// Application Library Includes

// Local Includes
#include "../sharedbase/Entity.h"
#include "../sharedbase/ComponentInterface.h"
#include "../sharedbase/Envelope.h"

#include "Components.h"
#include "component/ComponentLinearVelocity.h"
#include "component/ASTARMap.h"
#include "component/collider/ColliderSphere.h"
#include "component/GridMover.h"

// Classes
namespace Physics {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	 * For standard, single-parameter setters, provides a quick and easy way to reduce an immense number of lines of code in CreateComponent by centralizing all common operations.
	 */
	template<typename T>
	inline void getAttribute(const std::string type, std::map<std::string, boost::any> &attributes, const std::string key, boost::function<bool(T)> setter) {
		bool status = false;
		
		if (attributes.count(key) > 0) {
			try {
				status = setter(boost::any_cast < T > (attributes[key]));
			}
			catch (const boost::bad_any_cast &) {
				LOG(LOG_PRIORITY::INFO, type + " " + key + " set with invalid data type.");
			}
			
			if (!status) {
				LOG(LOG_PRIORITY::INFO, "Setter for " +  type + " " + key + " failed to set. (returned false)");
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	 * For non-standard setters, provides a relatively quick and easy way to reduce an immense number of lines of code in CreateComponent by centralizing all common operations.
	 */
	template<typename T>
	inline std::pair<bool, T> getAttributeAlt(const std::string type, std::map<std::string, boost::any> &attributes, const std::string key) {
		bool status = false;
		T result;
		if (attributes.count(key) > 0) {
			try {
				result = boost::any_cast < T > (attributes[key]);
				status = true;
			}
			catch (const boost::bad_any_cast &) {
				LOG(LOG_PRIORITY::INFO, type + " " + key + " set with invalid data type.");
			}
		}
		return std::pair<bool, T>(status, result);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Called to create a component of type (type), with the set of attributes (attributes), and with parent entity (entity)
	void PhysicsModule::CreateComponent(std::string type, std::map<std::string, boost::any> &attributes, EntitySPTR entity) {
		/* * * * * * * * * * * * * * * * * * * * * * * * * */
		if (type == LINEAR_VELOCITY) {
			/* * * * * * * * * * * */
			// Create object.
			/* * * * * * * * * * * */
			ComponentLinearVelocity* object = new ComponentLinearVelocity(entity, this);
			
			/* * * * * * * * * * * */
			// Process each parameter.
			/* * * * * * * * * * * */
			getAttribute < LINEAR_VELOCITY__VELOCITY_TYPE > (
				type, attributes, LINEAR_VELOCITY__VELOCITY, std::bind1st(std::mem_fun(&ComponentLinearVelocity::SetVelocity), object)
			);
			
			/* * * * * * * * * * * */
			// Add to list(s).
			/* * * * * * * * * * * */
			// Add to the updateable list since this is an updateable.
			{
				Threading::WriteLock w_lock(this->updateableObjectsMutex);
				this->updateableObjects.insert(object);
			}
			
			// Add to the main list of components.
			{
				Threading::WriteLock w_lock(this->physicsComponentsMutex);
				this->physicsComponents.insert(object);
			}
		}
		/* * * * * * * * * * * * * * * * * * * * * * * * * */
		else if (type == "ASTARMap") {
			ASTARMap* astar = new ASTARMap(entity, this, this->msgrouter);

			getAttribute < EnvelopeSPTR > (
				type, attributes, "mapdata", std::bind1st(std::mem_fun(&ASTARMap::LoadMapData), astar)
				);
			getAttribute < EnvelopeSPTR > (
				type, attributes, "objectdata", std::bind1st(std::mem_fun(&ASTARMap::LoadObjectData), astar)
				);

			astar->FindPath(astar->start);
			
			// Add to the main list of components.
			{
				Threading::WriteLock w_lock(this->physicsComponentsMutex);
				this->physicsComponents.insert(astar);
			}
		}
		else if (type == "GridMover") {
			GridMover* gmove = new GridMover(entity, this, this->msgrouter);

			getAttribute < unsigned int > (
				type, attributes, "scale", std::bind1st(std::mem_fun(&GridMover::SetMovementScale), gmove)
				);

			getAttribute < float > (
				type, attributes, "delay", std::bind1st(std::mem_fun(&GridMover::SetPauseDelay), gmove)
				);

			getAttribute < EnvelopeSPTR > (
				type, attributes, "objectdata", std::bind1st(std::mem_fun(&GridMover::LoadObjectData), gmove)
				);

			EnvelopeSPTR env( boost::any_cast<EnvelopeSPTR>(attributes["path"]) );
			PointList* path = boost::any_cast<PointList*>(env->GetData(0));

			unsigned int handler = boost::any_cast<unsigned int>(attributes["pointenteractionhandler"]);
			gmove->AddPointActionHandler(0, handler);

			handler = boost::any_cast<unsigned int>(attributes["pointleaveactionhandler"]);
			gmove->AddPointActionHandler(1, handler);

			gmove->SetPath(path);
			{
				Threading::WriteLock w_lock(this->updateableObjectsMutex);
				this->updateableObjects.insert(gmove);
			}
			{
				Threading::WriteLock w_lock(this->physicsComponentsMutex);
				this->physicsComponents.insert(gmove);
			}
		}
		/* * * * * * * * * * * * * * * * * * * * * * * * * */
		else if (type == SPHERE_COLLIDER) {
			/* * * * * * * * * * * */
			// Create object.
			/* * * * * * * * * * * */
			ColliderSphere* object = new ColliderSphere(entity, this);
			
			/* * * * * * * * * * * */
			// Process each parameter.
			/* * * * * * * * * * * */
			getAttribute < SPHERE_COLLIDER__RADIUS_TYPE > (
				type, attributes, SPHERE_COLLIDER__RADIUS, std::bind1st(std::mem_fun(&ColliderSphere::SetRadius), object)
			);
			
			/* * * * * * * * * * * */
			// Add to list(s).
			/* * * * * * * * * * * */
			// Add to the collidable list since this is a collidable.
			{
				Threading::WriteLock w_lock(this->collidableObjectsMutex);
				this->collidableObjects.insert(object);
			}
			
			// Add to the main list of components.
			{
				Threading::WriteLock w_lock(this->physicsComponentsMutex);
				this->physicsComponents.insert(object);
			}
		}
		
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	WHO_DELETES::TYPE PhysicsModule::RemoveComponent(ComponentInterface* component) {
		// Remove the requested component
		this->physicsComponents.erase(component);
		
		{
			Threading::UpgradeLock up_lock(this->collidableObjectsMutex);
			
			InterfaceCollider* collider = static_cast<InterfaceCollider*>(component);
			
			CollidableListType::const_iterator it = this->collidableObjects.find(collider);
			if (it != this->collidableObjects.end()) {
				Threading::UpgradeToUniqueLock unique_lock(up_lock);
				
				this->collidableObjects.erase(it);
			}
		}
		
		{
			Threading::UpgradeLock up_lock(this->updateableObjectsMutex);
			
			InterfaceUpdateable* updateable = static_cast<InterfaceUpdateable*>(component);
			
			UpdateableListType::const_iterator it = this->updateableObjects.find(updateable);
			if (it != this->updateableObjects.end()) {
				Threading::UpgradeToUniqueLock unique_lock(up_lock);
				
				this->updateableObjects.erase(it);
			}
		}
		
		return WHO_DELETES::CALLER; // Upstream deletion required
		//return WHO_DELETES::CALLEE; // Deletion accomplished here
	}
}
