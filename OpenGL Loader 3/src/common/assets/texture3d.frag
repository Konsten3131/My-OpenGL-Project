//http://kitsunegames.com/post/archived/2014/12/12/cross-platform-glsl-shaders-opengl-gl-eswebgl-tutorial/

#ifndef highp
#define highp
#endif
#ifndef mediump
#define mediump
#endif
#ifndef lowp
#define lowp
#endif

uniform sampler2D u_MainTex;
varying highp vec2 v_UV;
highp vec2 temp;


void main()
{
    
    temp.x = v_UV.x;
    
    temp.y =1.0 - v_UV.y;
	highp vec4 color = texture2D( u_MainTex, temp );

	gl_FragColor = color;
}
