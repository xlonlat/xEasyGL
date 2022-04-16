#include "xEasyGL.h"

#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>

//////////////////////////////////////////////////////////////////////////
// Viewer:

Viewer::Viewer(void)
{
	glfwInit();
	glewInit();
}

Viewer::~Viewer(void)
{
}

//////////////////////////////////////////////////////////////////////////
// Application:

Application::Application(const Viewer& viewer)
{
	m_viewer = viewer;
}

Application::~Application()
{

}

void Application::Init()
{

}

void Application::Run()
{

}

void Application::Clear()
{

}
