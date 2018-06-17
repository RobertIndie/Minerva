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
	for (int i = 0,j=0; i < 3; i++)
	{
		points[i].SetX(testPoints[j++]);
		points[i].SetY(testPoints[j++]);
		assert(Equal(points[i].GetX(), testPoints[j - 2]));
		assert(Equal(points[i].GetY(), testPoints[j - 1]));
	}
	//MODULE_END("Unit Test");
}