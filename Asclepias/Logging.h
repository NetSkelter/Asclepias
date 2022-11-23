/*
* File:		Logging.h
* Author:	NetSkelter
* Created:	2022.11.22
*/

#ifndef ASC_LOGGING_H
#define ASC_LOGGING_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define ASCLOG(SRC, LVL, ...) App::log(), App::log().getTimestamp(), \
	": [", #SRC, "] [", #LVL, "] ", __VA_ARGS__, "\n"

namespace ASC {
	class LogMgr {
	public:
		void init(bool, const std::vector<std::string>&, const std::string&);
		template <typename T>
		void write(const T& t) {
			if (console_) {
				std::cout << t;
			}
			for (std::ofstream& of : files_) {
				if (of.is_open()) {
					of << t;
				}
			}
		}
		const std::string getTimestamp() const;
		void destroy();

	private:
		bool console_
#ifdef _DEBUG
			= true;
#else
			= false;
#endif
		std::vector<std::ofstream> files_;
		std::string timestampFmt_ = "";
	};

	template<typename T>
	LogMgr& operator , (LogMgr& log, const T& t) {
		log.write(t);
		return log;
	}
}

#endif