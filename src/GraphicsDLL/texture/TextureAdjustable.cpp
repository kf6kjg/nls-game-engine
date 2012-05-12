/**
* \file
* \author Ricky Curtice
* \date 2012-02-01
* \brief Provides the ability to adjust the transformation of a texture on receipt of a message.
*
* *TODO: A full description of the purpose of the file.
*
*/
#pragma once

#include "TextureAdjustable.h"

// Standard Includes
#include <string>

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "../../sharedbase/MessageRouter.h"
#include "../../sharedbase/Envelope.h"
#include "../RenderModule.h"
#include "../Textures.h"
#include "TexturesSystemInterface.h"

// Forward Declarations

// Typedefs
namespace GraphicsCore {
	#define SUBSCRIBE_BASIC(msgid, handler)   {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->messageRouter->Subscribe((msgid), handler_sptr);}
	#define UNSUBSCRIBE_BASIC(msgid, handler) {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->messageRouter->Unsubscribe((msgid), handler_sptr);}
	#define SUBSCRIBE_DIRECTED(msgid, handler)   {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->messageRouter->SubscribeDirected((msgid), handler_sptr);}
	#define UNSUBSCRIBE_DIRECTED(msgid, handler) {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->messageRouter->UnsubscribeDirected((msgid), handler_sptr);}
}

// Local Constants
namespace GraphicsCore {
	// 0 is left for an undefined entry.
	static const unsigned int TEXTURE_OFFSET = 1; // Followed by: float, float
	static const unsigned int TEXTURE_SCALE = 2; // non-uniform scaling.  Followed by: float, float
	static const unsigned int TEXTURE_SCALE_UNIFORM = 3; // Followed by: float
	static const unsigned int TEXTURE_ROTATION = 4; // Followed by: float
	static const unsigned int TEXTURE_FILE = 5; // Followed by: string
}


namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	TextureAdjustable::TextureAdjustable(TextureInterface* texture, MessageRouter* messageRouter, RenderModule* renderModule) :
		primaryBuffer(TextureAdjustableDataPointer(new TextureAdjustableData())),
		secondaryBuffer(TextureAdjustableDataPointer(new TextureAdjustableData())),
		messageRouter(messageRouter),
		renderModule(renderModule),
		messageTypeId(0)
	{
		
		this->primaryBuffer->scale.x = this->primaryBuffer->scale.y = 1.0f;
		this->primaryBuffer->translation.x = this->primaryBuffer->translation.y = 0.0f;
		this->primaryBuffer->sinRotation = 0.0f;
		this->primaryBuffer->cosRotation = 1.0f;
		this->primaryBuffer->texture = texture;
		
		this->secondaryBuffer->scale.x = this->secondaryBuffer->scale.y = 1.0f;
		this->secondaryBuffer->translation.x = this->secondaryBuffer->translation.y = 0.0f;
		this->secondaryBuffer->sinRotation = 0.0f;
		this->secondaryBuffer->cosRotation = 1.0f;
		this->secondaryBuffer->texture = nullptr;
		
		this->swap.needed = false;
		
		D3DXMatrixIdentity(&this->currentMatrix);
	}
	
	TextureAdjustable::~TextureAdjustable() {
		UNSUBSCRIBE_BASIC(this->messageTypeId, TextureAdjustable::MessageHandler);
		
		if (this->secondaryBuffer->texture != nullptr && this->secondaryBuffer->texture != this->primaryBuffer->texture) {
			TexturesSystemInterface::DestroyTexture(this->secondaryBuffer->texture);
		}
		
		if (this->primaryBuffer->texture != nullptr) {
			TexturesSystemInterface::DestroyTexture(this->primaryBuffer->texture);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool TextureAdjustable::SetMessageId(unsigned int message_type_id) {
		UNSUBSCRIBE_BASIC(this->messageTypeId, TextureAdjustable::MessageHandler);
		SUBSCRIBE_BASIC(message_type_id, TextureAdjustable::MessageHandler);
		
		this->messageTypeId = message_type_id;
		
		return true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureAdjustable::Apply(LPDIRECT3DDEVICE9 d3d_device) {
		// Attempt buffer swap here if needed.
		{
			Threading::UpgradeLock swap_lock(this->swap.mutex);
			if (this->swap.needed) {
				{
					Threading::UpgradeToUniqueLock unique_lock(swap_lock);
					this->swap.needed = false;
				}
				
				{
					Threading::WriteLock w1_lock(this->secondaryBuffer->mutex);
					Threading::WriteLock w2_lock(this->primaryBuffer->mutex);
					Threading::Swap<TextureAdjustableDataPointer>(this->primaryBuffer, this->secondaryBuffer);
				}
				
				// Recompute matrix.  Not all positions need changing as they never are modified from when set by the Identity operation in the constructor.
				this->currentMatrix(0, 0) = this->primaryBuffer->scale.x * this->primaryBuffer->cosRotation;
				this->currentMatrix(0, 1) = this->primaryBuffer->scale.x * this->primaryBuffer->sinRotation;
				
				this->currentMatrix(1, 0) = this->primaryBuffer->scale.y * -this->primaryBuffer->sinRotation;
				this->currentMatrix(1, 1) = this->primaryBuffer->scale.y * this->primaryBuffer->cosRotation;
				
				this->currentMatrix(2, 0) = this->primaryBuffer->translation.x;
				this->currentMatrix(2, 1) = this->primaryBuffer->translation.y;
			}
		}
		
		Threading::ReadLock r_lock(this->primaryBuffer->mutex);
		if (this->primaryBuffer->texture != nullptr) {
			d3d_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
			d3d_device->SetTransform(D3DTS_TEXTURE0, &this->currentMatrix);
			
			this->primaryBuffer->texture->Apply(d3d_device);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void TextureAdjustable::MessageHandler(EnvelopeSPTR envelope) {
		if (envelope->GetCount()) {
			bool safe = true; // if the parsing or lexing fails, this will be set false and the changes will not be committed.
			
			{
				Threading::WriteLock w_lock(this->secondaryBuffer->mutex);
				
				// Copy the primary buffer
				{
					Threading::ReadLock r_lock(this->primaryBuffer->mutex);
					this->secondaryBuffer->scale = this->primaryBuffer->scale;
					this->secondaryBuffer->translation = this->primaryBuffer->translation;
					this->secondaryBuffer->sinRotation = this->primaryBuffer->sinRotation;
					this->secondaryBuffer->cosRotation = this->primaryBuffer->cosRotation;
					
					if (this->secondaryBuffer->texture != nullptr && this->secondaryBuffer->texture != this->primaryBuffer->texture) {
						TexturesSystemInterface::DestroyTexture(this->secondaryBuffer->texture);
					}
					
					this->secondaryBuffer->texture = this->primaryBuffer->texture;
				}
				
				// Parse the envelope
				{
					// Syntax: {uint code, properties in a fixed amount per code...}, ...
					unsigned int param_type;
					unsigned int count = envelope->GetCount();
					for (unsigned int index = 0; index < count && safe; ++index) {
						param_type = envelope->GetDataUInt(index);
						switch (param_type) {
							case TEXTURE_OFFSET: {
								// Get the values
								this->secondaryBuffer->translation.x = envelope->GetDataFloat(index + 1);
								this->secondaryBuffer->translation.y = envelope->GetDataFloat(index + 2);
								
								// Consume the positions used.
								index += 2;
							}
							break;
							case TEXTURE_SCALE: {
								// Get the values
								this->secondaryBuffer->scale.x = envelope->GetDataFloat(index + 1);
								this->secondaryBuffer->scale.y = envelope->GetDataFloat(index + 2);
								
								// Consume the positions used.
								index += 2;
							}
							break;
							case TEXTURE_SCALE_UNIFORM: {
								// Get the values
								this->secondaryBuffer->scale.x = this->secondaryBuffer->scale.y = envelope->GetDataFloat(index + 1);
								
								// Consume the positions used.
								index += 1;
							}
							break;
							case TEXTURE_ROTATION: {
								// Get the values
								float angle = envelope->GetDataFloat(index + 1);
								
								this->secondaryBuffer->sinRotation = sin(angle);
								this->secondaryBuffer->cosRotation = cos(angle);
								
								// Consume the positions used.
								index += 1;
							}
							break;
							case TEXTURE_FILE: {
								// Get the values
								std::string file = envelope->GetDataString(index + 1);
								
								EnvelopeSPTR envelope(new Envelope());
								envelope->msgid = ID_TEXTURE_BASIC;
								envelope->AddData(ID_TEXTURE_PARAM_FILE);
								envelope->AddData(file);
								
								// At this point the texture in the secondary buffer is either a nullptr, or a pointer to the same texture that is in the primary buffer.
								// Either way, we can just override it.
								this->secondaryBuffer->texture = TexturesSystemInterface::CreateTexture(envelope, renderModule);
								
								// Consume the positions used.
								index += 1;
							}
							break;
							default: {
								LOG(LOG_PRIORITY::INFO, "Unknown texture parameter type sent: " + boost::lexical_cast < std::string > (param_type));
								safe = false; // Stop processing the loop: to proceed would be just garbage due to unknown types.
							}
						}
					}
				}
			}
			
			if (safe) {
				// Mark the buffers for swapping.
				Threading::WriteLock w_lock(this->swap.mutex);
				this->swap.needed = true;
			}
		}
	}
}
