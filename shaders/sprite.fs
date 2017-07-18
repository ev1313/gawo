#version 120

#ifdef GAWO_RTT
#define FRAGOUT gl_FragData[0]
#else
#define FRAGOUT gl_FragColor
#endif

varying vec2 outvertex;
varying vec2 outtexcoord;

uniform sampler2D tex;

void main()
{
  FRAGOUT = texture2D(tex, vec2(outtexcoord.x, 1-outtexcoord.y));

  if(FRAGOUT.a < 0.1f)
    discard;
}

