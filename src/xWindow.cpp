#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{

        void OnSize(GLFWwindow* window, int width, int height)
        {
            xViewer* viewer = (xViewer*)glfwGetWindowUserPointer(window);
            assert(viewer != nullptr);

            xEvent event{};
            event.type = xEasyGL::Resize;
            event.x = width;
            event.y = height;
            event.tag = 0;

            viewer->Event(event);
        }
        
        // @param[button] 0:Left | 1:Right | 2:Middle.
        // @param[action] 0:GLFW_RELEASE | 1:GLFW_PRESS.
        void OnMouseEvent(GLFWwindow* window, int button, int action, int pos)
        {
            xViewer* viewer = (xViewer*)glfwGetWindowUserPointer(window);
            assert(viewer != nullptr);

            double x = 0, y = 0;
            glfwGetCursorPos(window, &x, &y);

			int btn = -1;
			if (button == GLFW_MOUSE_BUTTON_LEFT)   btn = 0;
			if (button == GLFW_MOUSE_BUTTON_RIGHT)  btn = 1;
			if (button == GLFW_MOUSE_BUTTON_MIDDLE) btn = 2;

            xEvent event{};
            event.type = action == GLFW_RELEASE ? xEasyGL::MouseUp : xEasyGL::MouseDown;
            event.x = (int)x;
            event.y = (int)y;
            event.tag = btn;

            glm::ivec2 mouse{};
            mouse.x = (int)x;
            mouse.y = (int)y;

            viewer->Event(event);
        }

        void OnMouseMove(GLFWwindow* window, double x, double y)
        {
            xViewer* viewer = (xViewer*)glfwGetWindowUserPointer(window);
            assert(viewer != nullptr);

            int btn = -1;
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)   == 1) btn = 0;
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)  == 1) btn = 1;
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == 1) btn = 2;

            xEvent event{};
            event.type = xEasyGL::MouseMove;
            event.x = (int)x;
            event.y = (int)y;
            event.tag = btn;

            glm::ivec2 mouse{};
            mouse.x = (int)x;
            mouse.y = (int)y;

            viewer->Event(event);
        }

         // @param[yoff] 1:zoomin | -1:zoomout.
        void OnMouseWheel(GLFWwindow* window, double xoff, double yoff)
        {
            xViewer* viewer = (xViewer*)glfwGetWindowUserPointer(window);
            assert(viewer != nullptr);

            double x = 0, y = 0;
            glfwGetCursorPos(window, &x, &y);

            xEvent event{};
            event.type = xEasyGL::MouseWheel;
            event.x = (int)x;
            event.y = (int)y;
            event.tag = yoff > 0 ? 1 : -1;

            glm::ivec2 mouse{};
            mouse.x = (int)x;
            mouse.y = (int)y;

            viewer->Event(event);
        }

		void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);

            xViewer* viewer = (xViewer*)glfwGetWindowUserPointer(window);
            assert(viewer != nullptr);

			double x = 0, y = 0;
			glfwGetCursorPos(window, &x, &y);

			if (action == GLFW_PRESS || action == GLFW_RELEASE)
			{
                xEvent event{};
				event.type = action == GLFW_PRESS ? xEasyGL::KeyDown : xEasyGL::KeyUp;
				event.x = (int)x;
				event.y = (int)y;
				event.tag = key;

				viewer->Event(event);
			}
		}


		xWindow::xWindow(const char* title, uint32_t width, uint32_t height, xViewer* viewer) :
            m_pViewer(viewer), 
            m_pWindow(nullptr)
        {
            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            // Setup the render mode: GLFW_OPENGL_CORE_PROFILE or GLFW_OPENGL_COMPAT_PROFILE.
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

            m_pWindow = glfwCreateWindow(width, height, "xApp", nullptr, nullptr);
            if (m_pWindow == nullptr)
            {
                glfwTerminate();
                return;
            }

            glfwMakeContextCurrent(m_pWindow);
            glfwSetWindowUserPointer(m_pWindow, (void*)m_pViewer);
            glfwSetKeyCallback(m_pWindow, OnKeyEvent);
            glfwSetScrollCallback(m_pWindow, OnMouseWheel);
            glfwSetCursorPosCallback(m_pWindow, OnMouseMove);
            glfwSetMouseButtonCallback(m_pWindow, OnMouseEvent);
            glfwSetFramebufferSizeCallback(m_pWindow, OnSize);
            glfwSwapInterval(1);

            // After MakeContextCurrent. 
            {
                glewInit();
            }

            OnSize(m_pWindow, width, height);

            if (m_pViewer)
            {
                m_pViewer->Initialize();
            }
		}

		xWindow::~xWindow()
		{
            if (m_pViewer)
            {
                m_pViewer->Clear();
            }

            glfwTerminate();
            m_pWindow = nullptr;

            if (m_pViewer)
            {
                delete m_pViewer;
                m_pViewer = nullptr;
            }
		}

		void xWindow::Run()
		{
            while (m_pWindow && !glfwWindowShouldClose(m_pWindow))
            {
                if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(m_pWindow, true);

                if (m_pViewer)
                {
                    m_pViewer->Render();
                }

                glfwSwapBuffers(m_pWindow);
                glfwPollEvents();
            }
		}

	}
}
