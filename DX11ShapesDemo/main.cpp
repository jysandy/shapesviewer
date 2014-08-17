#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "ShapesApp.h"
#include <iostream>

int APIENTRY wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInst, PWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInst);

	ShapesDemo::ShapesApp theApp(hinstance);

	try
	{
		if (theApp.Init())
		{
			return theApp.Run();
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}