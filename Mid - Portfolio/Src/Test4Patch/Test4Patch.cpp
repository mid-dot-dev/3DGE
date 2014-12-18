/*
################################################################################

Copyright © 2014-2015 Mid (Midhun Kumar Pallipurath)
Aspiring Game Programmer
Undergraduate Portfolio Project @ The Art Institue of Vancouver
Bachelor's of Science in Visual and Game Programming

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
#include "Test4Patch.h"



void Test4Patch::startup()
{
	mCamera.Setup(50.0f, ((float)info.windowWidth / (float)info.windowHeight),  1.0f, 100000.0f);
	mCamera.SetPosition(vmath::vec3(10.0f, -28.0f, 10.0f));
	mCamera.SetLookAt(vmath::vec3(10.0f, -28.0f, 11.0f));
	mCamera.InitMat();

    load_shaders();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

	int sizeOfPoly = sizeof(vmath::vec3) * ocean.getPolySize();

    glBufferData(GL_ARRAY_BUFFER, sizeOfPoly, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);// sets the buffer as 0th vertex attribute
    glEnableVertexAttribArray(0);//then enable it

	patchInit.InitPoly(50);
	ocean.InitWave(waves[0], 1, -2.5, 15.0f, 5, 30, vmath::vec3( 4.3f, 0.3f,-0.3f), false);
	ocean.InitWave(waves[1], 1,    3, 5.0f,  5, 30, vmath::vec3( 0.4f,-0.1f,-7.3f), false);
	ocean.InitWave(waves[2], 1,    2, 15.0f, 5, 30, vmath::vec3( 1.0f, 9.0f, 0.0f), false);

}

void Test4Patch::render(double currentTime)
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one = 1.0f;

	static GLfloat t = 0.0f;
	static unsigned int startTime = 0;
	static unsigned int lastTime = 0;
	if (startTime == 0)
	{
		startTime = currentTime;
		lastTime = currentTime;
	}
	// if (!input.mKeyStates['P'])
	t = (currentTime - startTime) * 0.001f;
	float dt = (currentTime - lastTime) * 0.001f;
	lastTime = currentTime;

    glViewport(0, 0, info.windowWidth, info.windowHeight);
    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    glEnable(GL_DEPTH_TEST);

	int sizeOfPolyD = sizeof(vmath::vec3) * ocean.getPolySize();
	ocean.polyD = (vmath::vec3 *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeOfPolyD, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(ocean.polyD, patchInit.polyD, sizeOfPolyD);
	ocean.Animate(currentTime,3,waves);
	glUnmapBuffer(GL_ARRAY_BUFFER);

    glUseProgram(program);
	//mCamera.UpdateMatrix();
	mCamera.SetCenter(ocean.polyAvg());
    vmath::mat4 proj_matrix = mCamera.GetProjectionMatrix();
	vmath::mat4 mv_matrix = mCamera.GetViewMatrix();

	HandleInput(dt);

    glUniformMatrix4fv(uniforms.patch.mv_matrix, 1, GL_FALSE, mv_matrix);
    glUniformMatrix4fv(uniforms.patch.proj_matrix, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(uniforms.patch.mvp, 1, GL_FALSE, proj_matrix * mv_matrix);

    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(GL_PATCHES, 0, ocean.getPolySize());

	glUseProgram(programCP);
    glUniformMatrix4fv(uniforms.control_point.mvp, 1, GL_FALSE, proj_matrix * mv_matrix);

    if (points)
    {
        glPointSize(9.0f);
        glUniform4fv(uniforms.control_point.draw_color, 1, vmath::vec4(0.2f, 0.7f, 0.9f, 1.0f));
        glDrawArrays(GL_POINTS, 0, ocean.getPolySize());
    }
}

void Test4Patch::load_shaders()
{
    if (program)
        glDeleteProgram(program);

    GLuint shaders[4];
//#ifdef  _DEBUG
	shaders[0] = sb6::shader::load("Bin/media/shaders/Test4Patch/Test4Patch.vs.glsl", GL_VERTEX_SHADER, true);
    shaders[1] = sb6::shader::load("Bin/media/shaders/Test4Patch/Test4Patch.tcs.glsl", GL_TESS_CONTROL_SHADER);
    shaders[2] = sb6::shader::load("Bin/media/shaders/Test4Patch/Test4Patch.tes.glsl", GL_TESS_EVALUATION_SHADER);
    shaders[3] = sb6::shader::load("Bin/media/shaders/Test4Patch/Test4Patch.fs.glsl", GL_FRAGMENT_SHADER);
//#else
//	  shaders[0] = sb6::shader::load("media/shaders/Test4Patch/Test4Patch.vs.glsl", GL_VERTEX_SHADER, true);
//    shaders[1] = sb6::shader::load("media/shaders/Test4Patch/Test4Patch.tcs.glsl", GL_TESS_CONTROL_SHADER);
//    shaders[2] = sb6::shader::load("media/shaders/Test4Patch/Test4Patch.tes.glsl", GL_TESS_EVALUATION_SHADER);
//    shaders[3] = sb6::shader::load("media/shaders/Test4Patch/Test4Patch.fs.glsl", GL_FRAGMENT_SHADER);
//#endif //  _DEBUG

    program = sb6::program::link_from_shaders(shaders, 4, true);

    uniforms.patch.mv_matrix = glGetUniformLocation(program, "mv_matrix");
    uniforms.patch.proj_matrix = glGetUniformLocation(program, "proj_matrix");
    uniforms.patch.mvp = glGetUniformLocation(program, "mvp");

	if (programCP)
        glDeleteProgram(programCP);

    shaders[0] = sb6::shader::load("Bin/media/shaders/Test4Patch/points.vs.glsl", GL_VERTEX_SHADER);
    shaders[1] = sb6::shader::load("Bin/media/shaders/Test4Patch/points.fs.glsl", GL_FRAGMENT_SHADER);

    programCP = sb6::program::link_from_shaders(shaders, 2, true);

    uniforms.control_point.draw_color = glGetUniformLocation(programCP, "draw_color");
    uniforms.control_point.mvp = glGetUniformLocation(programCP, "mvp");

}



DECLARE_MAIN(Test4Patch)
