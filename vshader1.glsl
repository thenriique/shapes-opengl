#version 400

layout ( location = 0) in vec4 vPosition;
layout ( location = 1) in vec4 vColors;

uniform vec4 vColor;

out vec4 v2fcolor;

void main ()
{
    gl_Position = vPosition ; //gl_position -- variavel built-in

    v2fcolor = vColor;
}
