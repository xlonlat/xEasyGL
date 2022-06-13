#include "xEasyGL.h"

using namespace xlonlat;
using namespace xlonlat::xEasyGL;

int main()
{
	std::wcout.imbue(std::locale("chs"));

	{
		xWindow window("xApp", 1280, 720, new xViewer(nullptr));
		window.Run();
	}

    _CrtDumpMemoryLeaks();
    return 0;
}