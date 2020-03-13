#version 300 es
precision highp float;

float PI2 = 3.141592*2.0;

uniform float iTime;
uniform int iBranchs;
uniform int iDepth;
uniform float iLength;
uniform float iThickness;
uniform float iLengthFct;
uniform float iThicknessFct;
uniform float iTheta;
uniform int iLeafCount;
uniform float iLeafSize;

in vec2 coords;
out vec4 oFrag_Color;

float line(vec2 uv, vec2 a, vec2 b, float th)
{
    vec2 i = uv-a; float li = length(i);
    vec2 t = b-a; float lt = length(t);

    float x = dot(i,t)/lt;
    float y = length(cross(vec3(i,0.0),vec3(t,0.0)))/lt;

    return step(-th,x) * step(x,lt+th) * step(abs(y),th);
}

vec2 circle(vec2 uv, vec2 c, float r)
{
    vec2 d = uv-c;
    float ld = length(d);

    return vec2( step(ld,r),ld-r );
}

vec2 circleOC(vec2 uv, vec2 center, int count, float radius)
{
    vec2 res;

    for(int i=0;i<=count;++i)
    {
        float a = float(i) / float(count);
        a *= 3.141592*2.0;

        vec2 sc = center + vec2(cos(a),sin(a)) * (radius*1.3);
        res += circle(uv, sc, radius);
    }

    return res;
}

vec2 my_tree(vec2 uv, vec2 t0, float len, float a, float thickness)
{
    vec2 r = vec2(0.0,0.0);

    int pk = 1;
    int k = 1;

    float plen = 0.0;
    float pa = 0.0;

    float th = thickness;

    for(int cl = 0;cl<int(iDepth);++cl)
    {
        for(int i=0;i<k;++i)
        {
            float p = float(i)/float(k-1);
            float pp = float(i/int(iBranchs))/float(pk-1);
            if(pk<=1)pp=0.5;
            if(k<=1)p=0.5;

            float paa = mix(-pa,pa,pp);
            float aa = mix(-a,a,p);

            vec2 t00 = t0 + vec2(sin(paa),cos(paa))*plen;
            vec2 t = t0 + vec2(sin(aa),cos(aa))*(plen+len);

            r.x += line(uv,t00,t,th);
            vec2 leaf = circleOC(uv,t,iLeafCount,iLeafSize);
            r.y += leaf.x;
        }

        pk = k;
        k *= int(iBranchs);
        plen += len;
        len *= iLengthFct; // 0.9;
        pa = a;
        a *= 1.1;
        th *= iThicknessFct; // 0.7;
    }

    return r;
}

void main()
{
    vec2 uv = coords;
    vec2 origin = vec2(0.0);

    uv -= origin;

    vec2 tree = my_tree(uv, vec2(0.0,0.0), iLength, iTheta + sin(iTime)*0.02, iThickness);

    vec3 woodColor = vec3(0.3,0.3,0.2);
    vec3 leafColor = vec3(0.0,1.0,0.0);

    vec3 color = vec3(0.0);
    color = mix(color,woodColor,clamp(tree.x,0.0,1.0));
    color = mix(color,leafColor,clamp(tree.y,0.0,1.0));

    oFrag_Color = vec4(color,1.0);
}
