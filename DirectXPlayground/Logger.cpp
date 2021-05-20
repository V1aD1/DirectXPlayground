#include "pch.h"
#include "Logger.h"

#include <iostream>
#include <sstream>

void Logger::Log(std::string&& msg) {
	if (msg.length() == 0) {
		return;
	}

	// appending new line character in case it was forgotten
	if (msg.back() != '\n') {
		msg.append("\n");
	}

	std::ostringstream os;
	os << msg;
	OutputDebugStringA(os.str().c_str());
}
