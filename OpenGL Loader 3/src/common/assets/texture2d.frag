//http://kitsunegames.com/post/archived/2014/12/12/cross-platform-glsl-shaders-opengl-gl-eswebgl-tutorial/
#ifdef GL_ES

#else
#version 110
#define highp
#define mediump
#define lowp
#endif

uniform sampler2D u_MainTex;
varying highp vec2 v_UV;

void main()
{

	highp vec4 color = texture2D( u_MainTex, v_UV );

	gl_FragColor = color;
}
