/**
 * \file
 * \author Ricky Curtice
 * \date 20110713
 * \brief Provides the interface class declaration for all renderable components
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
 * Team: Team A - Graphics
 * Justin Avignone, Ricky Curtice, Jordan Lawyer, Brett Ramsey, Chris Schaber
 */
#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>

#include "../../sharedbase/ComponentInterface.h"

namespace GraphicsCore {
	class RenderModule;
	
	enum ECoordinateSpace {
		CS_SCREEN,
		CS_WORLD
	};

	class RenderableComponent: public ComponentInterface {
	public:
		RenderableComponent(EntitySPTR owner, ModuleInterface* core);
		virtual ~RenderableComponent();

		/**
		 * Draws objects either in the world or on the screen,
		 * depending on the status of the current coordinate space.
		 * The object's location and rotation in said space is already provided,
		 * just draw at <0,0,0> and a 0 rotation - unless you need to correct for model problems.
		 */
		virtual void Render(LPDIRECT3DDEVICE9) = 0;

		ECoordinateSpace GetCoordSpace();
		bool SetCoordSpace(ECoordinateSpace);

	protected:
		inline RenderModule* GetRenderModule() const {
			return this->renderModule;
		}

	private:
		/**
		 * Declares in what space this object should be rendered.
		 */
		ECoordinateSpace space;

		/**
		 * Quick-reference handle back to the renderModule this belongs to, primarily for unregistering upon deletion.
		 */
		RenderModule* renderModule;
	};
}
