#version 120

attribute vec2 invertex;
varying vec2 outvertex;
varying vec2 outtexcoord;

uniform mat4 mvp;

void main() {
     gl_Position.xy = invertex;
     gl_Position.zw = vec2(0.0f, 1.0);
     gl_Position = mvp * gl_Position;
     //automatically interpolates
     outvertex = invertex;
     outtexcoord = invertex;
}
