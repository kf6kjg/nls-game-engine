/**
 * \file
 * \author Ricky Curtice
 * \date 20120129
 * \brief Provides a simple, spherical collider.
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include <boost/lexical_cast.hpp>

#include "ColliderSphere.h"

#include "../../../sharedbase/Entity.h"
#include "../../../sharedbase/EventLogger.h"

namespace Physics {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ColliderSphere::ColliderSphere(EntitySPTR owner, ModuleInterface* module) : InterfaceCollider(owner, module) {
		this->primaryBuffer = ColliderSphereDataPointer(new ColliderSphereData());
		this->secondaryBuffer = ColliderSphereDataPointer(new ColliderSphereData());
		
		this->SetRadius(1.0f);
	}
	
	ColliderSphere::~ColliderSphere() {
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ColliderSphere::SetRadius(float radius) {
		if (radius > 0.0f && radius < 1000000.0f) { // sanity check
			{
				Threading::WriteLock w_lock(this->secondaryBuffer->mutex);
				this->secondaryBuffer->radius = radius;
			}
			
			// Buffer swap here as this is the only writer.
			{
				Threading::WriteLock w1_lock(this->secondaryBuffer->mutex);
				Threading::WriteLock w2_lock(this->primaryBuffer->mutex);
				Threading::Swap<ColliderSphereDataPointer>(this->primaryBuffer, this->secondaryBuffer);
			}
			
			return true;
		}
		
		LOG(LOG_PRIORITY::INFO, "Attempted to set an invalid radius of " + boost::lexical_cast<std::string>(radius));
		
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	BoundingSphere ColliderSphere::GetBoundingSphere() const {
		Threading::ReadLock lock(this->primaryBuffer->mutex);
		
		BoundingSphere sphere = {
			this->owner->GetWorldPosition(),
			this->primaryBuffer->radius
		};
		
		return sphere;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	AABB ColliderSphere::GetAABB() const {
		Threading::ReadLock lock(this->primaryBuffer->mutex);
		
		Vector3D location = this->owner->GetWorldPosition();
		
		AABB aabb;
		
		aabb.lowestCorner.x = location.x - this->primaryBuffer->radius;
		aabb.lowestCorner.y = location.y - this->primaryBuffer->radius;
		aabb.lowestCorner.z = location.z - this->primaryBuffer->radius;
		
		aabb.highestCorner.x = location.x + this->primaryBuffer->radius;
		aabb.highestCorner.y = location.y + this->primaryBuffer->radius;
		aabb.highestCorner.z = location.z + this->primaryBuffer->radius;
		
		return aabb;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ColliderSphere::CollideRay(const Vector3D& source, const Vector3D& direction) const {
		Threading::ReadLock lock(this->primaryBuffer->mutex);
		
		return D3DXSphereBoundProbe(&this->owner->GetWorldPosition(), this->primaryBuffer->radius, &source, &direction) == TRUE;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	EntitySPTR ColliderSphere::GetOwnerEntity() const {
		return this->GetOwner();
	}
}
