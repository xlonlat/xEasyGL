
#ifndef _XEASYGL_H_
#define _XEASYGL_H_

#ifdef XEASYGL_PLATFORM_WINDOWS

#ifdef XEASYGL_EXPORT_DLL
#define XEASYGL_API __declspec(dllexport)
#else
#define XEASYGL_API __declspec(dllimport)
#endif // XEASYGL_EXPORT_DLL

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif // _DEBUG

#else
#error "xEasyGL only support Windows!"
#endif // XEASYGL_PLATFORM_WINDOWS

#include <iostream>
#include <iomanip>

#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xlonlat
{
	namespace xEasyGL
	{
		enum class xEventType
		{
			None = 0,
			MouseDown = 1,
			MouseUp = 2,
			MouseMove = 3,
			MouseWheel = 4,
			KeyDown = 5,
			KeyUp = 6,
			Resize = 7
		};

		// --------------------------val--------------------------
		// type==MouseDown  : 0 is left down, 1 is right down, 2 is middle down;
		// type==MouseUp    : 0 is left up, 1 is right up, 2 is middle up;
		// type==MouseMove  : 0 is left down, 1 is right down, 2 is middle down;
		// type==MouseWheel : 0 is zoom out, 1 is zoom in; 
		// type==KeyDown	: key value;
		// type==KeyUp		: key value;
		// -------------------------------------------------------
		typedef struct
		{
			xEventType	type;
			int			x;
			int			y;
			int			val; 
		} xEvent;

		typedef struct
		{
			int x;
			int y;
			int w;
			int h;
		} xViewportState;

		typedef struct
		{
			float zNear;
			float zFar;
			float fovy;
			float aspect;
		} xProjState;

		typedef struct
		{
			glm::vec3	pos;
			glm::vec3	lookAt;
			glm::vec3	up;
		} xCameraState;

		class xViewer;

		class XEASYGL_API xGlobal
		{
		public:
			static  const xGlobal& Instance();
		private:
			xGlobal();
			xGlobal(const xGlobal&) = delete;
			static xGlobal& m_instance;
		};

		class XEASYGL_API xMath
		{
			///////////////////////////////////////
			// 
		};

		class XEASYGL_API xCommon
		{
		public:
			template<typename T>
			static void PrintMatrix4x4T(T mat[16])
			{
				std::cout.setf(std::ios::fixed);
				std::cout.setf(std::ios::showpos); // show '+' or '-'
				std::cout << std::setprecision(6) <<
					mat[0] << " " << mat[1] << " " << mat[2] << " " << mat[3] << std::endl <<
					mat[4] << " " << mat[5] << " " << mat[6] << " " << mat[7] << std::endl <<
					mat[8] << " " << mat[9] << " " << mat[10] << " " << mat[11] << std::endl <<
					mat[12] << " " << mat[13] << " " << mat[14] << " " << mat[15] << std::endl;
			}
			static void PrintMatrix4x4f(float mat[16])
			{
				PrintMatrix4x4T<float>(mat);
			}
			static void PrintMatrix4x4d(double mat[16])
			{
				PrintMatrix4x4T<double>(mat);
			}
		};

		class XEASYGL_API xEventParser
		{
		public :
			virtual void OnLButtonUp(int cx, int cy){}
			virtual void OnRButtonUp(int cx, int cy){}
			virtual void OnLButtonDown(int cx, int cy){}
			virtual void OnRButtonDown(int cx, int cy){}
			virtual void OnMouseMove(int cx, int cy, int button) {}
			virtual void OnMouseWheel(int cx, int cy, bool zoomin) {}
			virtual void OnSize(int cx, int cy) {}
			virtual void OnKeyUp(int key) {}
			virtual void OnKeyDown(int key) {}
		};

		class XEASYGL_API xDrawArgs
		{
		public:
			xDrawArgs();
			xDrawArgs(const xViewportState& vs, const xProjState& ps);
			virtual ~xDrawArgs();

			const xViewportState& vs() const;
			const xProjState&	  ps() const;

			void  vs(const xViewportState& vs);
			void  ps(const xProjState& ps);
		private:
			xViewportState m_vs;
			xProjState     m_ps;
		};

		class XEASYGL_API xLayer : public xEventParser
		{
		public :
			virtual void	Clear() = 0;
			virtual void	Draw2D(const xDrawArgs& args) = 0;
			virtual void	Draw3D(const xDrawArgs& args) = 0;
		};

		class XEASYGL_API xCamera
		{
		public:
			xCamera();
			virtual ~xCamera();

			virtual void	Pan() {}
			virtual void	Rotate() {}
			virtual void	Zoom() {}

			void  Link(const xViewer* viewer);
			void  State(const xCameraState& state);

			const xCameraState& State() { return m_state; }
		protected:
			xCameraState	m_state;
			const xViewer*		m_viewer;
		};

		class XEASYGL_API xViewer : public xEventParser
		{
		public:
			xViewer(xCamera* camera = nullptr);
			~xViewer(void);

			virtual void	 Initialize();
			virtual void	 Render();
			virtual void	 Clear();
			virtual void	 Event(const xEvent& event);

			const xDrawArgs& DrawArgs() const;

		protected:
			xCamera*	m_camera;
			xDrawArgs*	m_drawArgs;
			void		Begin2D(const xDrawArgs& args);
			void		Begin3D(const xDrawArgs& args);
		};

		class XEASYGL_API xWindow
		{
		public:
			xWindow(xViewer* viewer);
			virtual ~xWindow();

			void    Run();
		private:
			xViewer*	m_viewer;
			GLFWwindow* m_window;
		};
	}
}


#endif // _XEASYGL_H_