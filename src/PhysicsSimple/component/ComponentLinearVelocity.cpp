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

#include "ComponentLinearVelocity.h"

#include "../../sharedbase/Entity.h"

namespace Physics {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ComponentLinearVelocity::ComponentLinearVelocity(EntitySPTR owner, ModuleInterface* module) :
		InterfaceUpdateable(owner, module),
		primaryBuffer(ComponentLinearVelocityDataPointer(new ComponentLinearVelocityData())),
		secondaryBuffer(ComponentLinearVelocityDataPointer(new ComponentLinearVelocityData()))
	{
		this->swap.needed = false;
		this->SetVelocity(ZERO_VECTOR_3D);
	}
	
	ComponentLinearVelocity::~ComponentLinearVelocity() {
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool ComponentLinearVelocity::SetVelocity(Vector3D velocity) {
		// Low-level copy just to be sure
		{
			Threading::WriteLock w_lock(this->secondaryBuffer->mutex);
			this->secondaryBuffer->velocity.x = velocity.x;
			this->secondaryBuffer->velocity.y = velocity.y;
			this->secondaryBuffer->velocity.z = velocity.z;
		}
		
		{
			Threading::WriteLock w_lock(this->swap.mutex);
			this->swap.needed = true;
		}
		
		return true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void ComponentLinearVelocity::Update(double dt) {
		// Attempt buffer swap here if needed.  If it fails, try again next time.
		{
			Threading::UpgradeLock swap_lock(this->swap.mutex);
			if (this->swap.needed) {
				Threading::UpgradeToUniqueLock unique_lock(swap_lock);
				this->swap.needed = false;
				
				Threading::WriteLock w1_lock(this->secondaryBuffer->mutex);
				Threading::WriteLock w2_lock(this->primaryBuffer->mutex);
				Threading::Swap<ComponentLinearVelocityDataPointer>(this->primaryBuffer, this->secondaryBuffer);
			}
		}
		
		if (this->owner.get() != nullptr) {
			Threading::ReadLock lock(this->primaryBuffer->mutex);
			
			this->owner->location += this->primaryBuffer->velocity * (float) dt;
		}
	}
}
