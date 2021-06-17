uniform sampler2D u_texture;
varying mediump vec2 v_texture;

uniform mediump vec3 u_filter;

void main(void)
{
    mediump vec4 texel = texture2D(u_texture, v_texture);
    gl_FragColor = vec4(texel.rgb * u_filter, texel.a);
}
