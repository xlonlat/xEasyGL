#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xGlobal& xGlobal::m_instance = xGlobal::xGlobal();

		xGlobal::xGlobal()
		{
			// Initialize something...
			char* aaa = (char*)(new char[4]);
			char* bbb = (char*)malloc(4);

			memset(m_resourcePath, 0, sizeof(m_resourcePath));
		}

		xGlobal::xGlobal(const xGlobal&)
		{
			memset(m_resourcePath, 0, sizeof(m_resourcePath));
		}

		const xGlobal& xGlobal::Instance()
		{
			return m_instance;
		}

		const wchar_t* xGlobal::ResourcePath()
		{
			return m_resourcePath;
		}
	}
}
