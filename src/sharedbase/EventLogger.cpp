/**
* \file
* \author Adam Martin
* \date 2011-10-25
* \brief Simple event logger to output debug and other messages to file
*
*
*/

#include "EventLogger.h"

// Standard Includes
#include <cassert>
#include <fstream>

// Library Includes
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>
#include <EngineConfig.h>

// Local Includes

// Static class member initialization
EventLogger* EventLogger::glogger(nullptr);
std::string EventLogger::module;

// Forward declares
void ArchiveOldLog(const std::string&, const std::string& = "", const unsigned int& = 1);

// Class methods in the order they are defined within the class header

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EventLogger::EventLogger() {
}

EventLogger::~EventLogger() {
	if (this->logFile.length() > 0) {
		std::ofstream out_stream;

		out_stream.open(this->logFile.c_str(), std::ios_base::out | std::ios_base::app);

		if (!out_stream.bad()) {
			out_stream << "]";
			out_stream.close();
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/// Static member function.  Allows each compilation unit to set their unique glogger, but keeps the singleton pattern enforced within each unit.
/// Combined with the compilation units (the dynamic libraries) being passed an instance of the EventLogger, as created in main(), this creates a cross-compilation singleton.
void EventLogger::SetEventLogger(EventLogger* elog) {
	if (EventLogger::glogger == nullptr) {
		EventLogger::glogger = elog;
	}
}

/// Static member function.  Acts as combination factory and getter of the singleton.
EventLogger* EventLogger::GetEventLogger() {
	if (EventLogger::glogger == nullptr) {
		EventLogger::SetEventLogger(new EventLogger());
	}
	
	return EventLogger::glogger;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool EventLogger::LogToDisk(const LOG_PRIORITY::TYPE& priority_level, const std::string& text, const std::string& file, const unsigned int& line, const std::string& func) {
	std::string fname(file);
	
	if (fname.find_last_of("/") != std::string::npos) {
		fname = fname.substr(fname.find_last_of("/") + 1);
	}
	else if (fname.find_last_of("\\") != std::string::npos) {
		fname = fname.substr(fname.find_last_of("\\") + 1);
	}
	
	Threading::WriteLock w_lock(this->mutex);
	
	// Push the formatted message onto the queue
	{
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
		
		// Format the log entry: timestamp	priority	module	file(line): function	message
		std::stringstream ss;
		ss	<< "," << "\n" << "{"
			<< "\"time\":\""	<< boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::universal_time())	<< "UTC\"," // Timestamp
			<< "\"level\":\""	<< LOG_PRIORITY::PRINTABLE_NOTICES[static_cast<unsigned int>(priority_level)]						<< "\","  // Error level
			<< "\"function\":\""<< EventLogger::module << "|" << fname << "(" << line << "): " << func								<< "\","// Module, file, line, and function in a pattern similar to VS2010's error log.
			<< "\"message\":\""	<< std::string(text).erase(text.find_last_not_of(std::string("\n\r")), std::string::npos)			<< "\""// Message
			<< "}";
		
		// Push into queue
		this->logQueue.push(ss.str());
	}
	
	// Write queued messages out to disk, but only if there is a file to send to.
	if (this->logFile.length() > 0) {
		std::string message;
		
		std::fstream out_stream;
		
		out_stream.open(this->logFile.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::ate | std::ios_base::binary);
		
		if (!out_stream.bad()) {
			//out_stream.seekp(std::ios_base::end);
			out_stream.seekp(-3, std::ios_base::end); // Move back 3 to cover over the old JSON ending brackets with \n for format.
			while (this->logQueue.size() > 0) {
				message = this->logQueue.front();
				this->logQueue.pop();
				out_stream << message << std::endl;
			}
			out_stream << "]}"; // Add back the JSON ending brackets
			out_stream.close();
		}
		else {
			return false; // Return false to indicate the log could not be written to.  Message isn't lost, just stuck in memory!
		}
	}
	
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool EventLogger::SetLogFile(const std::string& file) {
	Threading::WriteLock w_lock(this->mutex);
	
	this->logFile = file;
	
	// If the file exists already, move it to an archived name
	ArchiveOldLog(file);

	std::stringstream ss;
	ss	<< "{\"NLSEngineLogVersion1.0\":[\n"
		<< "{"
		<< "\"time\":\""	<< boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::universal_time())	<< "UTC\"," // Timestamp
		<< "\"level\":\""	<< LOG_PRIORITY::PRINTABLE_NOTICES[static_cast<unsigned int>(LOG_PRIORITY::INFO)]					<< "\"," // Error level
		<< "\"function\":\""<< EventLogger::module << "|" << "EventLogger.cpp" << "(" << __LINE__ << "): " << __FUNCTION__				<< "\","// Module, file, line, and function in a pattern similar to VS2010's error log.
		<< "\"message\":\""	<< "Log file created."																				<< "\""
		<< "}\n]}";

	if (this->logFile.length() > 0) {
		std::ofstream out_stream;

		out_stream.open(this->logFile.c_str(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);

		if (!out_stream.bad()) {
			out_stream << ss.str();
			out_stream.close();
		}
	}
	
	return true; 
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ArchiveOldLog(const std::string& old_file, const std::string& new_file, const unsigned int& index) {
	std::string base_name;
	if (new_file.length()) {
		base_name = new_file;
	}
	else {
		base_name = old_file;
	}
	
	std::string file_name(base_name + "." + boost::lexical_cast<std::string>(index));
	
	if (boost::filesystem::exists(old_file)) {
		// If the new file is already in use, go move it to the next slot first.
		if(boost::filesystem::exists(file_name)) {
			// But limit to 5 archives
			if (index < 5) {
				ArchiveOldLog(file_name, base_name, index + 1);
			}
			else {
				boost::filesystem::remove_all(file_name);
			}
		}
		
		// Rename the passed file into the empty space
		boost::filesystem::rename(old_file, file_name);
	}
}
