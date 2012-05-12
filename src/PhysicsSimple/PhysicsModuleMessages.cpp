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
#include "../sharedbase/MessageRouter.h"
#include "../sharedbase/Envelope.h"
#include "../sharedbase/Entity.h"
#include "../enginecore/EntityList.h"

#include "Messages.h"
#include "Types.h"

#include "component/InterfaceCollider.h"

// Classes
namespace Physics {
	#define SUBSCRIBE_BASIC(msgid, handler)   {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Subscribe((msgid), handler_sptr);}
	#define UNSUBSCRIBE_BASIC(msgid, handler) {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Unsubscribe((msgid), handler_sptr);}
	#define SUBSCRIBE_DIRECTED(msgid, handler)   {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->SubscribeDirected((msgid), handler_sptr);}
	#define UNSUBSCRIBE_DIRECTED(msgid, handler) {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->UnsubscribeDirected((msgid), handler_sptr);}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void PhysicsModule::RegisterMessageHandlers() {
		/* * * * * * * * * * * * * * * * * * * * * */
		// General Message Handlers
		/* * * * * * * * * * * * * * * * * * * * * */
		SUBSCRIBE_BASIC(MSG_MODULE_START, PhysicsModule::HandlerStartup);
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Directed Message Handlers
		/* * * * * * * * * * * * * * * * * * * * * */
		SUBSCRIBE_DIRECTED(MSG_RAY_CAST, PhysicsModule::HandleRayCast);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void PhysicsModule::HandlerStartup(EnvelopeSPTR envelope) {
		bool status = false;
		
		
		// *TODO: actually do a reasonable startup and determeine if this really should be true.
		status = true;
		
		
		// Un-register from the startup message, as there's no need to start again.
		UNSUBSCRIBE_BASIC(MSG_MODULE_START, PhysicsModule::HandlerStartup);
		
		// Send notification that the module is started.
		{
			EnvelopeSPTR e(new Envelope);
			e->msgid = ACK_MODULE_START;
			e->AddData(status);
			this->msgrouter->SendSP(e);
		}
		
		LOG(LOG_PRIORITY::FLOW, "SimplePhysics started.");
		
		return;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	EnvelopeSPTR PhysicsModule::HandleRayCast(EnvelopeSPTR envelope) const {
		unsigned int type = envelope->GetDataUInt(0);
		Vector3D source = envelope->GetDataVector(1);
		Vector3D direction = envelope->GetDataVector(2);
		
		EnvelopeSPTR response(new Envelope());
		response->msgid = 0; // Directed response doesn't need an ID.
		
		EntitySPTR closest_hit_entity(nullptr);
		float closest_hit_dist;
		
		{
			Threading::ReadLock r_lock(this->collidableObjectsMutex);
			
			for (CollidableListType::iterator collider_iterator = this->collidableObjects.begin(); collider_iterator != this->collidableObjects.end(); ++collider_iterator) {
				InterfaceCollider* collider = *collider_iterator;
				
				if (collider->CollideRay(source, direction)) {
					EntitySPTR hit_entity(collider->GetOwnerEntity());
					
					switch (type) {
						case RAY_CAST_NEAREST: {
							float hit_dist = D3DXVec3LengthSq(&(hit_entity->GetWorldPosition() - source));
							
							if (closest_hit_entity.get() == nullptr || hit_dist < closest_hit_dist) {
								// *BUG: since this is based on the center coordinates of the objects, not the ray-object intersection point, there are cases where the object returned will not actually be the closest, just very near the closest.
								
								closest_hit_entity = hit_entity;
								closest_hit_dist = hit_dist;
							}
						}
						break;
						default: { // assumed RAY_CAST_ALL
							std::string hit_entity_name;// = this->elist->FindName(hit_entity);
							
							response->AddData(hit_entity_name);
						}
					}
				}
			}
		}
		
		if (type == RAY_CAST_NEAREST && closest_hit_entity.get() != nullptr) {
			std::string hit_entity_name;// = this->elist->FindName(closest_hit_entity);
			
			response->AddData(hit_entity_name);
		}
		
		return response;
	}
}
