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

// Application Library Includes
#include <EngineConfig.h>

// Local Includes
#include "../sharedbase/MessageRouter.h"
#include "../sharedbase/Envelope.h"

#include "Messages.h"

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

// Classes
namespace Sound {
	#define SUBSCRIBE_BASIC(msgid, handler)   {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Subscribe((msgid), handler_sptr);}
	#define UNSUBSCRIBE_BASIC(msgid, handler) {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Unsubscribe((msgid), handler_sptr);}
	#define SUBSCRIBE_DIRECTED(msgid, handler)   {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->SubscribeDirected((msgid), handler_sptr);}
	#define UNSUBSCRIBE_DIRECTED(msgid, handler) {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->UnsubscribeDirected((msgid), handler_sptr);}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::RegisterMessageHandlers() {
		/* * * * * * * * * * * * * * * * * * * * * */
		// General Message Handlers
		/* * * * * * * * * * * * * * * * * * * * * */
		SUBSCRIBE_BASIC(MSG_MODULE_START, SoundModule::HandlerStartup);
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Operations Message Handlers
		/* * * * * * * * * * * * * * * * * * * * * */
		// *TODO: Break these up into seperate message handlers.
		SUBSCRIBE_BASIC(MSG_LOAD_XACT_SETTINGS_FILE, SoundModule::HandlerBasic);
		SUBSCRIBE_BASIC(MSG_LOAD_IN_MEMORY_WAVEBANK, SoundModule::HandlerBasic);
		SUBSCRIBE_BASIC(MSG_LOAD_STREAMING_WAVEBANK, SoundModule::HandlerBasic);
		SUBSCRIBE_BASIC(MSG_LOAD_SOUNDBANK, SoundModule::HandlerBasic);
		SUBSCRIBE_BASIC(MSG_PREPARE_CUE, SoundModule::HandlerBasic);
		SUBSCRIBE_BASIC(MSG_PLAY_CUE, SoundModule::HandlerBasic);
		//SUBSCRIBE_BASIC(MSG_PLAY_CUE_AT, SoundModule::HandlerBasic);
		//SUBSCRIBE_BASIC(MSG_PREPARE_3D_CUE, SoundModule::HandlerBasic);
		//SUBSCRIBE_BASIC(MSG_PLAY_3D_CUE, SoundModule::HandlerBasic);
		SUBSCRIBE_BASIC(MSG_STOP_CUE, SoundModule::HandlerBasic);
		
		SUBSCRIBE_DIRECTED(MSG_GET_CUE_KEY, SoundModule::HandlerDirectedGetCueIndex);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::HandlerStartup(EnvelopeSPTR envelope) {
		bool status = false;
		
		if (this->engine == NULL) {
			//Nothing to startup
			return;
		}
		
		{
			// Startup the XACT engine
			XACT_RUNTIME_PARAMETERS xrParams = {0};
			xrParams.fnNotificationCallback = XACTNotificationCallback;
			
			{ //*HACK: This whole block doesn't belong here, but it can't go anywhere else.  Grrrr.  ~Ricky
				std::string file;
				HANDLE hFile;
				std::string errStr;
				
				try {
					file = NLS_ENGINE_DATA_PATH + boost::any_cast<std::string>(envelope->GetData(0));
				}
				catch (boost::bad_any_cast&) {
					errStr += "Audio Startup: Bad data type passed for the sound settings file path string!";
				}
				catch (std::out_of_range&) {
					errStr += "Audio Startup: Missing path string for the sound settings file!";
				}
				
				if (file.length() > 0) {
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
							VOID* pvGlobalSettingsData = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
							
							if (pvGlobalSettingsData != NULL) {
								xrParams.pGlobalSettingsBuffer = pvGlobalSettingsData;
								xrParams.globalSettingsBufferSize = dwFileSize;
								//xrParams.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA;
							}
							else {
								DWORD dw = GetLastError();
								LPVOID lpMsgBuf;
								
								FormatMessage(
									FORMAT_MESSAGE_ALLOCATE_BUFFER | 
									FORMAT_MESSAGE_FROM_SYSTEM |
									FORMAT_MESSAGE_IGNORE_INSERTS,
									NULL,
									dw,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									(LPTSTR) &lpMsgBuf,
									0, NULL
								);
								
								errStr = "Unable to load the settings file into allocated memory!!"; //*TODO: better error message
								errStr.append("\n\tError details: ").append((LPCTSTR)lpMsgBuf);
								LOG(LOG_PRIORITY::ERR, errStr);
							}
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
				}
				
				if (errStr.compare("") != 0) {
					// Notify failure to load
					EnvelopeSPTR confirmation(new Envelope);
					confirmation->msgid = ACK_LOAD_XACT_SETTINGS_FILE;
					confirmation->AddData(file);
					confirmation->AddData(false);
					this->msgrouter->SendSP(confirmation);
					return;
				}
			}
			
			xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
			HRESULT hr;
			{
				Threading::WriteLock w_lock(this->engineMutex);
				hr = this->engine->Initialize( &xrParams );
			}
			if (FAILED(hr)) {
				std::string errStr = "Unable to initialize the audio engine.  Do you have a sound device attached?  Error code 0x" + to_hex<HRESULT>(hr);
				LOG(LOG_PRIORITY::ERR, errStr);
				MessageBox(NULL, (LPCTSTR)errStr.c_str(), TEXT("Audio Error"), MB_OK);
				
				this->engine = NULL; // No engine.
				
				status = false;
			}
			else {
				// Register the need for wavebank loading completion notification
				this->RegisterNotification(XACTNOTIFICATIONTYPE_WAVEBANKPREPARED, XACT_FLAG_NOTIFICATION_PERSIST);
				
				// Register the need for wavebank destruction notification
				this->RegisterNotification(XACTNOTIFICATIONTYPE_WAVEBANKDESTROYED, XACT_FLAG_NOTIFICATION_PERSIST);
				
				
				// Register the need for soundbank loading completion notification
				//this->RegisterNotification(XACTNOTIFICATIONTYPE_SOUNDBANKPREPARED, XACT_FLAG_NOTIFICATION_PERSIST);
				// No such notification exists
				
				// Register the need for soundbank destruction notification
				this->RegisterNotification(XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED, XACT_FLAG_NOTIFICATION_PERSIST);
				
				// Register for knowing when a cue has been prepared.
				this->RegisterNotification(XACTNOTIFICATIONTYPE_CUEPREPARED, XACT_FLAG_NOTIFICATION_PERSIST);
				
				// Register for knowing when a cue has stopped.
				this->RegisterNotification(XACTNOTIFICATIONTYPE_CUESTOP, XACT_FLAG_NOTIFICATION_PERSIST);
				
				
				// Reset the time accumulator
				this->deltaAccumulator = 0.0;
				
				status = true;
			}
		}
		
		// Un-register from the startup message, as there's no need to start again.
		UNSUBSCRIBE_BASIC(MSG_MODULE_START, SoundModule::HandlerStartup);
		
		// Send notification that the module is started.
		{
			EnvelopeSPTR e(new Envelope);
			e->msgid = ACK_MODULE_START;
			e->AddData(status);
			this->msgrouter->SendSP(e);
		}
		
		LOG(LOG_PRIORITY::FLOW, "SimpleSound started.");
		
		this->engineInitialized = true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <typename T>
	inline void HandleMessageSingleParam(boost::function<void (T)> setter, const int msgID, EnvelopeSPTR envelope) {
		T value;
		
		try {
			value = boost::any_cast<T>(envelope->GetData());
		}
		catch (boost::bad_any_cast&) {
			std::string errStr ("Bad data type passed for message id ");
			errStr.append(boost::lexical_cast<std::string>(msgID));
			LOG(LOG_PRIORITY::ERR, errStr);
		}
		
		setter(value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void SoundModule::HandlerBasic(EnvelopeSPTR envelope) {
		if (this->engine == NULL) {
			//Nothing to do here
			return;
		}
		
		int msg = envelope->msgid;
		
		switch (msg) {
			case MSG_LOAD_XACT_SETTINGS_FILE: {
				HandleMessageSingleParam <std::string> (
					std::bind1st(std::mem_fun(&SoundModule::LoadXACTSettingsBuffer), this), // The setter method
					msg, envelope
				);
			}
			break;
			case MSG_LOAD_IN_MEMORY_WAVEBANK: {
				HandleMessageSingleParam <std::string> (
					std::bind1st(std::mem_fun(&SoundModule::LoadInMemoryWavebank), this), // The setter method
					msg, envelope
				);
			}
			break;
			case MSG_LOAD_STREAMING_WAVEBANK: {
				HandleMessageSingleParam <std::string> (
					std::bind1st(std::mem_fun(&SoundModule::LoadStreamingWavebank), this), // The setter method
					msg, envelope
				);
			}
			break;
			case MSG_LOAD_SOUNDBANK: {
				HandleMessageSingleParam <std::string> (
					std::bind1st(std::mem_fun(&SoundModule::LoadSoundbank), this), // The setter method
					msg, envelope
				);
			}
			break;
			case MSG_PREPARE_CUE: {
				std::string bankName ("");
				int cueIndex = -1;
				
				try {
					bankName = NLS_ENGINE_DATA_PATH + boost::any_cast<std::string>(envelope->GetData(0));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Prepare Cue: Bad data type passed for the sound bank id in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				
				try {
					cueIndex = boost::any_cast<int>(envelope->GetData(1));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Prepare Cue: Bad data type passed for the cue index in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				
				if (bankName.compare("") != 0 && cueIndex >= 0) {
					unsigned int count;
					{
						Threading::ReadLock r_lock(this->soundBanksMutex);
						count = this->soundBanks.count(bankName);
					}
					
					if (count) {
						std::pair<std::string, int> key (bankName, cueIndex);
						{
							Threading::ReadLock r_lock(this->preparedCuesMutex);
							count = this->preparedCues.count(key);
						}
						
						if (count == 0) {
							IXACT3SoundBank* soundBank;
							{
								Threading::ReadLock r_lock(this->soundBanksMutex);
								soundBank = this->soundBanks.at(bankName);
							}
							
							std::string logStr;
							logStr.assign("Preparing cue in ").append(bankName).append(" with index ").append(to_string<int>(cueIndex));
							LOG(LOG_PRIORITY::FLOW, logStr);
							
							IXACT3Cue* cue = nullptr;
							HRESULT hr = 0;
							
							XACTINDEX cue_count;
							{
								Threading::ReadLock r_lock(this->engineMutex);
								soundBank->GetNumCues(&cue_count);
							}
							
							if (cueIndex < cue_count) {
								{
									Threading::WriteLock w_lock(this->engineMutex);
									hr = soundBank->Prepare(cueIndex, 0, 0, &cue);
								}
								
								if (SUCCEEDED(hr)) {
									Threading::WriteLock w_lock(this->preparedCuesMutex);
									this->preparedCues[key] = cue;
								}
								else {
									std::string logStr;
									logStr.assign("Cue prepare failed with error code: 0x").append(to_hex<HRESULT>(hr));
									LOG(LOG_PRIORITY::ERR, logStr);
								}
							}
							else {
								LOG(LOG_PRIORITY::CONFIG, "Attempted to prepare cue with out-of-bounds index!");
							}
						}
						else {
							std::string logStr;
							logStr.assign("Cue in ").append(bankName).append(" with index ").append(to_string<int>(cueIndex)).append(" already prepared!");
							LOG(LOG_PRIORITY::WARN, logStr);
						}
					}
					else {
						std::string logStr;
						logStr.assign("Prepare Cue: Named bank not found ").append(bankName).append(" for cue index ").append(to_string<int>(cueIndex));
						LOG(LOG_PRIORITY::ERR, logStr);
					}
				}
				else {
					std::string logStr;
					logStr.assign("Prepare Cue: Bank name or cue index missing! Bank name: '").append(bankName).append("'. Cue Index: ").append(to_string<int>(cueIndex));
					LOG(LOG_PRIORITY::ERR, logStr);
				}
			}
			break;
			case MSG_PLAY_CUE: {
				std::string bankName ("");
				XACTINDEX cueIndex = -1;
				int count = 1;
				float volume = 1.0f;
				
				try {
					bankName = NLS_ENGINE_DATA_PATH + boost::any_cast<std::string>(envelope->GetData(0));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Play Cue: Bad data type passed for the sound bank id in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				
				try {
					cueIndex = boost::any_cast<int>(envelope->GetData(1));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Play Cue: Bad data type passed for the cue index in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				
				try {
					count = boost::any_cast<int>(envelope->GetData(2));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Play Cue: Bad data type passed for the cue repeat count in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				catch (std::out_of_range&) {
					// Do nothing: parameter IS optional!
				}
				
				try {
					volume = boost::any_cast<float>(envelope->GetData(3));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Play Cue: Bad data type passed for the cue volume in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				catch (std::out_of_range&) {
					// Do nothing: parameter IS optional!
				}
				
				if (bankName.compare("") != 0 && cueIndex >= 0) {
					std::pair<std::string, int> key (bankName, cueIndex);
					HRESULT hr = 0;
					
					unsigned int prep_cues_count;
					unsigned int banks_count;
					
					{
						Threading::ReadLock r_lock(this->preparedCuesMutex);
						prep_cues_count = this->preparedCues.count(key);
					}
					
					{
						Threading::ReadLock r_lock(this->soundBanksMutex);
						banks_count = this->soundBanks.count(bankName);
					}
					
					if (prep_cues_count) {
						IXACT3Cue* cue;
						{
							Threading::ReadLock r_lock(this->preparedCuesMutex);
							cue = this->preparedCues[key];
						}
						//*TODO: adjust volume of cue based on parameter...
						//*TODO: figure out how to repeat the cue based on parameter... (note that -1 means infinite repeat)
						
						std::string logStr;
						logStr.assign("Playing pre-prepared cue in ").append(bankName).append(" with index ").append(to_string<int>(cueIndex));
						LOG(LOG_PRIORITY::FLOW, logStr);
						
						{
							Threading::ReadLock r_lock(this->engineMutex);
							hr = cue->Play();
						}
					}
					else if (banks_count) {
						IXACT3SoundBank* soundBank;
						{
							Threading::ReadLock r_lock(this->soundBanksMutex);
							soundBank = this->soundBanks.at(bankName);
						}
						
						//*TODO: adjust volume of cue based on parameter...
						//*TODO: figure out how to repeat the cue based on parameter... (note that -1 means infinite repeat)
						
						std::string logStr;
						logStr.assign("Playing unprepared cue in ").append(bankName).append(" with index ").append(to_string<int>(cueIndex));
						LOG(LOG_PRIORITY::FLOW, logStr);
						
						XACTINDEX cue_count;
						{
							Threading::ReadLock r_lock(this->engineMutex);
							soundBank->GetNumCues(&cue_count);
						}
						
						if (cueIndex < cue_count) {
							Threading::ReadLock r_lock(this->engineMutex);
							hr = soundBank->Play(cueIndex, 0, 0, nullptr);
						}
						else {
							LOG(LOG_PRIORITY::ERR, "Attempted to play an index that is out-of-bounds!");
						}
					}
					else {
						std::string logStr;
						logStr.assign("Play Cue: Named bank not found ").append(bankName).append(" for cue index ").append(to_string<int>(cueIndex));
						LOG(LOG_PRIORITY::ERR, logStr);
					}
					
					if (FAILED(hr)) {
						std::string logStr("Cue failed to play.");
						switch (hr) {
							case 0x8ac7000c: { //XACTENGINE_E_INVALIDCUEINDEX: {
								logStr += " Invalid cue index of " + to_string<int>(cueIndex) + ".";
							}
							break;
							default: {
								logStr += " Error code: 0x" + to_hex<HRESULT>(hr);
							}
						}
						LOG(LOG_PRIORITY::ERR, logStr);
					}
				}
				else {
					std::string logStr;
					logStr.assign("Play Cue: Bank name or cue index missing! Bank name: '").append(bankName).append("'. Cue Index: ").append(to_string<int>(cueIndex));
					LOG(LOG_PRIORITY::ERR, logStr);
				}
			}
			break;
			case MSG_STOP_CUE: {
				std::string bankName ("");
				int cueIndex = -1;
				
				try {
					bankName = NLS_ENGINE_DATA_PATH + boost::any_cast<std::string>(envelope->GetData(0));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Stop Cue: Bad data type passed for the sound bank id in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				
				try {
					cueIndex = boost::any_cast<int>(envelope->GetData(1));
				}
				catch (boost::bad_any_cast&) {
					std::string errStr ("Stop Cue: Bad data type passed for the cue index in message id ");
					errStr.append(boost::lexical_cast<std::string>(msg));
					LOG(LOG_PRIORITY::ERR, errStr);
				}
				
				if (bankName.compare("") != 0 && cueIndex >= 0) {
					unsigned int count;
					{
						Threading::ReadLock r_lock(this->soundBanksMutex);
						count = this->soundBanks.count(bankName);
					}
					
					if (count) {
						IXACT3SoundBank* soundBank;
						
						{
							Threading::ReadLock r_lock(this->soundBanksMutex);
							soundBank = this->soundBanks.at(bankName);
						}
						
						std::string logStr;
						logStr.assign("Stopping cue in ").append(bankName).append(" with index ").append(to_string<int>(cueIndex));
						LOG(LOG_PRIORITY::FLOW, logStr);
						
						HRESULT hr;
						{
							Threading::ReadLock r_lock(this->engineMutex);
							hr = soundBank->Stop(cueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
						}
						
						if (FAILED(hr)) {
							std::string logStr;
							logStr.assign("Cue failed to stop. Error code: 0x").append(to_hex<HRESULT>(hr));
							LOG(LOG_PRIORITY::ERR, logStr);
						}
					}
					else {
						std::string logStr;
						logStr.assign("Stop Cue: Named bank not found ").append(bankName).append(" for cue index ").append(to_string<int>(cueIndex));
						LOG(LOG_PRIORITY::ERR, logStr);
					}
				}
				else {
					std::string logStr;
					logStr.assign("Stop Cue: Bank name or cue index missing! Bank name: '").append(bankName).append("'. Cue Index: ").append(to_string<int>(cueIndex));
					LOG(LOG_PRIORITY::ERR, logStr);
				}
			}
			break;
			default: // Do nothing: the message wasn't meant for me.
			break;
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	EnvelopeSPTR SoundModule::HandlerDirectedGetCueIndex(EnvelopeSPTR envelope) {
		std::string bankName ("");
		std::string cueName ("");
		
		EnvelopeSPTR confirmation(new Envelope);
		confirmation->msgid = ACK_GET_CUE_KEY;
		
		try {
			bankName = NLS_ENGINE_DATA_PATH + boost::any_cast<std::string>(envelope->GetData(0));
		}
		catch (boost::bad_any_cast&) {
			LOG(LOG_PRIORITY::ERR, "Bad data type passed for the sound bank id!");
		}
		
		try {
			cueName = boost::any_cast<std::string>(envelope->GetData(1));
		}
		catch (boost::bad_any_cast&) {
			LOG(LOG_PRIORITY::ERR, "Bad data type passed for the cue name!");
		}
		
		if (bankName.compare("") != 0 && cueName.compare("") != 0) {
			unsigned int count;
			{
				Threading::ReadLock r_lock(this->soundBanksMutex);
				count = this->soundBanks.count(bankName);
			}
			
			if (count) {
				IXACT3SoundBank* soundBank;
				{
					Threading::ReadLock r_lock(this->soundBanksMutex);
					soundBank = this->soundBanks.at(bankName);
				}
				unsigned int cue_index;
				
				{
					Threading::ReadLock r_lock(this->engineMutex);
					cue_index = soundBank->GetCueIndex(cueName.c_str());
				}
				
				if (cue_index != XACTINDEX_INVALID) {
					confirmation->AddData(bankName);
					confirmation->AddData(cue_index);
					
					LOG(LOG_PRIORITY::FLOW, "Got cue in " + bankName + " with name " + cueName + " and resulting index " + boost::lexical_cast<std::string>(cue_index));
				}
				else {
					LOG(LOG_PRIORITY::ERR, "Cue not found in " + bankName + " with name " + cueName + "!");
				}
			}
			else {
				LOG(LOG_PRIORITY::ERR, "Named bank not found " + bankName + " for cue name " + cueName);
			}
		}
		else {
			LOG(LOG_PRIORITY::ERR, "Bank name or cue name missing! Bank name: '" + bankName + "'. Cue Name: " + cueName);
		}
		
		return confirmation;
	}
}
