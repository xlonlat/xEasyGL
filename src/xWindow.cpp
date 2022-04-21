#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
        void OnSize(GLFWwindow* window, int width, int height)
        {
            xViewer* viewer = nullptr;
            assert(viewer = (xViewer*)glfwGetWindowUserPointer(window));

            xEvent event;
            event.type = xEventType::Resize;
            event.x = width;
            event.y = height;
            event.val = 0;

            viewer->Event(event);
        }
        
        // @param[button] 0:Left | 1:Right | 2:Middle.
        // @param[action] 0:GLFW_RELEASE | 1:GLFW_PRESS.
        void OnMouseEvent(GLFWwindow* window, int button, int action, int pos)
        {
            xViewer* viewer = nullptr;
            assert(viewer = (xViewer*)glfwGetWindowUserPointer(window));

            double x = 0, y = 0;
            glfwGetCursorPos(window, &x, &y);

            xEvent event;
            event.type = action == GLFW_RELEASE ? xEventType::MouseUp : xEventType::MouseDown;
            event.x = (int)x;
            event.y = (int)y;
            event.val = button;

            viewer->Event(event);
        }

        void OnMouseMove(GLFWwindow* window, double x, double y)
        {
            xViewer* viewer = nullptr;
            assert(viewer = (xViewer*)glfwGetWindowUserPointer(window));

            xEvent event;
            event.type = xEventType::MouseMove;
            event.x = (int)x;
            event.y = (int)y;
            event.val = -1;

            viewer->Event(event);
        }

         // @param[yoff] 1:zoomin | -1:zoomout.
        void OnMouseWheel(GLFWwindow* window, double xoff, double yoff)
        {
            xViewer* viewer = nullptr;
            assert(viewer = (xViewer*)glfwGetWindowUserPointer(window));

            double x = 0, y = 0;
            glfwGetCursorPos(window, &x, &y);

            xEvent event;
            event.type = xEventType::MouseWheel;
            event.x = (int)x;
            event.y = (int)y;
            event.val = yoff > 0 ? 1 : 0;

            viewer->Event(event);
        }

		xWindow::xWindow(xViewer* viewer) : m_viewer(viewer), m_window(nullptr)
		{
		}

		xWindow::~xWindow()
		{

		}

		void xWindow::Run()
		{
            int width = m_viewer->DrawArgs().vs().width;
            int height = m_viewer->DrawArgs().vs().height;

            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            // Setup the render mode: GLFW_OPENGL_CORE_PROFILE or GLFW_OPENGL_COMPAT_PROFILE.
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

            m_window = glfwCreateWindow(width, height, "xApp", nullptr, nullptr);
            if (m_window == nullptr)
            {
                glfwTerminate();
                return;
            }

            glfwMakeContextCurrent(m_window);
            glfwSetWindowUserPointer(m_window, (void*)m_viewer);
            glfwSetScrollCallback(m_window, OnMouseWheel);
            glfwSetCursorPosCallback(m_window, OnMouseMove);
            glfwSetMouseButtonCallback(m_window, OnMouseEvent);
            glfwSetFramebufferSizeCallback(m_window, OnSize);

            // After MakeContextCurrent. 
            {
                glewInit();
            }

            m_viewer->Initialize();

            while (!glfwWindowShouldClose(m_window))
            {
                if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(m_window, true);

                m_viewer->Render();

                glfwSwapBuffers(m_window);
                glfwPollEvents();
            }

            m_viewer->Clear();

            glfwTerminate();

            delete m_viewer;
            m_viewer = nullptr;
            m_window = nullptr;
		}
	}
}
