#pragma once

void debugOutput(const char* message);
void errorInput(const char* error);

#define MODULE_START(m) debugOutput((string("{")+string(m)+string("}Start.")).c_str());
#define MODULE_END(m) debugOutput((string("{")+string(m)+string("}End.")).c_str());