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
uniform float u_time;

// fragment shader stage input
out vec2 v_texCoord;
out vec3 v_m;
out vec3 v_mv;
out vec3 v_n;
out vec3 v_vertex;
out vec3 v_color;
out mat4 v_view;
out mat4 v_model;

void main()
{
    vec4 vert = vec4(a_vertex,1.0);
    vec4 mvert = u_model * vert;
    float oft = mvert.x+mvert.z;
    vert += vec4(0.0,1.0,0.0,0.0) * (sin(oft*10.0+u_time*2.0)*0.5+0.5) * 0.5;
    vec4 mv_pos = u_view * u_model * vert;
    gl_Position = u_proj * mv_pos;
    v_n = normalize(u_normal * a_normal);
    v_color = a_color;

    v_texCoord = a_texcoord;
    v_m = (u_model * vert).xyz;
    v_mv = mv_pos.xyz;
    v_vertex = vert.xyz;
    v_model = u_model;
    v_view = u_view;
}
