
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
