#version 120

varying vec2 outvertex;
varying vec2 outtexcoord;

uniform sampler2D tex;

void main()
{
  gl_FragColor = texture2D(tex, vec2(outtexcoord.x, 1-outtexcoord.y));

  if(gl_FragColor.a < 0.1f)
    discard;
}

