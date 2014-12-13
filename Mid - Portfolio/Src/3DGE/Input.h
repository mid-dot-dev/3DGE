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
#ifndef _EDGEinput_
	#define _EDGEinput_
#pragma once
//#include "SB6\vmath.h"
#include "3DGE.h"
enum mouse
	{
		 BUTTON_1
		,BUTTON_2
		,BUTTON_3
		,BUTTON_4
		,BUTTON_5
		,BUTTON_6
		,BUTTON_7
		,BUTTON_8
	};
struct MousePress
{
	int X;
	int Y;
};
class Input
{
public:
	Input()
	{
		for(int i=0; i<GLFW_MOUSE_BUTTON_LAST; i++)
		{
			mMouseStates[i]=false;
		}
		for(int i=0; i<GLFW_MOUSE_BUTTON_LAST; i++)
		{
			mDeltaFlag[i]=false;
		}
		for(int i=0; i<GLFW_MOUSE_BUTTON_LAST; i++)
		{
			mMousePress[i].X=0;
			mMousePress[i].Y=0;;
		}
		for(int i=0; i<GLFW_KEY_LAST; i++)
		{
			mKeyStates[i]=false;
		}
		mWheelPos	= 0;
		mMouseX		= 0;
		mMouseY		= 0;
	}
	void updateMouseDeltas()
	{
		for(int i=0; i<GLFW_MOUSE_BUTTON_LAST; i++)
		{
			if(mMouseStates[i] && !mDeltaFlag[i])
			{
				mMousePress[i].X=mMouseX;
				mMousePress[i].Y=mMouseY;
				mDeltaFlag[i] = true;
			}
			else if(!mMouseStates[i])
			{
				mMousePress[i].X=0;
				mMousePress[i].Y=0;
				mDeltaFlag[i] = false;
			}
		}
	}
	
	bool		mKeyStates[GLFW_KEY_LAST];
	bool		mMouseStates[GLFW_MOUSE_BUTTON_LAST];
	bool		mDeltaFlag[GLFW_MOUSE_BUTTON_LAST];
	MousePress  mMousePress[GLFW_MOUSE_BUTTON_LAST];
	int			mWheelPos;
	int			mMouseX;
	int			mMouseY;
};

#endif