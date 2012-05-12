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

// System Library Includes
#include <set>
#include <string>
#include <xact3.h>

// Application Library Includes
#include <threading.h>

// Local Includes
#include "../sharedbase/ModuleInterface.h"
#include "../sharedbase/Envelope_fwd.h"

// Forward Declarations

// Typedefs
/// Namespace to contain every item that is related to the sound engine.
namespace Sound {
	typedef std::map<std::string,  IXACT3WaveBank*>  wavebank_map;
	typedef std::pair<std::string, IXACT3WaveBank*>  wavebank_pair;
	typedef std::map<std::string,  IXACT3SoundBank*> soundbank_map;
	typedef std::pair<std::string, IXACT3SoundBank*> soundbank_pair;
}

// DLL Interface
extern "C" {
	__declspec(dllexport) ModuleInterface* ModuleFactory(PropertyMap*, MessageRouter*, EntityList*, EventLogger*);
}

// Classes
namespace Sound {
	class SoundModule: public ModuleInterface {
		friend void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION*);
		
	public:
		static SoundModule* GetModuleInstance() {
			return SoundModule::moduleInstance;
		}
		
	private:
		static SoundModule* moduleInstance;
	
	public:
		SoundModule(PropertyMap*, MessageRouter*, EntityList*);
		~SoundModule(void);
		
		XACTINDEX GetCueIndex(const std::string& bank, const std::string& cueName);
		
	private: // Inherited methods
		void Update(double dt);
		void CreateComponent(std::string, std::map<std::string, boost::any> &, EntitySPTR) {}
		WHO_DELETES::TYPE RemoveComponent(ComponentInterface*) { return WHO_DELETES::CALLEE; }
		
	private: // Member methods
		/// Registers all the always-active message handlers for this module
		void RegisterMessageHandlers();
		
		// Message handlers
		void HandlerStartup(EnvelopeSPTR);
		
		void HandlerBasic(EnvelopeSPTR);
		
		EnvelopeSPTR HandlerDirectedGetCueIndex(EnvelopeSPTR);
		
		// XACT operations
		void LoadXACTSettingsBuffer(std::string file);
		void LoadInMemoryWavebank(std::string file);
		void LoadStreamingWavebank(std::string file);
		void LoadSoundbank(std::string file);
		
		void RegisterNotification(XACTNOTIFICATIONTYPE, BYTE, PVOID = NULL);
		
	private: // Member data
		double deltaAccumulator; ///< Accumulator for the change in time between each call to update
		float updateDelay;
		
		Threading::ReadWriteMutex engineMutex;
		bool engineInitialized;
		IXACT3Engine* engine;
		
		Threading::ReadWriteMutex waveBanksMutex;
		wavebank_map waveBanks;
		
		Threading::ReadWriteMutex soundBanksMutex;
		soundbank_map soundBanks;
		
		Threading::ReadWriteMutex preparedCuesMutex;
		std::map< std::pair<std::string, int>, IXACT3Cue*> preparedCues; // key is {bank, cueIndex}
	};
}
