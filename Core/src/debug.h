#pragma once
#include <string>
#include <sstream>

using message = std::stringstream;

void debugOutput(const char* message);
void errorInput(const char* error);

#define MODULE_START(m) debugOutput((std::string("{")+std::string(m)+std::string("}Start.")).c_str());
#define MODULE_END(m) debugOutput((std::string("{")+std::string(m)+std::string("}End.")).c_str());

//string stream to c string
#define SS2CSTR(s) (s.str().c_str())
//message to c string
#define M2CSTR(m) SS2CSTR(m)