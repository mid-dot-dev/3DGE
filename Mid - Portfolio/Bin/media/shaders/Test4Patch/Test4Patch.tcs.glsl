#version 410 core

struct distance
{
    int dist[5];
};

uniform distance myDistances[100];

layout (vertices = 16) out;

void main(void)
{
    uint id = gl_InvocationID;

    //if (id == 0)
    //{
    //    gl_TessLevelInner[0] = 16.0;
    //    gl_TessLevelInner[1] = 16.0;
    //    gl_TessLevelOuter[0] = 16.0;
    //    gl_TessLevelOuter[1] = 16.0;
    //    gl_TessLevelOuter[2] = 16.0;
    //    gl_TessLevelOuter[3] = 16.0;
    //}

	// if (id == 0)
    //{
    //    gl_TessLevelInner[0] = myDistances;
    //    gl_TessLevelInner[1] = myDistances;
    //    gl_TessLevelOuter[0] = myDistances;
    //    gl_TessLevelOuter[1] = myDistances;
    //    gl_TessLevelOuter[2] = myDistances;
    //    gl_TessLevelOuter[3] = myDistances;
    //}

	//if (id == 0)
    //{
    //    gl_TessLevelInner[0] = gl_PrimitiveID;
    //    gl_TessLevelInner[1] = gl_PrimitiveID;
    //    gl_TessLevelOuter[0] = gl_PrimitiveID;
    //    gl_TessLevelOuter[1] = gl_PrimitiveID;
    //    gl_TessLevelOuter[2] = gl_PrimitiveID;
    //    gl_TessLevelOuter[3] = gl_PrimitiveID;
    //}

	if (id == 0)
    {
        gl_TessLevelInner[0] = myDistances[gl_PrimitiveID].dist[4];
        gl_TessLevelInner[1] = myDistances[gl_PrimitiveID].dist[4];
        gl_TessLevelOuter[0] = myDistances[gl_PrimitiveID].dist[0];
        gl_TessLevelOuter[1] = myDistances[gl_PrimitiveID].dist[1];
        gl_TessLevelOuter[2] = myDistances[gl_PrimitiveID].dist[2];
        gl_TessLevelOuter[3] = myDistances[gl_PrimitiveID].dist[3];
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[id].gl_Position;
}
