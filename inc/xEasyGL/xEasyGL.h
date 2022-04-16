#pragma once

#ifdef XEASYGL_PLATFORM_WINDOWS
#ifdef XEASYGL_EXPORT_DLL
#define XEASYGL_API __declspec(dllexport)
#else
#define XEASYGL_API __declspec(dllimport)
#endif // XEASYGL_EXPORT_DLL
#else
#error "xEasyGL only support Windows!"
#endif // XEASYGL_PLATFORM_WINDOWS

class XEASYGL_API iViewer
{
public:
	iViewer(void);
	~iViewer(void);
};

