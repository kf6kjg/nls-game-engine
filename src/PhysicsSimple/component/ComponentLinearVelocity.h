/**
 * \file
 * \author Ricky Curtice
 * \date 20120129
 * \brief Provides basic linear velocity interpolation.
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include <threading.h>

#include "../../sharedbase/ComponentInterface.h"
#include "../InterfaceUpdateable.h"

#include "../Types.h"

namespace Physics {
	struct ComponentLinearVelocityData {
		/// Object velocity
		Vector3D velocity;
		
		mutable Threading::ReadWriteMutex mutex;
	};
	typedef std::shared_ptr<ComponentLinearVelocityData> ComponentLinearVelocityDataPointer;
	
	struct SwapMarker {
		bool needed;
		mutable Threading::ReadWriteMutex mutex;
	};
}

namespace Physics {
	class ComponentLinearVelocity : public InterfaceUpdateable {
	public:
		ComponentLinearVelocity(EntitySPTR, ModuleInterface*);
		virtual ~ComponentLinearVelocity();
		
	public: // API methods
		bool SetVelocity(Vector3D);
		
	private: // Overridden methods
		void Update(double);
		
	private: // Member methods
		
	private: // Member data
		ComponentLinearVelocityDataPointer primaryBuffer;
		ComponentLinearVelocityDataPointer secondaryBuffer;
		SwapMarker swap;
	};
}
