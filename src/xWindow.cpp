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
            event.val = 0;

            viewer->Event(event);
            viewer->OnSize(width, height);
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
            event.val = btn;

            xMousePos mouse{};
            mouse.x = (int)x;
            mouse.y = (int)y;

            viewer->Event(event);
            switch (event.type)
            {
            case xEasyGL::MouseUp:
            {
                     if (btn == 0) viewer->OnLButtonUp(mouse);
                else if (btn == 1) viewer->OnRButtonUp(mouse);
            }break;
            case xEasyGL::MouseDown:
            {
                     if (btn == 0) viewer->OnLButtonDown(mouse);
                else if (btn == 1) viewer->OnRButtonDown(mouse);
            }break;
            default:
                break;
            }
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
            event.val = btn;

            xMousePos mouse{};
            mouse.x = (int)x;
            mouse.y = (int)y;

            viewer->Event(event);
            viewer->OnMouseMove(mouse, btn);
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
            event.val = yoff > 0 ? 1 : 0;

            xMousePos mouse{};
            mouse.x = (int)x;
            mouse.y = (int)y;

            viewer->Event(event);
            viewer->OnMouseWheel(mouse, yoff > 0);
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
				event.val = key;

				viewer->Event(event);

				if (action == GLFW_PRESS)          
					viewer->OnKeyDown(key);
				else if (action == GLFW_RELEASE)           
					viewer->OnKeyUp(key);
			}
		}

		xWindow::xWindow(xViewer* viewer) : m_viewer(viewer), m_window(nullptr)
		{
		}

		xWindow::~xWindow()
		{

		}

		void xWindow::Run()
		{
            int width = m_viewer->DrawArgs().vs().w;
            int height = m_viewer->DrawArgs().vs().h;

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
			glfwSetKeyCallback(m_window, OnKeyEvent);
            glfwSetScrollCallback(m_window, OnMouseWheel);
            glfwSetCursorPosCallback(m_window, OnMouseMove);
            glfwSetMouseButtonCallback(m_window, OnMouseEvent);
            glfwSetFramebufferSizeCallback(m_window, OnSize);
			glfwSwapInterval(1);

            // After MakeContextCurrent. 
            {
                glewInit();
            }

            m_viewer->Initialize();

			double deltaTime = 0.0, currFrame = 0.0, lastFrame = 0.0;
            while (!glfwWindowShouldClose(m_window))
            {
				currFrame = glfwGetTime();
				if (lastFrame == 0.0) lastFrame = currFrame;
				deltaTime = currFrame - lastFrame;
				lastFrame = currFrame;

                if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(m_window, true);

                m_viewer->Render(deltaTime);

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
