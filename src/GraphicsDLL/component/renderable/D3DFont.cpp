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

#include "D3DFont.h"

// System Library Includes

// Application Library Includes
#include <EngineConfig.h>

// Local Includes
#include "../../../sharedbase/Entity.h"
#include "../../../sharedbase/MessageRouter.h"
#include "../../../sharedbase/Envelope.h"
#include "../../Types.h"

// Forward Declarations

// Typedefs and data structures

// Constants

// Class methods
namespace GraphicsCore {
#define SUBSCRIBE_BASIC(msgid, handler)   {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Subscribe((msgid), handler_sptr);}
#define UNSUBSCRIBE_BASIC(msgid, handler) {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->c->Unsubscribe((msgid), handler_sptr);}
#define SUBSCRIBE_DIRECTED(msgid, handler)   {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->SubscribeDirected((msgid), handler_sptr);}
#define UNSUBSCRIBE_DIRECTED(msgid, handler) {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->UnsubscribeDirected((msgid), handler_sptr);}

	D3DFont::D3DFont( EntitySPTR entity, ModuleInterface* mod, LPDIRECT3DDEVICE9 device, MessageRouter* msgrtr, unsigned int width /*= 5*/, unsigned int height /*= 12*/, unsigned int weight /*= FW_NORMAL*/, bool italic /*= false*/ ) : RenderableComponent(entity, mod) {
		this->msgrouter = msgrtr;
		D3DXCreateFont(device, height, width, weight, 1, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,  DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Arial", &this->font);
		D3DXCreateSprite(device, &this->sprite);
		SUBSCRIBE_BASIC(1101, D3DFont::SetText);
		this->text = "";
	}

	D3DFont::~D3DFont( void ) {

	}

	void D3DFont::SetDialogRect( unsigned int offX, unsigned int offY, unsigned int width, unsigned int height )
	{
		this->dialogrect.left = offX;
		this->dialogrect.top = offY;
		this->dialogrect.right = offX + width;
		this->dialogrect.bottom = offY + height;
	}

	void D3DFont::SetText( EnvelopeSPTR env ) {
		this->text = env->GetDataString(0);
	}

	void D3DFont::Render( LPDIRECT3DDEVICE9 dev ) {
		if (this->text != "") {
			Vector3 pos(this->owner->GetWorldPosition());
			
			RECT position;
			position.left = (long) pos.x + this->dialogrect.left;
			position.top = (long) pos.y + this->dialogrect.top;
			position.right = (long) pos.x + this->dialogrect.right;
			position.bottom = (long) pos.y + this->dialogrect.bottom;

			this->sprite->Begin(D3DXSPRITE_ALPHABLEND);
			this->font->DrawTextA(this->sprite, this->text.c_str(), this->text.length(), &position, DT_LEFT|DT_WORDBREAK, 0xFFFFFFFF);
			this->sprite->End();
		}
	}
}
