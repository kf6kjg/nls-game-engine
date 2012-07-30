/**
 * \file
 * \author Ricky Curtice
 * \brief Provides language-specific strings for usage in the engine.
 */
#include "EngineConfig.h"

namespace NLS_I18N {
	const std::string TITLE_INFO("Information");
	const std::string TITLE_WARNING("Warning");
	const std::string TITLE_CRITICAL("Error");
	
	const std::string FILESYSTEM_ERROR_TITLE("File System Error!");
	const std::string FILESYSTEM_PER_USER_FOLDER_ACCESS_ERROR("Attempt to access per-user config folder failed:\n");
	const std::string FILESYSTEM_PER_USER_FOLDER_WRITE_ERROR("Attempt to write to per-user config folder failed.");
	const std::string FILESYSTEM_PER_USER_FOLDER_CREATE_ERROR("Attempt to create per-user config folder failed:\n");
	
	const std::string CONFIG_LOAD_ERROR("Unable to load configuration file from disk.\nUnable to continue, please add the configuration file!");
	const std::string CONFIG_KEY_MISSING_ERROR("Configuration file incomplete!\nUnable to continue, please correct the configuration file!\nMissing key: ");
	
	const std::string CONFIG_PARSE_KEY_ERROR("Unable to load configuration file: key of invalid type!");
	const std::string CONFIG_PARSE_VALUE_ERROR("Unable to load configuration file: unrecognized value for key: ");
	const std::string CONFIG_PARSE_VALUE_MISSING_ERROR("Unable to load configuration file: missing value for key: ");
	
	const std::string CONFIG_LEX_KEY_ERROR("Unable to load configuration file: unrecognized key: ");
	const std::string CONFIG_LEX_DUP_KEY_ERROR("Unable to load configuration file: key not valid to be duplicated: ");
	const std::string CONFIG_LEX_VALUE_ERROR("Unable to load configuration file: unrecognized value type for key: ");
}
