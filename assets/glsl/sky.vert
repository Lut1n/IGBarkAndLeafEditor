// vertex attributes ( @see glBindAttribLocation )
in vec3 a_vertex;   // location 0
in vec3 a_color;    // location 1
in vec3 a_normal;   // location 2
in vec2 a_texcoord; // location 3

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
    vec3 v = normalize(a_vertex) * 1024.0;
    mat4 a_view = u_view;
    // a_view[3].xyz = vec3(0.0,0.0,0.0);
    vec4 mv_pos = a_view * u_model * vec4(v,1.0);
    gl_Position = u_proj * mv_pos;
    gl_Position.z = gl_Position.w; // force depth == 1.0
    v_n = normalize(u_normal * a_normal);
    v_color = a_color;

    v_texCoord = a_texcoord;
    v_m = (u_model * vec4(v,1.0)).xyz;
    v_mv = mv_pos.xyz;
    v_vertex = v;
    v_model = u_model;
    v_view = u_view;
}
