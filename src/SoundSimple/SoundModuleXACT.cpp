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
#include <set>
#include <string>

// Application Library Includes
#include <d3d9.h>
#include <boost/foreach.hpp>
#include <EngineConfig.h>

// Local Includes
#include "../sharedbase/MessageRouter.h"
#include "../sharedbase/PropertyMap.h"
#include "../sharedbase/Envelope.h"

#include "Messages.h"

// Prototypes
namespace Sound {
	void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION*);
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
		ss << hex << t;
		return ss.str();
	}
}

// Class Methods
namespace Sound {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	XACTINDEX SoundModule::GetCueIndex(const std::string& bank, const std::string& cueName) {
		XACTINDEX index = XACTINDEX_INVALID;
		
		if (bank.length() > 0 && cueName.length() > 0) {
			Threading::ReadLock r_lock(this->soundBanksMutex);
			soundbank_map::iterator sbIter = this->soundBanks.find(bank);
			if (sbIter != this->soundBanks.end()) {
				IXACT3SoundBank* soundBank = sbIter->second;
				
				Threading::ReadLock r_lock(this->engineMutex);
				index = soundBank->GetCueIndex(cueName.c_str());
			}
		}
		
		return index;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::LoadXACTSettingsBuffer(std::string file) {
		HANDLE hFile;
		std::string errStr;
		
		file = NLS_ENGINE_DATA_PATH + file;
		
		LOG(LOG_PRIORITY::FLOW, "Loading setting file: " + file);
		
		unsigned int count;
		{
			Threading::ReadLock r_lock(this->waveBanksMutex);
			count = this->waveBanks.count(file);
		}
		
		if (count <= 0) {
			hFile = CreateFile( file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL );
			
			if( hFile != INVALID_HANDLE_VALUE ) {
				DWORD dwFileSize = GetFileSize( hFile, NULL );
				HANDLE hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
				VOID* pSoundBankData = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				
				/*TODO
				EngineParameters.pGlobalSettingsBuffer = pvGlobalSettingsData;
				EngineParameters.globalSettingsBufferSize = dwGlobalSettingsFileSize;
				EngineParameters.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA;
				*/
				
			}
			else {
				errStr = "Couldn't find the settings file!!"; //*TODO: better error message
				LOG(LOG_PRIORITY::MISSRESS, errStr);
			}
		}
		else {
			errStr = "Settings file already loaded!!"; //*TODO: better error message
			LOG(LOG_PRIORITY::ERR, errStr);
		}
		
		if (errStr.compare("") != 0) {
			// Notify failure to load
			EnvelopeSPTR confirmation(new Envelope);
			confirmation->msgid = ACK_LOAD_XACT_SETTINGS_FILE;
			confirmation->AddData(file);
			confirmation->AddData(false);
			this->msgrouter->SendSP(confirmation);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::LoadInMemoryWavebank(std::string file) {
		//*TODO: Load an in-memory wavebank
		
		file = NLS_ENGINE_DATA_PATH + file;
		
		LOG(LOG_PRIORITY::WARN, "Loading in-memory wavebank: " + file + " --- UNIMPLEMENTED!!!");
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::LoadStreamingWavebank(std::string file) {
		IXACT3WaveBank* pStreamingWaveBank = NULL;
		HANDLE hFile;
		HRESULT hr;
		std::string errStr ("");
		
		file = NLS_ENGINE_DATA_PATH + file;
		
		LOG(LOG_PRIORITY::FLOW, "Loading streaming wavebank: " + file);
		
		unsigned int count;
		{
			Threading::ReadLock r_lock(this->waveBanksMutex);
			count = this->waveBanks.count(file);
		}
		
		if (count <= 0) {
			hFile = CreateFile( file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL );
			
			if( hFile != INVALID_HANDLE_VALUE ) {
				XACT_WAVEBANK_STREAMING_PARAMETERS wsParams;
				ZeroMemory( &wsParams, sizeof(XACT_WAVEBANK_STREAMING_PARAMETERS) );
				wsParams.file = hFile;
				wsParams.offset = 0;
				wsParams.packetSize = 64;
				
				{
					Threading::ReadLock r_lock(this->engineMutex);
					hr = this->engine->CreateStreamingWaveBank( &wsParams, &pStreamingWaveBank );
				}
				
				if (SUCCEEDED(hr)) {
					// Put the wave bank into the map
					Threading::WriteLock w_lock(this->waveBanksMutex);
					this->waveBanks[file] = pStreamingWaveBank;
				}
				else {
					errStr.assign("Couldn't load the wavebank!!"); //*TODO: better error message
				}
			}
			else {
				errStr.assign("Couldn't find the wavebank!!"); //*TODO: better error message
			}
		}
		else {
			errStr.assign("Wavebank already loaded!!"); //*TODO: better error message
		}
		
		if (errStr.compare("") != 0) {
			LOG(LOG_PRIORITY::ERR, errStr);
			
			// Notify failure to load
			EnvelopeSPTR confirmation(new Envelope);
			confirmation->msgid = ACK_LOAD_STREAMING_WAVEBANK;
			confirmation->AddData(file);
			confirmation->AddData(false);
			msgrouter->SendSP(confirmation);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::LoadSoundbank(std::string file) {
		IXACT3SoundBank* pSoundBank;
		HANDLE hFile;
		HRESULT hr;
		std::string errStr ("");
		
		file = NLS_ENGINE_DATA_PATH + file;
		
		LOG(LOG_PRIORITY::FLOW, "Loading soundbank: " + file);
		
		unsigned int count;
		
		{
			Threading::ReadLock r_lock(this->soundBanksMutex);
			count = this->soundBanks.count(file);
		}
		
		if (count <= 0) {
			hFile = CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
			
			if( hFile != INVALID_HANDLE_VALUE ) {
				DWORD dwFileSize = GetFileSize( hFile, NULL );
				VOID* pSoundBankData = new BYTE[dwFileSize];
				DWORD bytesRead;
				
				if (ReadFile(hFile, pSoundBankData, dwFileSize, &bytesRead, NULL)) {
					{
						Threading::ReadLock r_lock(this->engineMutex);
						hr = this->engine->CreateSoundBank(pSoundBankData, dwFileSize, 0, 0, &pSoundBank);
					}
					
					if (SUCCEEDED(hr)) {
						// Put the wave bank into the map
						Threading::WriteLock w_lock(this->soundBanksMutex);
						this->soundBanks[file] = pSoundBank;
					}
					else {
						errStr.assign("Couldn't load the soundbank!!"); //*TODO: better error message
						LOG(LOG_PRIORITY::ERR, errStr);
					}
				}
			}
			else {
				errStr.assign("Couldn't find the soundbank!!"); //*TODO: better error message
				LOG(LOG_PRIORITY::MISSRESS, errStr);
			}
		}
		else {
			errStr.assign("Soundbank already loaded!!"); //*TODO: better error message
			LOG(LOG_PRIORITY::ERR, errStr);
		}
		
		if (errStr.compare("") != 0) {
			// Notify failure to load
			EnvelopeSPTR confirmation(new Envelope);
			confirmation->msgid = ACK_LOAD_STREAMING_WAVEBANK;
			confirmation->AddData(file);
			confirmation->AddData(false);
			msgrouter->SendSP(confirmation);
		}
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::RegisterNotification(XACTNOTIFICATIONTYPE type, BYTE flags, PVOID context) {
		XACT_NOTIFICATION_DESCRIPTION desc = {0};
		desc.type       = type;
		desc.flags      = flags;
		desc.pSoundBank = NULL;              // SoundBank instance
		desc.pWaveBank  = NULL;              // WaveBank instance
		desc.pCue       = NULL;              // Cue instance
		desc.pWave      = NULL;              // Wave instance
		desc.cueIndex   = XACTINDEX_INVALID; // Cue index
		desc.waveIndex  = XACTINDEX_INVALID; // Wave index
		desc.pvContext  = context;           // User context (optional)
		
		Threading::WriteLock w_lock(this->engineMutex);
		this->engine->RegisterNotification(&desc);
	}
	
	
	/**
	* Finds a key in a map by the value.  Only the first key found with the given value is returned.
	* Note that multiple calls to this method may provide different answers, depending on how the std::map is implemented for your compiler.
	* If the specified value is not found, a NULL is returned - this may not work with some types of A.
	*/
	template <typename A, typename B>
	const A FindKeyByValue(std::map<A, B> map, B value) {
		for (std::map<A, B>::const_iterator it = map.begin(); it != map.end(); ++it) {
			if (it->second == value ){
				return (*it).first;
			}
		}
		return NULL;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION* notification) {
		SoundModule* audio_module = SoundModule::GetModuleInstance();
		
		if (!audio_module->engineInitialized) {
			// *HACK: This is getting called during dtor if there's a sound playing.  This prevents this from being an issue as the below code is unsafe during dtor.
			return;
		}
		
		if (notification->type == XACTNOTIFICATIONTYPE_WAVEBANKPREPARED) {
			std::string file;
			
			{
				Threading::ReadLock r_lock(audio_module->waveBanksMutex);
				file = FindKeyByValue<std::string, IXACT3WaveBank*>(audio_module->waveBanks, notification->waveBank.pWaveBank);
			}
			
			if (file.length() > 0) {
				// Notify completion of wavebank loadup
				EnvelopeSPTR confirmation(new Envelope);
				confirmation->msgid = ACK_LOAD_STREAMING_WAVEBANK;
				confirmation->AddData(file);
				confirmation->AddData(true);
				audio_module->msgrouter->SendSP(confirmation);
				
				std::string logStr;
				logStr.assign("Wavebank Prepared: Loaded wavebank: ").append(file);
				LOG(LOG_PRIORITY::FLOW, logStr);
			}
			else {
				std::string logStr;
				logStr.assign("Wavebank Prepared: Wavebank reported as loaded, but was not found in memory!");
				LOG(LOG_PRIORITY::ERR, logStr);
			}
		}
		else if (notification->type == XACTNOTIFICATIONTYPE_CUEPREPARED) {
			// Handle the cue prepared notification
			std::string file;
			
			{
				Threading::ReadLock r_lock(audio_module->soundBanksMutex);
				file = FindKeyByValue<std::string, IXACT3SoundBank*>(audio_module->soundBanks, notification->cue.pSoundBank);
			}
			
			if (file.length() > 0) {
				EnvelopeSPTR confirmation(new Envelope);
				confirmation->msgid = ACK_PREPARE_CUE;
				confirmation->AddData(file);
				confirmation->AddData(notification->cue.cueIndex);
				audio_module->msgrouter->SendSP(confirmation);
				
				std::string logStr;
				logStr.assign("Cue Prepared: Soundbank ").append(file).append(" with cue index ").append(to_string<int>(notification->cue.cueIndex));
				LOG(LOG_PRIORITY::FLOW, logStr);
			}
			else {
				std::string logStr;
				logStr.assign("Cue Prepared: Soundbank reported as loaded, but was not found in memory!");
				LOG(LOG_PRIORITY::ERR, logStr);
			}
		}
		else if (notification->type == XACTNOTIFICATIONTYPE_CUESTOP) {
			// Handle the cue stop notification
			std::string file;
			
			{
				Threading::ReadLock r_lock(audio_module->soundBanksMutex);
				file = FindKeyByValue<std::string, IXACT3SoundBank*>(audio_module->soundBanks, notification->cue.pSoundBank);
			}
			
			if (file.length() > 0) {
				EnvelopeSPTR confirmation(new Envelope);
				confirmation->msgid = ACK_STOP_CUE;
				confirmation->AddData(file);
				confirmation->AddData(notification->cue.cueIndex);
				audio_module->msgrouter->SendSP(confirmation);
				
				std::string logStr;
				logStr.assign("Cue Stopped: Soundbank ").append(file).append(" with cue index ").append(to_string<int>(notification->cue.cueIndex));
				LOG(LOG_PRIORITY::FLOW, logStr);
				
				// If the cue was previously prepared, then remove it from the list as it is no longer useable.
				std::pair<std::string, int> key (file, notification->cue.cueIndex);
				if (audio_module->preparedCues.count(key)) {
					audio_module->preparedCues.erase(key);
				}
			}
			else {
				std::string logStr;
				logStr.assign("Cue Stopped: Soundbank reported as loaded, but was not found in memory!");
				LOG(LOG_PRIORITY::FLOW, logStr);
			}
		}
	}
}
