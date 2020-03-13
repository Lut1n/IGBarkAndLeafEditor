uniform samplerCube u_input_cube;
uniform float u_reversed;
uniform mat4 u_view;
uniform vec3 u_lightPos;
in vec2 v_texCoord;
in vec3 v_m;

vec4 i_col(vec3 uvw){return texture(u_input_cube, uvw).xyzw;}

void lighting(out vec4 out_color,
              out vec4 out_emissive,
              out vec3 out_normal,
              out float out_reflectivity,
              out float out_shininess,
              out float out_depth)
{
    vec3 uvw = v_m;
    // uvw.y *= -1.0;
    uvw = u_reversed>0.5? -uvw : uvw;
    out_color = i_col(uvw);
    out_emissive = i_col(uvw);
    out_normal = vec3(0.0);
    out_reflectivity = 0.0;
    out_shininess = 0.0;
    out_depth = 1.0;
}
