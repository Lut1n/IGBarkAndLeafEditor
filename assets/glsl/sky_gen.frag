uniform mat4 u_view;
uniform vec3 u_lightPos;
in vec2 v_texCoord;
in vec3 v_m;


float hash(vec3 uvw, float seed)
{
    uvw = mod(uvw, 1.0);
    vec3 other = vec3(123.757, 87.4568, 548.21453);
    float L = 548.12348969;

    return fract(sin(seed + dot(uvw,other)) * L);
}


float perlin(vec3 uvw, float freq)
{
    vec3 i = floor(uvw * freq);
    vec3 f = fract(uvw * freq);

    f = 3.0*f*f - 2.0*f*f*f;

    float v0 = hash( (i+vec3(0.0,0.0,0.0))/freq,0.0 );
    float v1 = hash( (i+vec3(1.0,0.0,0.0))/freq,0.0 );
    float v2 = hash( (i+vec3(0.0,1.0,0.0))/freq,0.0 );
    float v3 = hash( (i+vec3(1.0,1.0,0.0))/freq,0.0 );
    float v4 = hash( (i+vec3(0.0,0.0,1.0))/freq,0.0 );
    float v5 = hash( (i+vec3(1.0,0.0,1.0))/freq,0.0 );
    float v6 = hash( (i+vec3(0.0,1.0,1.0))/freq,0.0 );
    float v7 = hash( (i+vec3(1.0,1.0,1.0))/freq,0.0 );

    float v01 = mix(v0,v1,f.x);
    float v23 = mix(v2,v3,f.x);
    float v45 = mix(v4,v5,f.x);
    float v67 = mix(v6,v7,f.x);

    float v0123 = mix(v01,v23,f.y);
    float v4567 = mix(v45,v67,f.y);

    return mix(v0123,v4567, f.z);
}

float fBnPerlin(vec3 uvw, int octave, float persist, float fq)
{
    float intensity = 1.0;
    float total = 0.0;
    float freq = fq;

    float r = 0.0;

    for(int i=0; i<octave; i++)
    {
        r += perlin(uvw, freq) * intensity;
        total += intensity;

        freq *= 2.0;
        intensity *= persist;
    }

    return r/total;
}




vec4 i_col(vec3 uvw)
{
    vec3 star = vec3(1.0);
    vec3 cloud = vec3(0.3,0.3,1.0) * fBnPerlin(uvw*vec3(1.0,4.0,1.0),4,0.7,2.0)*1.3;
    cloud += star * step(0.85,fBnPerlin(uvw,4,0.9,32.0));
    vec3 dust = vec3(0.2,0.25,0.1);
    vec3 mountains = vec3(.1,0.1,0.1) * fBnPerlin(uvw.yzx,8,0.7,4.0)*1.5;

    float a = atan(uvw.x,uvw.z) / 3.141592; a = a*0.5+0.5;
    float elevation = fBnPerlin(vec3(a,0.0,0.0),8,0.7,2.0);


    vec3 color = step(uvw.y,elevation) * mountains + step(elevation,uvw.y) * cloud;
    color += mix(dust,vec3(0.0),uvw.y * 10.0) * 0.1;

    return vec4(color,1.0);
}

void lighting(out vec4 out_color,
              out vec4 out_emissive,
              out vec3 out_normal,
              out float out_reflectivity,
              out float out_shininess,
              out float out_depth)
{
    vec3 uvw = normalize(v_m);
    out_color = i_col(uvw);
    out_emissive = i_col(uvw);
    out_normal = vec3(0.0);
    out_reflectivity = 0.0;
    out_shininess = 0.0;
    out_depth = 1.0;
}
