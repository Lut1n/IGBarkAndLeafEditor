float PI2 = 3.141592*2.0;

uniform int iOctaves;
uniform float iFreq;
uniform float iPersist;
uniform float iScaleX;
uniform float iScaleY;

float hash(vec3 uvw, float seed)
{
    uvw = mod(uvw, 1.0);
    vec3 other = vec3(123.757, 87.4568, 548.21453);
    float L = 548.12348969;

    return fract(sin(seed + dot(uvw,other)) * L);
}


float voronoi(vec3 uvw, float freq)
{
    vec3 iuv = floor(uvw * freq);
    vec3 fuv = fract(uvw * freq);

    vec3 pos[8];

    float dis[8];

    for(int i=0;i<8;++i)
    {
        vec3 base = vec3(i%2, (i/2)%2, (i/4)%2);
        pos[i] = iuv+base;
        pos[i] /= freq;
        pos[i] = base + (vec3(hash(pos[i],0.0), hash(pos[i],1.234),hash(pos[i],8.794)) - 0.5) * 0.7;

        dis[i]= distance(pos[i],fuv);
    }

    float min1 = -1.0;
    float min2 = -1.0;

    for(int i=0; i<8; i++)
    {
        if(min1 == -1.0 || dis[i] < min1)
        {
            min2 = min1;
            min1 = dis[i];
        }
        else if(dis[i] != min1 && (min2 == -1.0 || dis[i] < min2) )
        {
                min2 = dis[i];
        }
    }


    float result = min1 / (min1+min2);

    return result;
}


float fBn(vec3 uvw, int octave, float fq, float persist)
{
        float intensity = 1.0;
    float total = 0.0;
    float freq = fq; // 16.0;

    float r = 0.0;

    int MAX_OCTAVE = 10;
    for(int i=0; i<MAX_OCTAVE; i++)
    {
                r += voronoi(uvw, freq) * intensity;
        total += intensity;

        freq *= 2.0;
        intensity *= persist;

        if(i>=octave)break;
    }

    return r/total;
}


vec4 textureColor(vec2 uv)
{
    uv *= vec2(iScaleX,iScaleY);


    vec3 uvw = vec3(uv,0.0);

    float v = 0.0;
    // v = hash(uvw,0.0);
    // v = voronoi(uvw,10.0);
    v = fBn(uvw,iOctaves,iFreq,iPersist);

    v = v + pow(v,2.0);

    return vec4(vec3(v),1.0);
}

vec3 textureNormal(vec2 uv) { return vec3(0.0,0.0,1.0); }

float textureReflectivity(vec2 uv){ return 0.0; }

vec4 textureEmissive(vec2 uv) { return vec4(0.0); }
