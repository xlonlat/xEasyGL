#include "xEasyGL.h"

#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

double	m_mousex = 0.0;
double	m_mousey = 0.0;

void OnSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	GLFWwindow* pWindow = NULL;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/*
	 * Setup the render mode.
	 * GLFW_OPENGL_CORE_PROFILE.
	 * GLFW_OPENGL_COMPAT_PROFILE.
	 */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	pWindow = glfwCreateWindow(800, 600, "xApplication", NULL, NULL);
	if (pWindow == NULL)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, OnSize);

	/* After MakeContextCurrent. */
	{
		glewInit();	
	}

	OnSize(pWindow, 800, 600);

	int nFPS = 0;
	double last = 0.0, curr = 0.0, delta = 0.0;

	while (!glfwWindowShouldClose(pWindow))
	{
		if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(pWindow, true);

		const GLubyte* pstrVersion = glGetString(GL_VERSION);
		const GLubyte* name = glGetString(GL_VENDOR);
		const GLubyte* biaoshifu = glGetString(GL_RENDERER);
		const GLubyte* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);


		glm::vec3 pos(0, 0, 1);
		glm::vec3 tar(0, 0, 0);
		glm::vec3 up(0, 1, 0);
		glm::mat4 model = glm::lookAtRH(pos, tar, up);

		static int print = 0;
		if (!print++)
		{
			float marray[16] = { 0 };
			memcpy(marray, glm::value_ptr(model), sizeof(float) * 16);

			printf("   %+0.6lf   %+0.6lf   %+0.6lf   %+0.6lf;\n", marray[0], marray[1], marray[2], marray[3]);
			printf("   %+0.6lf   %+0.6lf   %+0.6lf   %+0.6lf;\n", marray[4], marray[5], marray[6], marray[7]);
			printf("   %+0.6lf   %+0.6lf   %+0.6lf   %+0.6lf;\n", marray[8], marray[9], marray[10], marray[11]);
			printf("   %+0.6lf   %+0.6lf   %+0.6lf   %+0.6lf;\n", marray[12], marray[13], marray[14], marray[15]);
		}

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}