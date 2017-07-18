#version 120

uniform vec3 color;

#ifdef GAWO_RTT
#define FRAGOUT gl_FragData[0]
#else
#define FRAGOUT gl_FragColor
#endif

void main()
{
  FRAGOUT = vec4(color, 0.0f);
}

