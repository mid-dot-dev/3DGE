
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

#include "Camera.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Camera::Camera()
	: mFOV(50.0f)
	, mAspectRatio(4.0f / 3.0f)
	, mNearPlane(0.01f)
	, mFarPlane(1000.0f)
	, mRot(0.0f)
{
	mPosition = vmath::vec3(0.0f, 0.0f, -4.0f);
	mLookAt = vmath::vec3(0.0f, 0.0f, 1.0f);
	mUp = vmath::vec3(0.0f, 1.0f, 0.0f);
	Renormalize();
	mProj = vmath::mat4::identity();
	mMView = vmath::mat4::identity();
	mRot = vmath::mat4::identity();
}

//----------------------------------------------------------------------------------------------------

void Camera::Setup(GLfloat fov, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane)
{
	mFOV = fov;
	mAspectRatio = aspectRatio;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetPosition(const vmath::vec3& position)
{
	mPosition = position;
}


//----------------------------------------------------------------------------------------------------

void Camera::SetLookAt(const vmath::vec3& target)
{
	mLookAt = target;
	Renormalize();
}
void Camera::InitMat()
{
	UpdateMatrix();
}
//----------------------------------------------------------------------------------------------------

void Camera::Walk(GLfloat distance)
{
	mPosition += vmath::vec3(0.0f,0.0f,distance);
	mLookAt += vmath::vec3(0.0f,0.0f,distance);
	UpdateMatrix();
}

//----------------------------------------------------------------------------------------------------

void Camera::Strafe(GLfloat distance)
{
	mPosition += vmath::vec3(distance,0.0f,0.0f);
	mLookAt += vmath::vec3(distance,0.0f,0.0f);
	UpdateMatrix();
}

//----------------------------------------------------------------------------------------------------

void Camera::Rise(GLfloat distance)
{
	mPosition += vmath::vec3(0.0f,distance,0.0f);
	mLookAt += vmath::vec3(0.0f,distance,0.0f);
	UpdateMatrix();
}

//----------------------------------------------------------------------------------------------------

void Camera::Pitch(GLfloat degree)
{
	Renormalize();

	mRot =  vmath::rotate(degree, mRight) * mRot;
	Renormalize();
	UpdateMatrix();
}
void Camera::Yaw(GLfloat degree)
{
	Renormalize();
	//vmath::mat4 test = vmath::rotate(degree, mUp);
	//vmath::vec4 test2 = vmath::vec4(mLookAt[0], mLookAt[1], mLookAt[2], 1.0f);
	//test2 = test2*test;
	//mLookAt[0]=test2[0]; mLookAt[1]=test2[1]; mLookAt[2]=test2[2];
	mRot =  vmath::rotate(degree, mUp) * mRot;
	Renormalize();
	UpdateMatrix();
}

//----------------------------------------------------------------------------------------------------

vmath::mat4 Camera::GetViewMatrix() const
{
	return mMView;
}

//----------------------------------------------------------------------------------------------------
void Camera::UpdateMatrix()
{
	mProj = vmath::perspective(mFOV, mAspectRatio, mNearPlane, mFarPlane);

	vmath::vec3 viewZ = mLookAt-mPosition;
	viewZ = vmath::normalize(viewZ);
	vmath::vec3 viewX = vmath::cross(viewZ, mUp);
	viewX = vmath::normalize(viewX);
	vmath::vec3 viewY = vmath::cross(viewX, viewZ);
	viewY = vmath::normalize(viewY);

	vmath::mat4 rotation = vmath::mat4::identity();
	rotation[0][0]=viewX[0];
	rotation[0][1]=viewX[1];
	rotation[0][2]=viewX[2];

	rotation[1][0]=viewY[0];
	rotation[1][1]=viewY[1];
	rotation[1][2]=viewY[2];

	rotation[2][0]=viewZ[0];
	rotation[2][1]=viewZ[1];
	rotation[2][2]=viewZ[2];

	vmath::mat4 translation = vmath::translate(mPosition);
	mMView = vmath::mat4::identity();
	mMView = mRot*translation*mMView;

}
vmath::mat4 Camera::GetProjectionMatrix() const
{
	return mProj;
}

//----------------------------------------------------------------------------------------------------

void Camera::Renormalize()
{
	mUp = vmath::vec3(0.0f, 1.0f, 0.0f);
	mLookAt = vmath::normalize(mLookAt);
	mRight = (vmath::cross(mUp, mLookAt));
}