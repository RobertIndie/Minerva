#include "debug.h"
#include <iostream>
#include <stdarg.h>

using namespace std;

void debugOutput(const char* message)
{
	cout << "[DEBUG]" << message << endl;
}

void errorInput(const char* error)
{
	cout << "[ERROR]" << error << endl;
}
