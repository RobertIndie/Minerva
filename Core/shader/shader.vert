
#version 400 core

layout( location = 0 ) in vec4 vPosition;

layout(location = 1) in vec4 offset;

void
main()
{
	vec4 t = vec4(0,0,0,0.5);
    gl_Position = vPosition / 2 + t;
}
