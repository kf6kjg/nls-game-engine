/**
 * \file
 * \author Ricky Curtice
 * \date 20120211
 * \brief Provides the class declaration for the sound engine
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include "SoundModule.h"

// System Library Includes
#include <cassert>
#include <set>
#include <string>

// Application Library Includes
#include <d3d9.h>
#include <boost/foreach.hpp>

// Local Includes
#include "../sharedbase/MessageRouter.h"
#include "../sharedbase/PropertyMap.h"
#include "../sharedbase/Envelope.h"

#include "Messages.h"

// Prototypes
namespace Sound {
	void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION*);
}

// Static data
namespace Sound {
	SoundModule* SoundModule::moduleInstance;
}

// Helpers
namespace Sound {
	// Taken from a comment on: http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
	template <class T>
	inline std::string to_string (const T& t) {
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	template <class T>
	inline std::string to_hex (const T& t) {
		std::stringstream ss;
		ss << std::hex << t;
		return ss.str();
	}
}

// Class Methods
namespace Sound {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	SoundModule::SoundModule(PropertyMap* gprops, MessageRouter* msgrouter, EntityList* elist) :
		ModuleInterface(gprops, msgrouter, elist),
		// Initialize member data
		deltaAccumulator(0.0f),
		updateDelay(1.0f / 30.0f), // 30 FPS
		engine(nullptr),
		engineInitialized(false)
	{
		// Verify status as an instantiable Singleton.
		assert(SoundModule::moduleInstance == NULL);
		SoundModule::moduleInstance = this;
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Register the message handlers
		/* * * * * * * * * * * * * * * * * * * * * */
		this->RegisterMessageHandlers();
		
		{
			HRESULT hr;
			DWORD dwCreationFlags = 0;
			
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			
			std::string errStr;
			
#ifdef _DEBUG
			dwCreationFlags |= XACT_FLAG_API_DEBUG_MODE;
#endif
			
			{
				Threading::WriteLock w_lock(this->engineMutex);
				hr = XACT3CreateEngine( dwCreationFlags, &this->engine );
			}
			if (SUCCEEDED(hr)) {
				assert(this->engine != nullptr);
			}
			else {
				errStr = "Audio failed to initialize. Error code: 0x" + to_hex<HRESULT>(hr);
				
				this->engine = nullptr;
			}
			
			if (errStr.length() > 0) {
				LOG(LOG_PRIORITY::ERR, errStr);
				MessageBox(NULL, (LPCTSTR)errStr.c_str(), TEXT("Audio Error"), MB_OK); 
			}
		}
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Notify loading success
		/* * * * * * * * * * * * * * * * * * * * * */
		{
			EnvelopeSPTR e(new Envelope);
			e->msgid = ACK_MODULE_LOADED;
			this->msgrouter->SendSP(e);
		}
	}
	
	SoundModule::~SoundModule(void) {
		this->engineInitialized = false;
		
		{// Release the sound banks
			Threading::WriteLock w_lock(this->soundBanksMutex);
			
			BOOST_FOREACH(soundbank_pair soundBankPair, this->soundBanks) {
				IXACT3SoundBank* bank = soundBankPair.second;
				Threading::ReadLock r_lock(this->engineMutex);
				bank->Destroy();
			}
			this->soundBanks.clear();
		}
		
		{// Release the wave banks
			Threading::WriteLock w_lock(this->waveBanksMutex);
			
			BOOST_FOREACH(wavebank_pair waveBankPair, this->waveBanks) {
				IXACT3WaveBank* bank = waveBankPair.second;
				Threading::ReadLock r_lock(this->engineMutex);
				bank->Destroy();
			}
			this->waveBanks.clear();
		}
		
		// Shut down XACT
		if (this->engine != nullptr) {
			Threading::WriteLock w_lock(this->engineMutex);
			this->engine->ShutDown();
			this->engine->Release();
			this->engine = nullptr;
		}
		
		CoUninitialize();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::Update(double dt) {
		this->deltaAccumulator += dt;
		
		if (this->deltaAccumulator >= this->updateDelay) {
			
			// It is important to allow XACT to do periodic work by calling engine->DoWork().
			// However this must function be call often enough.  If you call it too infrequently,
			// streaming will suffer and resources will not be managed promptly.  On the other hand
			// if you call it too frequently, it will negatively affect performance. Calling it once
			// per frame is usually a good balance.
			//-----------------------------------------------------------------------------------------
			if (this->engine != nullptr && this->engineInitialized) {
				Threading::ReadLock r_lock(this->engineMutex);
				this->engine->DoWork();
			}
			
			// Zero the accumulator
			this->deltaAccumulator = 0.0;
		}
	}
}
