#define STB_IMAGE_IMPLEMENTATION

#include "App.h"

int main()
{
	App theApp;

	if (!theApp.Init())
		return 1;

	return theApp.Run();
}