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
#include <set>
#include <string>

// Application Library Includes
#include <d3d9.h>
#include <boost/foreach.hpp>

// Local Includes
#include "../sharedbase/MessageRouter.h"
#include "../sharedbase/PropertyMap.h"
#include "../sharedbase/Envelope.h"

#include "InterfaceUpdateable.h"

#include "GlobalProperties.h"
#include "Messages.h"

// Classes
namespace Physics {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	PhysicsModule::PhysicsModule(PropertyMap* gprops, MessageRouter* msgrouter, EntityList* elist) :
		ModuleInterface(gprops, msgrouter, elist),
		// Initialize member data
		deltaAccumulator(0.0f),
		updateDelay(1.0f / 30.0f) // 30 FPS
	{
		/* * * * * * * * * * * * * * * * * * * * * */
		// Set up the global property defaults
		/* * * * * * * * * * * * * * * * * * * * * */
		this->gprops->SetProperty(GP_PHYSICS_PAUSE, GP_PHYSICS_PAUSE_DEF);
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Register the message handlers
		/* * * * * * * * * * * * * * * * * * * * * */
		this->RegisterMessageHandlers();
		
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Notify loading success
		/* * * * * * * * * * * * * * * * * * * * * */
		{
			EnvelopeSPTR e(new Envelope);
			e->msgid = ACK_MODULE_LOADED;
			this->msgrouter->SendSP(e);
		}
	}
	
	PhysicsModule::~PhysicsModule(void) {
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void PhysicsModule::Update(double dt) {
		this->deltaAccumulator += dt;
		
		if (this->deltaAccumulator >= this->updateDelay) {
			if (!this->gprops->GetPropertyValue<bool>(GP_PHYSICS_PAUSE)) {
				// Run updates
				{
					Threading::ReadLock r_lock(this->updateableObjectsMutex);
					
					BOOST_FOREACH(InterfaceUpdateable* updateable, this->updateableObjects) {
						updateable->Update(this->deltaAccumulator);
					}
				}
				
				this->ReportCollisions();
			}
			
			// Zero the accumulator
			this->deltaAccumulator = 0.0;
		}
	}
}
