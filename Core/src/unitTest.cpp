#include "unitTest.h"
#include "debug.h"
#include "main.h"

void unitTest()
{
	MODULE_START("Point Test");
	float testPoints[] = {
		0.0,1.0,0.01,10000.0,-1.0,1.2
	};
	//Point
	Point points[3];
	for (int i = 0, j = 0; i < 3; i++)
	{
		points[i].SetX(testPoints[j++]);
		points[i].SetY(testPoints[j++]);
		assert(Equal(points[i].GetX(), testPoints[j - 2]));
		assert(Equal(points[i].GetY(), testPoints[j - 1]));
	}
	MODULE_END("Point Test");
	MODULE_START("Triangle Test");
	Triangle triangle;
	triangle.SetPoints(points);
	for (int i = 0; i < 3; i++)
	{
		assert(Equal(triangle.GetPoint(i).GetX(), points[i].GetX()));
		assert(triangle.GetPoint(i) == points[i]);
	}
	MODULE_END("Triangle Test");
	MODULE_START("Draw Test");
	App* app = new App;
	app->Initialize();
	app->Run();
	delete app;
	app = NULL;
	MODULE_END("Draw Test");
}