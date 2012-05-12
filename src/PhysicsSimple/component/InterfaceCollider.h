/**
 * \file
 * \author Ricky Curtice
 * \date 20120129
 * \brief Provides a common interface for all colliders.
 *
 * <p>
 * </p>
 *
 */
#pragma once

// Standard Includes

// Library Includes

// Local Includes
#include "../../sharedbase/ComponentInterface.h"
#include "../../sharedbase/Entity_fwd.h"

#include "../Types.h"

// Forward Declarations

// Typedefs and data structures

namespace Physics {
	struct BoundingSphere {
		Vector3D center;
		float radius;
	};
	
	struct AABB {
		Vector3D lowestCorner;
		Vector3D highestCorner;
	};
}

// Classes
namespace Physics {
	class InterfaceCollider : public ComponentInterface {
	public:
		virtual ~InterfaceCollider() {}
		
	public: // API methods
		virtual BoundingSphere GetBoundingSphere() const = 0;
		virtual AABB GetAABB() const = 0;
		
		virtual bool CollideRay(const Vector3D&, const Vector3D&) const = 0;
		
		virtual EntitySPTR GetOwnerEntity() const = 0;
		
	protected:
		InterfaceCollider(EntitySPTR owner, ModuleInterface* module) : ComponentInterface(owner, module) {}
	private:
		InterfaceCollider& operator=(const InterfaceCollider&); ///< Assignment operator disabled for pure interface class
	};
}
