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

#include <threading.h>

#include "../InterfaceCollider.h"

namespace Physics {
	struct ColliderSphereData {
		float radius;
		mutable Threading::ReadWriteMutex mutex;
	};
	typedef std::shared_ptr<ColliderSphereData> ColliderSphereDataPointer;
}

namespace Physics {
	class ColliderSphere : public InterfaceCollider {
	public:
		ColliderSphere(EntitySPTR, ModuleInterface*);
		virtual ~ColliderSphere();
		
	public: // API methods
		bool SetRadius(float);
		
	private: // overridden methods
		BoundingSphere GetBoundingSphere() const;
		AABB GetAABB() const;
		
		bool CollideRay(const Vector3D&, const Vector3D&) const;
		
		EntitySPTR GetOwnerEntity() const;
		
	private: // member data
		ColliderSphereDataPointer primaryBuffer;
		ColliderSphereDataPointer secondaryBuffer;
	};
}
