#pragma once
#include <string>

class Exception: public std::runtime_error {
public:
	Exception(std::string message) : runtime_error(message) {}
	using std::runtime_error::runtime_error;
};