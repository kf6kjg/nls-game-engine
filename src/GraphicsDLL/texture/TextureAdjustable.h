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

// Standard Includes
#include <string>

// Library Includes
#include <d3dx9tex.h>
#include <threading.h>

// Local Includes
#include "TextureInterface.h"
#include "../../sharedbase/Envelope_fwd.h"

// Forward Declarations
class MessageRouter;

namespace GraphicsCore {
	class RenderModule;
}

// Typedefs/types
namespace GraphicsCore {
	struct TextureAdjustableData {
		D3DXVECTOR2 scale;
		D3DXVECTOR2 translation;
		float sinRotation, cosRotation;
		TextureInterface* texture;
		
		mutable Threading::ReadWriteMutex mutex;
	};
	typedef std::shared_ptr<TextureAdjustableData> TextureAdjustableDataPointer;
	
	struct SwapMarker {
		bool needed;
		mutable Threading::ReadWriteMutex mutex;
	};
}

// Class
namespace GraphicsCore {
	class TextureAdjustable : public TextureInterface {
	public:
		/// Create an adjustable texture wrapping another texture.  When this is called, this class takes ownership of the pointer.
		TextureAdjustable(TextureInterface*, MessageRouter*, RenderModule*);
		~TextureAdjustable();
		
		bool SetMessageId(unsigned int);
		
	private: // Inherited interface function implementations should be private to help the compiler detect potential issues if the interface changes.
		void Apply(LPDIRECT3DDEVICE9);
		
	private:
		void MessageHandler(EnvelopeSPTR);
		
	private:
		TextureAdjustableDataPointer primaryBuffer;
		TextureAdjustableDataPointer secondaryBuffer;
		SwapMarker swap;
		
		D3DXMATRIX currentMatrix;
		
		MessageRouter* messageRouter;
		RenderModule* renderModule;
		
		unsigned int messageTypeId;
	};
}
