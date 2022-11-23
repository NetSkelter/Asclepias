/*
* File:		Logging.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include "Asclepias.h"

namespace ASC {
	void LogManager::init(bool console, const std::vector<std::string>& fileNames,
		const std::string& timestampFmt) {
		console_ = console;
		for (const std::string& fileName : fileNames) {
			files_.push_back(std::ofstream(fileName));
		}
		timestampFmt_ = timestampFmt;
	}

	const std::string LogManager::getTimestamp() const {
		struct tm t;
		__time64_t lt;
		char buffer[64];
		errno_t err;
		_time64(&lt);
		err = _localtime64_s(&t, &lt);
		if (err) {
			return "";
		}
		strftime(buffer, sizeof(buffer), timestampFmt_.c_str(), &t);
		return std::string(buffer);
	}

	void LogManager::destroy() {
#ifdef _DEBUG
		console_ = true;
#else
		console_ = false;
#endif
		for (std::ofstream& of : files_) {
			of.close();
		}
		files_.clear();
		timestampFmt_ = "";
	}
}