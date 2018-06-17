#include "unitTest.h"
#include "debug.h"

static bool Equal(float a, float b,float accuracy=0.001)
{
	int t = a - b;
	t = t > 0 ? t : (-t);
	return t < accuracy;
}

void unitTest()
{
	//MODULE_START("Unit Test");
	float testPoints[] = {
		0.0,1.0,0.01,10000.0,-1.0,1.2
	};
	//Point
	Point points[3];
	for (int i = 0; i < 6; )
	{
		points[i].SetX(testPoints[i++]);
		points[i].SetY(testPoints[i++]);
		assert(Equal(points[i].GetX(), testPoints[i - 2]));
		assert(Equal(points[i].GetY(), testPoints[i - 1]));
	}
	//MODULE_END("Unit Test");
}