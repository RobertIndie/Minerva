/*
	Studio: Ambicour Studio
	Author: Aaron Robert
*/
#include "core.h"
#include "debug.h"

using namespace std;

class App :public Renderer
{
public:
	void Update()
	{

	}
};


int main()
{
	App* app = new App;
	app->Initialize();
	app->Run();
	return 0;
}
