/**
* \file
* \author Adam Martin
* \date 20120120
* \brief Wrapper for using D3DXFont
*
* <p>
* 
* </p>
*
*/
#pragma once

// Standard Includes

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

// Local Includes
#include "../RenderableComponent.h"
#include "../../../sharedbase/Envelope_fwd.h"
#include "../../../sharedbase/Entity_fwd.h"

// Forward Declarations
class MessageRouter;

// Typedefs

// Constants

// Classes
namespace GraphicsCore {
	class D3DFont : public RenderableComponent {
	public:
		D3DFont(EntitySPTR, ModuleInterface*, LPDIRECT3DDEVICE9, MessageRouter* msgrtr, unsigned int width = 5, unsigned int height = 12, unsigned int weight = FW_NORMAL, bool italic = false);
		virtual ~D3DFont(void);

		void SetDialogRect( unsigned int offX, unsigned int offY, unsigned int width, unsigned int height );
		void SetText(EnvelopeSPTR env);

	private:
		void Render(LPDIRECT3DDEVICE9);
		LPD3DXFONT font;
		LPD3DXSPRITE sprite; // Sprite object for improved efficiency
		RECT dialogrect;
		unsigned int width, height, weight;
		bool italic;
		std::string text;
		MessageRouter* msgrouter;
	};
}
