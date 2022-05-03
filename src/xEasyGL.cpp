#include "xEasyGL.h"

#ifdef XEASYGL_PLATFORM_WINDOWS
#include <windows.h>
#endif // XEASYGL_PLATFORM_WINDOWS

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

namespace xlonlat
{
	namespace xEasyGL
	{
		xGlobal& xGlobal::m_instance = xGlobal::xGlobal();

		xGlobal::xGlobal()
		{
			//char* aaa = (char*)(new char[4]);
			//char* bbb = (char*)malloc(4);

			wchar_t szFilePath[MAX_PATH+1] = { 0 };
			GetModuleFileNameW(NULL, szFilePath, MAX_PATH);
			(wcsrchr(szFilePath, ('\\')))[1] = 0;
			
			memset(m_resourcePath, 0, sizeof(m_resourcePath));
			wcscpy(m_resourcePath, szFilePath);
			wcscat(m_resourcePath, L"..\\..\\res\\");
		}

		xGlobal::xGlobal(const xGlobal&)
		{
			memset(m_resourcePath, 0, sizeof(m_resourcePath));
		}

		const xGlobal& xGlobal::Instance()
		{
			return m_instance;
		}

		const wchar_t* xGlobal::ResourcePath() const
		{
			return m_resourcePath;
		}

		unsigned char* xGlobal::ReadImage(const wchar_t* file, int& width, int& height, int& channel) const
		{
			int len = WideCharToMultiByte(CP_ACP, 0, file, (int)wcslen(file), NULL, 0, NULL, NULL);
			char* achar = new char[len + 1];
			WideCharToMultiByte(CP_ACP, 0, file, (int)wcslen(file), achar, len, NULL, NULL);
			achar[len] = '\0';
			unsigned char* data = stbi_load(achar, &width, &height, &channel, 0);
			delete[] achar;
			return data;
		}
	}
}
