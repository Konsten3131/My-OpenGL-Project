
uniform sampler2D u_MainTex;

varying highp vec2 v_UV;

void main()
{
	highp vec4 color = texture2D( u_MainTex, v_UV );

	gl_FragColor = color;
}
