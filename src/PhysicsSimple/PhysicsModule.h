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

// System Library Includes
#include <set>
#include <string>

// Application Library Includes
#include <threading.h>

// Local Includes
#include "../sharedbase/ModuleInterface.h"
#include "../sharedbase/Envelope_fwd.h"

// Forward Declarations
/// Namespace to contain every item that is related to the physics engine.
namespace Physics {
	class InterfaceUpdateable;
	class InterfaceCollider;
}

// Typedefs
namespace Physics {
	typedef std::set<ComponentInterface*> ComponentListType;
	typedef std::set<InterfaceCollider*> CollidableListType;
	typedef std::set<InterfaceUpdateable*> UpdateableListType;
}

// DLL Interface
extern "C" {
	__declspec(dllexport) ModuleInterface* ModuleFactory(PropertyMap*, MessageRouter*, EntityList*, EventLogger*);
}

// Classes
namespace Physics {
	class PhysicsModule: public ModuleInterface {
	public:
		
		PhysicsModule(PropertyMap*, MessageRouter*, EntityList*);
		~PhysicsModule(void);
		
	private: // Inherited methods
		void Update(double dt);
		void CreateComponent(std::string, std::map<std::string, boost::any> &, EntitySPTR);
		WHO_DELETES::TYPE RemoveComponent(ComponentInterface*);
		
	private: // Member methods
		/// Registers all the always-active message handlers for this module
		void RegisterMessageHandlers();
		
		/// Checks for object-object collisions
		void ReportCollisions() const;
		
		// Message handlers
		void HandlerStartup(EnvelopeSPTR);
		
		EnvelopeSPTR HandleRayCast(EnvelopeSPTR) const;
		
	private: // Member data
		double deltaAccumulator; ///< Accumulator for the change in time between each call to update
		float updateDelay;
		
		ComponentListType physicsComponents;
		mutable Threading::ReadWriteMutex physicsComponentsMutex;
		
		CollidableListType collidableObjects;
		mutable Threading::ReadWriteMutex collidableObjectsMutex;
		
		UpdateableListType updateableObjects;
		mutable Threading::ReadWriteMutex updateableObjectsMutex;
	};
}
