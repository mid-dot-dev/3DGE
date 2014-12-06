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

#include "3DGE.h"
#include "Test1Patch.h"



void Test1Patch::startup()
{
    load_shaders();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);// sets the buffer as 0th vertex attribute
    glEnableVertexAttribArray(0);//then enable it

}

void Test1Patch::render(double currentTime)
{
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one = 1.0f;

    static const float patch_initializer[] =
    {
        -1.0f,  -1.0f,  0.0f,
        -0.33f, -1.0f,  0.0f,
         0.33f, -1.0f,  0.0f,
         1.0f,  -1.0f,  0.0f,

        -1.0f,  -0.33f, 0.0f,
        -0.33f, -0.33f, 0.0f,
         0.33f, -0.33f, 0.0f,
         1.0f,  -0.33f, 0.0f,

        -1.0f,   0.33f, 0.0f,
        -0.33f,  0.33f, 0.0f,
         0.33f,  0.33f, 0.0f,
         1.0f,   0.33f, 0.0f,

        -1.0f,   1.0f,  0.0f,
        -0.33f,  1.0f,  0.0f,
         0.33f,  1.0f,  0.0f,
         1.0f,   1.0f,  0.0f,
    };
	//sending data toot he buffer instead of poking memory each time
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(patch_initializer), patch_initializer);

    glViewport(0, 0, info.windowWidth, info.windowHeight);
    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(program);

    vmath::mat4 proj_matrix = vmath::perspective(50.0f, (float)info.windowWidth / (float)info.windowHeight,  1.0f, 1000.0f);
    vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f);
    
    glUniformMatrix4fv(uniforms.patch.mv_matrix, 1, GL_FALSE, mv_matrix);
    glUniformMatrix4fv(uniforms.patch.proj_matrix, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(uniforms.patch.mvp, 1, GL_FALSE, proj_matrix * mv_matrix);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    

    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(GL_PATCHES, 0, 16);
}

void Test1Patch::load_shaders()
{
    if (program)
        glDeleteProgram(program);

    GLuint shaders[4];
#ifdef  _DEBUG
	shaders[0] = sb6::shader::load("Bin/media/shaders/Test1Patch/Test1Patch.vs.glsl", GL_VERTEX_SHADER, true);
    shaders[1] = sb6::shader::load("Bin/media/shaders/Test1Patch/Test1Patch.tcs.glsl", GL_TESS_CONTROL_SHADER);
    shaders[2] = sb6::shader::load("Bin/media/shaders/Test1Patch/Test1Patch.tes.glsl", GL_TESS_EVALUATION_SHADER);
    shaders[3] = sb6::shader::load("Bin/media/shaders/Test1Patch/Test1Patch.fs.glsl", GL_FRAGMENT_SHADER);
#else
	shaders[0] = sb6::shader::load("media/shaders/Test1Patch/Test1Patch.vs.glsl", GL_VERTEX_SHADER, true);
    shaders[1] = sb6::shader::load("media/shaders/Test1Patch/Test1Patch.tcs.glsl", GL_TESS_CONTROL_SHADER);
    shaders[2] = sb6::shader::load("media/shaders/Test1Patch/Test1Patch.tes.glsl", GL_TESS_EVALUATION_SHADER);
    shaders[3] = sb6::shader::load("media/shaders/Test1Patch/Test1Patch.fs.glsl", GL_FRAGMENT_SHADER);
#endif //  _DEBUG

    

    program = sb6::program::link_from_shaders(shaders, 4, true);

    uniforms.patch.mv_matrix = glGetUniformLocation(program, "mv_matrix");
    uniforms.patch.proj_matrix = glGetUniformLocation(program, "proj_matrix");
    uniforms.patch.mvp = glGetUniformLocation(program, "mvp");

}


DECLARE_MAIN(Test1Patch)
