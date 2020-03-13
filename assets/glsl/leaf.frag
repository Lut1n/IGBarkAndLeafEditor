uniform int iBranchs;
uniform int iDepth;
uniform float iTilt;
uniform float iLen;
uniform float iLenFct;
uniform float iThickness;

uniform float iTexFreq;
uniform float iTexPersist;
uniform int iTexOctave;

uniform float iShapeFq1;
uniform float iShapeFq2;
uniform float iShapeMin;
uniform float iShapeMax;


// ------- OLD STUFF ------------


float angle(vec2 a, vec2 b)
{
    float la = length(a);
    float lb = length(b);
    float lab = la*lb;

    float dx = dot(a,b) /lab;

    vec3 crossr = cross(vec3(a,0.0), vec3(b,0.0));
    float dy = sign(crossr.z) * length(crossr)/ lab;

    return atan(dy,dx);
}


vec2 arc(vec2 uv, vec2 pa, vec2 pb, vec2 c, float w)
{
    vec2 da = pa-c;
    vec2 db = pb-c;
    vec2 d = uv-c;

    float side = sign(da.x*db.y - da.y*db.x);

    float a = angle(d,da);
    float b = angle(db,da);

    float la = length(da);
    float lb = length(db);

    float li = length(d);

    float it = mix(la,lb,(a/b));
    float dd = li-it;

    float res = step(abs(dd)-w,0.0);
    res *= side>0.0? step(a,0.0) * step(b,a) : step(0.0,a) * step(a,b);


    float res2 = abs(dd)-w;
    float dang = 0.0;
    if(a<0.0) dang = -a;
    if(a>=b) dang = a-b;
    res2 = dang;

    return vec2(1.0-res,res2);
}



vec2 radvec(float ang)
{
    float PI2 = 3.141592*2.0;
    return vec2(cos(ang*PI2),sin(ang*PI2));
}

vec2 seg(in float ang, in vec2 uv, in float len, in float thickness )
{
    vec2 r = vec2(1.0,1.0);
    vec3 xy = vec3(radvec(ang),0.0);
    vec3 uvw = vec3(uv,0.0);

    float ans1 = dot(uvw,xy);
        float ans2 = cross(xy,uvw).z;


        if ( ans1>=0. && ans1< len ) r.x -= step(abs(ans2),thickness);

    r.y = 0.0;
    if(ans1<0.0) r.y = abs(ans1);
    else if(ans1>len) r.y = abs(ans1-len);

    float dx =  abs(ans2-thickness); // *step(thickness,ans2);
        r.y = sqrt(r.y*r.y + dx*dx);

        return r;
}

vec2 arcseg(in float ang, in vec2 uv, in float len, in float thickness )
{
    vec2 p1 = vec2(0.0);
    vec2 p2 = p1 + radvec(ang)*len;
    vec2 c0 = p1 - radvec(ang*0.5)*len*0.5;

    return arc(uv,p1,p2,c0,thickness);
}


vec2 arcseg2(in vec2 uv, in vec2 p1, in vec2 p2, in float thickness )
{
    float a = dot(p2-p1, vec2(0.0,1.0)) / length(p2-p1);
    vec2 c0 = p1 + radvec(a*2.0)*length(p2-p1)*0.5;

    return arc(uv,p1,p2,c0,thickness);
}

#define K 2 // branch count
#define S 6 // level count



vec2 line(vec2 uv, vec2 a, vec2 b, float th)
{
    vec2 r;

    // strict
    vec2 i = uv-a; float li = length(i);
    vec2 t = b-a; float lt = length(t);

    float x = dot(i,t)/lt;
    float y = length(cross(vec3(i,0.0),vec3(t,0.0)))/lt;

    r.x = step(-th,x) * step(x,lt+th) * step(abs(y),th);


    // dist
    float th_dist = 0.0;
    r.y = 0.0;
    if(x<-th_dist) r.y = abs(x);
    else if(x>lt) r.y = abs(x-lt);

    float dx = abs(y)-th_dist;
    dx = dx*step(0.0,dx);
        r.y = sqrt(r.y*r.y + dx*dx);

    return r;
}

vec4 my_tree2(vec2 uv, vec2 t0, float len, float a, float thickness)
{
    vec4 r = vec4(0.0,1.0,0.0,0.0);

    int pk = 1;
    int k = 1;

    float plen = 0.0;
    float pa = 0.0;

    float th = thickness;

    for(int cl = 0;cl<int(S);++cl)
    {
        for(int i=0;i<k;++i)
        {
            float p = float(i)/float(k-1);
            float pp = float(i/int(K))/float(pk-1);
            if(pk<=1)pp=0.5;
            if(k<=1)p=0.5;

            float paa = mix(-pa,pa,pp);
            float aa = mix(-a,a,p);

            vec2 t00 = t0 + vec2(sin(paa),cos(paa))*plen;
            vec2 t = t0 + vec2(sin(aa),cos(aa))*(plen+len);

            // t00 += hash2d(t00) * 0.02;
            // t += hash2d(t) * 0.02;

            r.x += line(uv,t00,t,th).x;
            r.y = min(r.y,line(uv,t00,t,th).y);
        }

        pk = k;
        k *= int(K);
        plen += len;
        len *= 0.9;
        pa = a;
        a *= 1.1;
        th *= 0.7;
    }

    return r;
}
// ------- OLD STUFF ------------

vec2 arc2(vec2 uv, vec2 pa, vec2 pb, vec2 c, float w)
{
    vec2 da = pa-c;
    vec2 db = pb-c;
    vec2 d = uv-c;

    float side = sign(da.x*db.y - da.y*db.x);

    float a = angle(d,da);
    float b = angle(db,da);

    float la = length(da);
    float lb = length(db);

    float li = length(d);

    float it = mix(la,lb,(a/b));
    float dd = li-it;

    float res = step(abs(dd)-w,0.0);
    res *= side>0.0? step(a,0.0) * step(b,a) : step(0.0,a) * step(a,b);


    float res2 = abs(dd)-w;
    float dang = 0.0;
    if(a<0.0) dang = -a;
    if(a>=b) dang = a-b;
    res2 = dang;

    return vec2(1.0-res,res2);
}

vec2 arcseg3(in vec2 uv, in vec2 p1, in vec2 p2, in float thickness )
{
    float a = dot(p2-p1, vec2(0.0,1.0)) / length(p2-p1);
    vec2 c0 = p1 + radvec(a*2.0)*length(p2-p1)*0.5;

    return arc2(uv,p1,p2,c0,thickness);
}


vec4 leaf_struct(vec2 uv, vec2 t0, float len, float a, float thickness)
{
    uv.x = abs(uv.x);

    vec4 r = vec4(0.0,1.0,0.0,0.0);

    int pk = 1;
    int k = 1;

    float plen = 0.0;
    float pa = 0.0;

    float th = thickness;
    vec2 line_r;

    line_r = line(uv,t0,t0+vec2(0.0,len*float(S)),th);
    r.x += line_r.x;
    r.y = min(r.y,line_r.y);

    vec2 t00 = vec2(0.0); // t0+vec2(0.0,len*2.0);
    float paa = a;

    for(int qq=0;qq<iBranchs;++qq)
    {
        float progr = float(qq)/float(iBranchs);
        float paa = a;

        t00 = t0+vec2(0.0,len*float(iBranchs)) * progr;

        for(int kk=0;kk<iDepth;++kk)
        {
            vec2 t = t00 + vec2(sin(paa),cos(paa))*(len);

            line_r = line(uv,t00,t,th);
            r.x += line_r.x;
            r.y = min(r.y,line_r.y);

            t00 = t;
            paa += a*0.8;
        }
    }

    return r;
}


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

float sinM(float r, float fq, float mn, float mx)
{
    float PI2 = 3.141592*2.0;
    return (-cos((r * PI2) * fq) * 0.5+0.5) * (mx-mn) + mn;
}


float leafFunc(vec2 uv)
{
    if(uv.y < 0.0) return 0.0;

    float fq_fct = mix(iShapeFq1,iShapeFq2,uv.y);

    float dleaf = sinM(uv.y, fq_fct, iShapeMin, iShapeMax) - abs(uv.x);
    return dleaf;
}

float leafFunc2(vec2 uv)
{
    if(uv.y < 0.0) return 0.0;

    vec2 teethdir = normalize(vec2(1.0,0.6));
    vec2 uv_teeth = uv; uv_teeth.x = -abs(uv_teeth.x);

    float fq_fct = mix(iShapeFq1,iShapeFq2,uv.y);
    float f2 = sin(dot(uv_teeth,teethdir)*200.0) * 0.01;

    float dleaf = sinM(uv.y, fq_fct, iShapeMin, iShapeMax)+f2 - abs(uv.x);
    return dleaf;
}

float queueFunc(vec2 uv)
{
    if(uv.y > 0.05) return -1.0;

    float dleaf = sinM(uv.y, 0.2, 0.0, 0.1) - abs(uv.x);
    return dleaf;
}

float sampleShape(float a,float len)
{
    float radius = 0.0;
    a += 3.141592*0.5; if(a>3.141592)a-=2.0*3.141592;
    float displ = (sin(abs(a)*1.0))*0.5+0.5;
    // displ = pow(displ,4.0);
    displ *= 0.2;
    // float displ = a*0.02;
    return step(len,radius+displ);
}

float shape_base(vec2 uv)
{
    vec2 center = vec2(0.0,0.3);

    vec2 dir = normalize(uv-center);
    float a = angle(dir,vec2(0.0,1.0));
    float len = distance(uv,center);
    float a_n = (a/3.141592);


    return sampleShape(a,len);
}



float leaf(vec2 uv, float len, float a, float th)
{
    float dshape = step((leafFunc2(uv)), 0.0);
    float dshape2 = step(-queueFunc(uv),0.01);

    float final_shape = 1.0-dshape + dshape2;

    float final_texture = clamp( fBn(vec3(uv,0.0), iTexOctave, iTexFreq, iTexPersist), 0.0, 1.0);
    final_texture = pow(final_texture,2.0)*2.5;
    // float final_texture = voronoi(vec3(uv,0.0), 64.0);

    // float final_struct = (1.0-my_tree(0.1, uv, 0.08, 0.02).x);
    float final_struct = leaf_struct(uv, vec2(0.0,0.0), len, a, 0.0).y;

    final_struct = clamp(mix(1.0,0.0,final_struct/th), 0.0,1.0);
    // final_struct = cos(final_struct * 3.14*0.5/th)*step(final_struct,th);
    // final_struct *= 0.7;

    // float filling = final_texture * (1.0-final_struct) + final_struct;
    // float filling = final_texture * (1.0-final_struct);
    float filling = max(final_texture,final_struct);
    // float filling = final_texture*(1.0- final_struct*0.7);
    filling *= final_shape;

    float g = clamp(filling,0.0,1.0);

    return g;
}

vec4 textureColor(vec2 uv)
{
    // vec2 uv = fragCoord.xy / iResolution.y;
    uv -= vec2(0.5, 0.2);
    uv *= 1.2;
    float g = leaf(uv, iLen, iTilt, iThickness);

    vec3 color1 = vec3(0.2,0.3,0.5); float ck_1 = 0.4;
    vec3 color2 = vec3(0.9,0.5,0.1); float ck_2 = 0.5;
    vec3 color3 = vec3(0.3,0.8,0.2); float ck_3 = 0.6;
    vec3 colormap = vec3(0.0);
    if(g>ck_1) colormap = color1;
    if(g>ck_2) colormap = color2;
    if(g>ck_3) colormap = color3;

    // float g = leaf(uv, 0.13, 0.5, 0.02);
    float EPSILON = 0.001;
    return vec4(vec3(g),step(EPSILON,g));
    // return vec4(g*colormap,step(EPSILON,g));
}

vec3 textureNormal(vec2 uv) { return vec3(0.0,0.0,1.0); }

float textureReflectivity(vec2 uv){ return 0.0; }

vec4 textureEmissive(vec2 uv) { return vec4(0.0); }

