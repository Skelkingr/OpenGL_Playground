#include "CubeApp.h"

int main()
{
	CubeApp theApp;

	if (!theApp.Init())
		return 1;

	return theApp.Run();
}