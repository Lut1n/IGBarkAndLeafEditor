// vertex attributes ( @see glBindAttribLocation )
in vec3 a_vertex;   // location 0
in vec3 a_color;    // location 1
in vec3 a_normal;   // location 2
in vec2 a_texcoord; // location 3

in mat4 a_instTransform; // location 4,5,6,7

// uniforms
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat3 u_normal;

// fragment shader stage input
out vec2 v_texCoord;
out vec3 v_m;
out vec3 v_mv;
out vec3 v_n;
out vec3 v_vertex;
out vec3 v_color;
out mat4 v_model;
out mat4 v_view;

void main()
{
    mat4 _model = u_model * a_instTransform;
    vec4 mv_pos = u_view * _model * vec4(a_vertex,1.0);
    gl_Position = u_proj * mv_pos;
    v_n = normalize(a_normal);
    v_color = a_color;

    v_texCoord = a_texcoord;
    v_m = (_model * vec4(a_vertex,1.0)).xyz;
    v_mv = mv_pos.xyz;
    v_vertex = a_vertex;
    v_model = _model;
    v_view = u_view;
}
