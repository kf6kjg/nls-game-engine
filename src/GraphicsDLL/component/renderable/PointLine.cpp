/**
 * \file
 * \author Ricky Curtice
 * \date 20120112
 * \brief A dynamic helical tube mesh
 *
 * <p>
 * </p>
 *
 */

#include "PointLine.h"

// System Library Includes

// Application Library Includes
#include <boost/any.hpp>

// Local Includes
#include "../../../sharedbase/PointList.h"
#include "../../../sharedbase/Entity.h"
#include "TiledMap.h"

// Forward Declarations

// Typedefs and data structures

// Constants

// Class methods
namespace GraphicsCore {
	PointLine::PointLine(EntitySPTR owner, ModuleInterface* renderModule, LPDIRECT3DDEVICE9 d3dDevice) : RenderableComponent(owner, renderModule) {
		D3DXCreateLine(d3dDevice, &this->line);
	}

	PointLine::~PointLine( void ) {

	}

	void PointLine::Render( LPDIRECT3DDEVICE9 ) {
		if (this->owner->properties.PropertyExists("pointlist")) {
			PointList plist = *(this->owner->properties.GetPropertyReference<PointList*>("pointlist"));
			unsigned int viewportWidth = 800, viewportHeight = 600;
			if (this->owner->GetParent() != nullptr) {
				viewportWidth = this->owner->GetParent()->properties.GetPropertyValue<unsigned int>("viewportWidth");
				viewportHeight = this->owner->GetParent()->properties.GetPropertyValue<unsigned int>("viewportHeight");
			}
			auto beginplist = plist.BeginPoint();
			auto endplist = plist.EndPoint();
			D3DXVECTOR2* points = new D3DXVECTOR2[plist.Size()];
			int i = 0;
			int x, y;
			for (auto itr = beginplist; itr != endplist; ++itr, ++i) {
				x = itr->x * GraphicsCore::TILE_WIDTH - this->owner->GetWorldPosition().x + GraphicsCore::TILE_WIDTH/2;
				y = itr->y * GraphicsCore::TILE_HEIGHT - this->owner->GetWorldPosition().y + GraphicsCore::TILE_HEIGHT/2;
				if (((x + GraphicsCore::TILE_WIDTH/2) < 0) && (x > (viewportWidth + (GraphicsCore::TILE_WIDTH/2)))) { // Clamping for inside viewport
					continue;
				}
				if (((y + GraphicsCore::TILE_HEIGHT/2) < 0) && (y > (viewportHeight + (GraphicsCore::TILE_HEIGHT/2)))) { // Clamping for inside viewport
					continue;
				}
				points[i].x = x;
				points[i].y = y;
			}

			this->line->SetWidth(3.0f);
			this->line->Begin();
			this->line->Draw(points, plist.Size(), D3DCOLOR_RGBA(255,0,0,255));
			this->line->End();

			delete[] points;
		}
	}
}
