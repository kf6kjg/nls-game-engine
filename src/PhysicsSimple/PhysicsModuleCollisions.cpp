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
#include <boost/foreach.hpp>

// Local Includes
#include "../sharedbase/MessageRouter.h"
#include "../enginecore/EntityList.h"
#include "../sharedbase/Envelope.h"
#include "component/InterfaceCollider.h"
#include "Messages.h"

// Classes
namespace Physics {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void PhysicsModule::ReportCollisions() const {
		InterfaceCollider* collider_left;
		InterfaceCollider* collider_right;
		BoundingSphere object_left;
		BoundingSphere object_right;
		Vector3D penetration;
		float seperation_sq;
		float penetration_sq;
		
		Threading::ReadLock r_lock(this->collidableObjectsMutex);
		
		// Broadphase collision detection
		for (CollidableListType::iterator collider_iterator_left = this->collidableObjects.begin(); collider_iterator_left != this->collidableObjects.end(); ++collider_iterator_left) {
			CollidableListType::iterator collider_iterator_right = collider_iterator_left;
			for (++collider_iterator_right; collider_iterator_right != this->collidableObjects.end(); ++collider_iterator_right) {
				collider_left = *collider_iterator_left;
				collider_right = *collider_iterator_right;
				
				// It should be impossible to collide an object with itself, but just to make sure...
				if (collider_left != collider_right) {
					
					// Using the sphere test as we currently only have spherical colliders.
					object_left = collider_left->GetBoundingSphere();
					object_right = collider_right->GetBoundingSphere();
					
					penetration = object_right.center - object_left.center;
					
					seperation_sq = D3DXVec3LengthSq(&penetration);
					
					penetration_sq = seperation_sq - (object_right.radius * object_right.radius + object_left.radius * object_left.radius);
					
					if (penetration_sq < 0.0f) {
						// *TODO: do a fine-phase collision, maybe only storing the possible collisions in a set or list of some kind so that the fine phase can be done in later code.
						
						// *HACK: Treating broadphase as fine phase and jumping the gun.
						D3DXVec3Normalize(&penetration, &penetration);
						penetration *= sqrt(-penetration_sq);
						
						EntitySPTR left_ent(collider_left->GetOwnerEntity());
						EntitySPTR right_ent(collider_right->GetOwnerEntity());
						
						std::string left_name;// = this->elist->FindName(left_ent);
						std::string right_name;// = this->elist->FindName(right_ent);
						
						EnvelopeSPTR envelope(new Envelope);
						envelope->msgid = MSG_ENTITY_COLLISION;
						envelope->AddData(left_name);
						envelope->AddData(right_name);
						envelope->AddData(penetration);
						this->msgrouter->SendSP(envelope);
					}
				}
			}
		}
		
	}
}
