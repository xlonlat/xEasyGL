
#ifndef _XEASYGL_H_
#define _XEASYGL_H_

#ifdef XEASYGL_PLATFORM_WINDOWS

#ifdef XEASYGL_EXPORT_DLL
#define XEASYGL_API __declspec(dllexport)
#else
#define XEASYGL_API __declspec(dllimport)
#endif // XEASYGL_EXPORT_DLL

#ifdef _DEBUG
#define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#else
#error "xEasyGL only support Windows!"
#endif // XEASYGL_PLATFORM_WINDOWS

#include <iostream>
#include <iomanip>
#include <string>

#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xlonlat
{
	namespace xEasyGL
	{
		enum xEventType 
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
		// type==KeyDown	: key value, same as GLFW;
		// type==KeyUp		: key value, same as GLFW;
		// -------------------------------------------------------
		struct xEvent
		{
			xEventType	type;
			int			x;
			int			y;
			int			val; 
		};

		struct xViewportState
		{
			int x;
			int y;
			int w;
			int h;
		};

		struct xProjState
		{
			float zNear;
			float zFar;
			float fovy;
			float aspect;
		};

		struct xCameraState
		{
			glm::vec3	pos;
			glm::vec3	lookAt;
			glm::vec3	up;
		};

		struct xMousePos
		{
			int x;
			int y;
		};

		class xViewer;

		class XEASYGL_API xGlobal
		{
		public:
			static  const xGlobal&	Instance();

			const	std::wstring&	ResourcePath() const;

			unsigned char*			ReadImage(const std::wstring& file, int& width, int& height, int& channel) const;

		private:
			xGlobal();
			xGlobal(const xGlobal&); //xGlobal(const xGlobal&) = delete; vs2010 not support "=delete"; 
			static xGlobal& m_instance;

			wchar_t	m_resourcePath[1024];
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
			virtual void OnLButtonUp(const xMousePos& pos){}
			virtual void OnRButtonUp(const xMousePos& pos){}
			virtual void OnLButtonDown(const xMousePos& pos){}
			virtual void OnRButtonDown(const xMousePos& pos){}
			virtual void OnMouseMove(const xMousePos& pos, int button) {}
			virtual void OnMouseWheel(const xMousePos& pos, bool zoomin) {}
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

			virtual void	Pan(const xMousePos& pos0, const xMousePos& pos1, int param = 0);
			virtual void	Rotate(const xMousePos& pos0, const xMousePos& pos1, int param = 0);
			virtual void	Zoom(const xMousePos& pos, bool zoomin, int param = 0);

			void  Update();	// Called by render process.
			void  Link(const xViewer* viewer);
			void  State(const xCameraState& state);

			const xCameraState& State() { return m_state; }
		protected:
			xCameraState		m_state;
			xCameraState		m_stateBak;
			const xViewer*		m_viewer;
		};

		class XEASYGL_API xViewer : public xEventParser
		{
		public:
			xViewer(xCamera* camera = nullptr);
			~xViewer(void);

			xDrawArgs& DrawArgs() const;

			virtual void	 Initialize();
			virtual void	 Render(double interval);
			virtual void	 Clear();
			virtual void	 Event(const xEvent& event);

			virtual void	 OnLButtonUp(const xMousePos& pos) override;
			virtual void	 OnRButtonUp(const xMousePos& pos) override;
			virtual void	 OnMouseMove(const xMousePos& pos, int button) override;
			virtual void	 OnMouseWheel(const xMousePos& pos, bool zoomin) override;

		protected:
			xCamera*	m_camera;
			xDrawArgs*	m_drawArgs;
			xMousePos   m_lastLDown;
			xMousePos   m_lastRDown;

			void		Begin2D();
			void		Begin3D();
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