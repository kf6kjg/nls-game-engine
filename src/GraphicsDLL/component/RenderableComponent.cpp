/**
 * \file
 * \author Ricky Curtice
 * \date 20110713
 * \brief Provides the interface class definition for all renderable components
 *
 * <p>
 * Many of the Components used by the Entities in a game engine need to be rendered
 * to the screen in some manner.  The render engine needs to be able to easily
 * differentiate between a component designed to be rendered and one that is not.
 * This class provides that distinction: any class descended from this one is by
 * definition renderable, and wil have its render method called by the render
 * engine at the appropriate time.
 * </p>
 * <p>
 * This class also provides a method by which its descendents can indicate whether
 * they are in screen-space or world-space.  Eg. HUDs would be in screen-space,
 * while a brick wall would be in world-space.
 * </p>
 *
 */

#include <cassert>
#include <d3d9.h>
#include <d3dx9tex.h>

#include "RenderableComponent.h"
#include "../RenderModule.h"

namespace GraphicsCore {
	RenderableComponent::RenderableComponent(EntitySPTR owner, ModuleInterface* core) :
			ComponentInterface(owner, core) {
		space = CS_WORLD; // A reasonable default, as most objects will want to exist in the world, not on the screen.

		// Verify we have an existent RenderModule
		assert(RenderModule::GetRenderModule() != NULL);
		
		// Go get the current RenderModule, and store it away for later use.
		this->renderModule = RenderModule::GetRenderModule();
		
		// Register descendant with RenderModule
		this->renderModule->RegisterComponent(this);
	}
	
	RenderableComponent::~RenderableComponent() {
		// Unregister descendant from stored RenderModule
		this->renderModule->UnregisterComponent(this);
	}
	
	ECoordinateSpace RenderableComponent::GetCoordSpace() {
		return space;
	}
	
	bool RenderableComponent::SetCoordSpace(ECoordinateSpace space) {
		this->space = space;
		return true;
	}

}
