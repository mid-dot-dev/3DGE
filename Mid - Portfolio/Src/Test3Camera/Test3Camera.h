
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
portions of the software and/or instructions.

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

//Header Definition
//################################################################################
#ifndef _Test3Camera_
	#define _Test3Camera_
#pragma once
#include "3DGE.h"
class Test2Input : public EDGE::application
{
public:
    Test2Input()
        : program(0),
          wireframe(false),
          paused(false)
    {
    }

    void init()
    {
        static const char title[] = "3DGE - Cubic Bezier Patch Test1";
        EDGE::application::init();
        memcpy(info.title, title, sizeof(title));
    }

    void startup();
    void render(double currentTime);

protected:
	Camera		mCamera;
    GLuint      program;
    GLuint      vao;
    GLuint      buffer;
    vmath::vec3 data[16];

    bool        wireframe;
    bool        paused;
	Input		input;

    void load_shaders();
	virtual void onKey(int key, int action);
    virtual void onMouseButton(int button, int action);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseWheel(int pos);
			void HandleInput(float dt);

    struct
    {
        struct
        {
            int     mv_matrix;
            int     proj_matrix;
            int     mvp;
        } patch;
    } uniforms;
};

void Test2Input::onMouseWheel(int pos)
{
	input.mWheelPos = pos;
}
void Test2Input::onKey(int key, int action)
{
	if(action==GLFW_PRESS)
	{
		input.mKeyStates[key]=true;
	}
	else if(action==GLFW_RELEASE)
	{
		input.mKeyStates[key]=false;
	}
}
void Test2Input::onMouseButton(int button, int action)
{
	if(action==GLFW_PRESS)
	{
		input.mMouseStates[button]=true;
	}
	else if(action==GLFW_RELEASE)
	{
		input.mMouseStates[button]=false;
	}
}
void Test2Input::onMouseMove(int x, int y)
{
	
	input.mMouseX=x;
	input.mMouseY=y;
}

void Test2Input::HandleInput(float dt)
{
	input.updateMouseDeltas();
	if (input.mKeyStates['Y'])
    {
		mCamera.Walk(-10.0f*dt);
	}
	if (input.mKeyStates['H'])
    {
		mCamera.Walk(10.0f*dt);
	}
	if (input.mKeyStates['G'])
    {
		mCamera.Strafe(-10.0f*dt);
	}
	if (input.mKeyStates['J'])
    {
		mCamera.Strafe(10.0f*dt);
	}
	if (input.mKeyStates['T'])
    {
		mCamera.Rise(10.0f*dt);
	}
	if (input.mKeyStates['U'])
    {
		mCamera.Rise(-10.0f*dt);
	}
	if (input.mKeyStates['V'])
    {
		mCamera.Pitch(10.0f*dt);
	}
	if (input.mKeyStates['C'])
    {
		mCamera.Yaw(10.0f*dt);
	}
	if (input.mMouseStates[mouse::BUTTON_1])
	{
		int deltaX = input.mMousePress[mouse::BUTTON_1].X - input.mMouseX;
		int deltaY = input.mMousePress[mouse::BUTTON_1].Y - input.mMouseY;
		if(deltaX ||deltaY)
		{mCamera.Yaw(dt*-deltaX);// diffin y
		mCamera.Pitch(dt*-deltaY);//diff in x
		}
		
	}
}
//Header Definition
//################################################################################
#endif// Test2Input