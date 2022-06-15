#ifndef XEASYGL_HEADER
#define XEASYGL_HEADER

#ifdef XEASYGL_PLATFORM_WINDOWS

#ifdef XEASYGL_EXPORT_DLL
#define XEASYGL_API __declspec(dllexport)
#else
#define XEASYGL_API __declspec(dllimport)
#endif // XEASYGL_EXPORT_DLL

#else
#error "xEasyGL only support Windows!"
#endif // XEASYGL_PLATFORM_WINDOWS

#include <codecvt> 
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

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

		// --------------------------tag--------------------------
		// type==MouseDown  : 0 is left down, 1 is right down, 2 is middle down;
		// type==MouseUp    : 0 is left up, 1 is right up, 2 is middle up;
		// type==MouseMove  : 0 is left down, 1 is right down, 2 is middle down;
		// type==MouseWheel : -1 is zoom out, 1 is zoom in; 
		// type==KeyDown	: key value, same as GLFW;
		// type==KeyUp		: key value, same as GLFW;
		// -------------------------------------------------------
		struct xEvent
		{
			xEventType	type;
			int			x;
			int			y;
			int			tag; 
		};

		struct xViewportState
		{
			int		x;
			int		y;
			int		w;
			int		h;
		};

		struct xProjState
		{
			float	near;
			float	far;
			float	fovy;
			float	aspect;
		};

		struct xCameraState
		{
			glm::vec3		eye;
			glm::vec3		tar;
			glm::vec3		up;
			xProjState		ps;
			xViewportState	vs;
		};

		class xViewer;

		class XEASYGL_API xGlobal
		{
		public:
			static   const xGlobal&	Instance();
			const	 wchar_t*		ResourcePath() const;
			unsigned char*			ReadImage(const wchar_t* file, int& width, int& height, int& channel) const;

			int		GetCurrentThread() const;

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
		public:
			virtual void Event(const xEvent& event);
		protected:
			virtual void OnEvent(const xEvent& event) {}
			virtual void OnSize(int width, int height) {}
			virtual void OnKeyUp(int key) {}
			virtual void OnKeyDown(int key) {}
			virtual void OnLButtonUp(int cx, int cy) {}
			virtual void OnRButtonUp(int cx, int cy) {}
			virtual void OnMButtonUp(int cx, int cy) {}
			virtual void OnLButtonDown(int cx, int cy) {}
			virtual void OnRButtonDown(int cx, int cy) {}
			virtual void OnMButtonDown(int cx, int cy) {}
			virtual void OnMouseMove(int cx, int cy, int button) {}
			virtual void OnMouseWheel(int cx, int cy, int zoom) {}
		};

		class XEASYGL_API xUniformBuffer
		{
		public:
			xUniformBuffer(uint32_t size, uint32_t binding);
			virtual ~xUniformBuffer();

			void		Bind()		const;
			void		Unbind()	const;
			void		Update(const void* data) const;

		protected:
			uint32_t	m_ID;
			uint32_t	m_Binding;
			uint32_t	m_Size;
		};

		class XEASYGL_API xShader
		{
		public:
			xShader();
			~xShader();

			void	Clear();
			bool	Load(const wchar_t* vs_file, const wchar_t* fs_file);
			bool	Load(const char* vs_code, const char* fs_code);
			void	GetInfo(char info[512]) const;
			void	Enable()	const;
			void	Disable()	const;

			GLuint	ID()	const;
		protected:
			GLuint	m_id;
			GLuint	m_vs;
			GLuint	m_fs;
		};

		class XEASYGL_API xShaderToy : public xEventParser
		{
		public:
			xShaderToy(const wchar_t* file);
			virtual ~xShaderToy();

			void	Draw(const xViewer* viewer);

		private:
			virtual void	OnEvent(const xEvent& event) override;

		private:
			xShader*		m_Shader;
			struct 
			{
				glm::vec3   iResolution;			// viewport resolution (in pixels)
				float		iTime;					// shader playback time (in seconds)
				float		iTimeDelta;				// render time (in seconds)
				int			iFrame;					// shader playback frame
				float		iChannelTime[4];		// channel playback time (in seconds)
				glm::vec3   iChannelResolution[4];	// channel resolution (in pixels)
				glm::vec4   iMouse;					// mouse pixel coords. xy: current (if MLB down), zw: click
				glm::vec4   iDate;                  // (year, month, day, time in seconds)
				float		iSampleRate;			// sound sample rate (i.e., 44100)
				int			iChannel0;				// input channel. XX = 2D/Cube
				int			iChannel1;				// input channel. XX = 2D/Cube
				int			iChannel2;				// input channel. XX = 2D/Cube
				int			iChannel3;				// input channel. XX = 2D/Cube
			} m_Block;
			struct 
			{
				glm::vec3	m_MousePosition;
			};
			clock_t			m_StartClock;
			uint32_t		m_FrameCount;
		};

		class XEASYGL_API xTexture
		{
		public:
			xTexture();
			~xTexture();

			void	Clear();										// Called by render process.
			bool	Load(const wchar_t* file);						// Called by render process.
			void	Load(const unsigned char* data, GLenum format);	// Called by render process.

			GLuint	ID()		const;
			size_t	Width()		const;
			size_t	Height()	const;
			size_t	Channel()	const;
			bool	Available()	const;
		private:
			GLuint	m_id;
			int		m_width;
			int		m_height;
			int		m_channel;
		};

		class XEASYGL_API xCamera : public xEventParser
		{
		public:
			xCamera();
			virtual ~xCamera();

			double		Timespan() const;
			float		PixelSize(const xCameraState& state, float dis2cam) const;

			const	xCameraState& State() const;

			glm::mat4 ProjMat()	const;
			glm::mat4 ViewMat()	const;

			virtual void	Update();
			virtual void	SetState(const xCameraState& state);

		protected:
			double			m_Timespan;
			const xViewer*	m_Viewer;
			xCameraState	m_State; 
			xCameraState	m_StatePre;
			clock_t			m_LastClock;
			uint32_t		m_FrameCounter;
		};

		class XEASYGL_API xDefaultCamera : public xCamera
		{
		public:
			xDefaultCamera();
			virtual ~xDefaultCamera();

		protected:
			virtual void	OnSize(int width, int height) override;

			virtual void	OnLButtonUp(int cx, int cy);
			virtual void	OnRButtonUp(int cx, int cy);
			virtual void	OnMouseMove(int cx, int cy, int button);
			virtual void	OnMouseWheel(int cx, int cy, int zoom);
			
		private:
			glm::ivec2		m_lastLDown;
			glm::ivec2		m_lastRDown;
		};

		class XEASYGL_API xLayer : public xEventParser
		{
		public:
			xLayer(const xViewer* viewer):m_pViewer(viewer) {}
			virtual void	OnAttach() {}
			virtual void	OnDetach() {}
			virtual void	OnDraw2D() {}
			virtual void	OnDraw3D() {}
		protected:
			const xViewer* m_pViewer;
		};

		class XEASYGL_API xViewer : public xEventParser
		{
		public:
			xViewer(xCamera* camera = nullptr);
			~xViewer(void);

			xCamera&        Camera()	const;

			void			AttachLayer(xLayer* layer);
			void			DetachLayer(xLayer* layer);

			virtual void	Initialize();
			virtual void	Render();
			virtual void	Clear();

		private:
			virtual void	OnEvent(const xEvent& event) override;

		protected:
			xCamera*	m_camera;
			int			m_RenderThreadID;
			xTexture	m_logoImg;
			xShader		m_sampleShader;
			struct
			{
				std::vector<xLayer*>	m_Layers;
			};

			void		Begin2D();
			void		Begin3D();
		};

		class XEASYGL_API xWindow
		{
		public:
			xWindow(const char* title, uint32_t width, uint32_t height, xViewer* viewer);
			virtual ~xWindow();

			void    Run();
		private:
			xViewer*	m_pViewer;
			GLFWwindow* m_pWindow;
		};
	}
}
#endif // XEASYGL_HEADER