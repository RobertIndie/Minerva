#pragma once
#include <string>

void debugOutput(const char* message);
void errorInput(const char* error);

#define MODULE_START(m) debugOutput((std::string("{")+std::string(m)+std::string("}Start.")).c_str());
#define MODULE_END(m) debugOutput((std::string("{")+std::string(m)+std::string("}End.")).c_str());