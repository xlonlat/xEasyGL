
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

#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xlonlat
{
	namespace xEasyGL
	{
		typedef struct
		{
			float width;
			float heigth;
		}xViewportState;

		typedef struct
		{
			float zNear;
			float zFar;
			float fovy;
			float aspect;
		}xProjectState;

		class XEASYGL_API xDrawArgs
		{
		public:
			xDrawArgs();
			xDrawArgs(const xViewportState& vs, const xProjectState& ps);
			virtual ~xDrawArgs();

			const xViewportState* vs();
			const xProjectState*  ps();
		private:
			xViewportState m_vs;
			xProjectState  m_ps;
		};

		class XEASYGL_API xViewer
		{
		public:
			xViewer(void);
			~xViewer(void);

			virtual void	 Initialize();
			virtual void	 Render();
			virtual void	 Clear();

			const xDrawArgs* DrawArgs();
		private:
			xDrawArgs	m_drawArgs;
		};

		class XEASYGL_API xApplication
		{
		public:
			xApplication(const xViewer& viewer);
			virtual ~xApplication();

			void    Run();
		private:
			xViewer		m_viewer;
			GLFWwindow* m_window;
		};
	}
}


#endif // _XEASYGL_H_