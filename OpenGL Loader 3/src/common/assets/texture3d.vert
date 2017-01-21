
uniform mat4 u_MatMVP;

attribute vec3 a_Position;
attribute vec2 a_UV;

varying vec2 v_UV;

void main()
{
	gl_Position = u_MatMVP * vec4( a_Position, 1.0 );
	
    v_UV = a_UV;
}
