#include "unitTest.h"
#include "debug.h"

void unitTest()
{
	MODULE_START("Unit Test");
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

	Triangle triangle;
	triangle.SetPoints(points);
	for (int i = 0; i < 3; i++)
	{
		assert(Equal(triangle.GetPoint(i).GetX(), points[i].GetX()));
		assert(triangle.GetPoint(i) == points[i]);
	}
	MODULE_END("Unit Test");
}