#include "xEasyGL.h"

#include <GLEW/GL/glew.h>

//////////////////////////////////////////////////////////////////////////
// Viewer:

Viewer::Viewer(void)
{
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
