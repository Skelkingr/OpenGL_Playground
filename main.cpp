#include "TriangleApp.h"

int main()
{
	TriangleApp theApp;

	if (!theApp.Init())
		return 1;

	return theApp.Run();
}