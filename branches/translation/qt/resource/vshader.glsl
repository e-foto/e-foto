attribute mediump vec4 a_vertex;
attribute mediump vec2 a_texture;
uniform mediump mat4 u_projection;

varying mediump vec2 v_texture;

void main(void)
{
    v_texture = a_texture;
    gl_Position = u_projection * a_vertex;
}
