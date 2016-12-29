
attribute vec2 a_Position;
attribute vec2 a_UV;

varying vec2 v_UV;

void main()
{
    gl_Position = vec4( a_Position, 1, 1 );

	v_UV = a_UV;
}
