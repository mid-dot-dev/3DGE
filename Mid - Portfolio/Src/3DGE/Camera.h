
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
#ifndef _EDGEcamera_
	#define _EDGEcamera_
#pragma once
//#include "SB6\vmath.h"
#include "3DGE.h"
class Camera
{
public:
	Camera();
	void Setup(GLfloat fov, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

	void SetPosition(const vmath::vec3& position);
	void SetLookAt(const vmath::vec3& target);
	void SetCenter(const vmath::vec3& center);
	void InitMat();

	void Walk(GLfloat distance);
	void Strafe(GLfloat distance);
	void Rise(GLfloat distance);

	void Yaw(GLfloat degree);
	void Pitch(GLfloat degree);

	const vmath::vec3& GetPosition() const	{ return mPosition; }
	
	vmath::mat4 GetViewMatrix() const;
	vmath::mat4 GetProjectionMatrix() const;
	void UpdateMatrix();

private:
	void Renormalize();

	vmath::vec3 mPosition;
	vmath::vec3 mLookAt;
	vmath::vec3 mRight;
	vmath::vec3 mUp;

	vmath::mat4 mProj;
	vmath::mat4 mMView;
	vmath::vec3 mCenter;
	vmath::mat4 mRot;


	GLfloat mFOV;
	GLfloat mAspectRatio;
	GLfloat mNearPlane;
	GLfloat mFarPlane;
};

#endif
