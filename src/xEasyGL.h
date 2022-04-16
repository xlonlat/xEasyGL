


#ifndef _XEASYGL_H_
#define _XEASYGL_H_

#ifdef XEASYGL_PLATFORM_WINDOWS
#ifdef XEASYGL_EXPORT_DLL
#define XEASYGL_API __declspec(dllexport)
#else
#define XEASYGL_API __declspec(dllimport)
#endif // XEASYGL_EXPORT_DLL
#else
#error "xEasyGL only support Windows!"
#endif // XEASYGL_PLATFORM_WINDOWS

class XEASYGL_API Viewer
{
public:
	Viewer(void);
	~Viewer(void);
};

class XEASYGL_API Application
{
private:
	Viewer	m_viewer;
public:
	Application(const Viewer& viewer);
	virtual ~Application();

	void    Init();
	void    Run();
	void    Clear();
};

#endif // _XEASYGL_H_