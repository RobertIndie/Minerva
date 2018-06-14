/*
	Studio: Ambicour Studio
	Author: Aaron Robert
*/
#include "main.h"
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
#include <list>
	list<int> l = { 1,2,3,4 };
	auto it = std::find(l.begin(), l.end(), 3);
	App* app = new App;
	app->Initialize();
	app->Run();
	return 0;
}
