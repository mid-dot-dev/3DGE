/*
################################################################################

Copyright © 2014-2015 Mid (Midhun Kumar Pallipurath)
Aspiring Game Programmer
Undergraduate Portfolio Project @ the Art Institute of Vancouver
Bachelors of Science in Visual and Game Programming

################################################################################

Copyright Notice:
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the fact that this copyright 
notice and this permission notice shall be included in all copies or 
portions of the software and/or documentation files.

DISCLAIMER:
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

################################################################################
*/

//Includes
//################################################################################
#include "3DGE.h"

void EDGE::application::run(EDGE::application* the_app)
{
	bool running = true;
	app = the_app;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}

	init();

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, info.majorVersion);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, info.minorVersion);

#ifdef _DEBUG
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif /* _DEBUG */
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, info.samples);
	glfwOpenWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
	if (info.flags.fullscreen)
	{
		if (info.windowWidth == 0 || info.windowHeight == 0)
		{
			GLFWvidmode mode;
			glfwGetDesktopMode(&mode);
			info.windowWidth = mode.Width;
			info.windowHeight = mode.Height;
		}
		glfwOpenWindow(info.windowWidth, info.windowHeight, 8, 8, 8, 0, 32, 0, GLFW_FULLSCREEN);
		glfwSwapInterval((int)info.flags.vsync);
	}
	else
	{
		if (!glfwOpenWindow(info.windowWidth, info.windowHeight, 8, 8, 8, 0, 32, 0, GLFW_WINDOW))
		{
			fprintf(stderr, "Failed to open window\n");
			return;
		}
	}

	glfwSetWindowTitle(info.title);
	glfwSetWindowSizeCallback(glfw_onResize);
	glfwSetKeyCallback(glfw_onKey);
	glfwSetMouseButtonCallback(glfw_onMouseButton);
	glfwSetMousePosCallback(glfw_onMouseMove);
	glfwSetMouseWheelCallback(glfw_onMouseWheel);
	(info.flags.cursor ? glfwEnable : glfwDisable)(GLFW_MOUSE_CURSOR);

	info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

	gl3wInit();

#ifdef _DEBUG
	OutputDebugStringA("################################\n");
	OutputDebugStringA("## 3DGE running in Debug Mode ##\n");
	OutputDebugStringA("################################\n");
	OutputDebugStringA("VENDOR: ");
	OutputDebugStringA((char *)glGetString(GL_VENDOR));OutputDebugStringA("\n");
	OutputDebugStringA("VERSION: ");
	OutputDebugStringA((char *)glGetString(GL_VERSION));OutputDebugStringA("\n");
	OutputDebugStringA("RENDERER: ");
	OutputDebugStringA((char *)glGetString(GL_RENDERER));OutputDebugStringA("\n");
	OutputDebugStringA("################################\n");
#endif /* _DEBUG */

	if (info.flags.debug)
	{
		if (gl3wIsSupported(4, 3))
		{
			glDebugMessageCallback(debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
	}

	startup();

	do
	{
		render(glfwGetTime());

		glfwSwapBuffers();

		running &= (glfwGetKey( GLFW_KEY_ESC ) == GLFW_RELEASE);
		running &= (glfwGetWindowParam( GLFW_OPENED ) != GL_FALSE);

	} while(running);

	shutdown();

	glfwTerminate();
}

void EDGE::application::init()
{
	strcpy(info.title, "3DGE (3D Graphic Engine)");
	info.windowWidth = 800;
	info.windowHeight = 600;
	info.majorVersion = 4;
	info.minorVersion = 3;
	info.samples = 0;
	info.flags.all = 0;
	info.flags.cursor = 1;
	setVsync(true);
	#ifdef _DEBUG
		info.flags.debug = 1;
	#endif /* _DEBUG */
}

void EDGE::application::onResize(int w, int h)
		{
			info.windowWidth = w;
			info.windowHeight = h;
		}

void EDGE::application::getMousePosition(int& x, int& y)
		{
			glfwGetMousePos(&x, &y);
		}

void EDGE::application::onDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message)
{
	#ifdef _WIN32
		OutputDebugStringA(message);
		OutputDebugStringA("\n");
	#endif /* _WIN32 */
}

EDGE::application * EDGE::application::app = 0;

