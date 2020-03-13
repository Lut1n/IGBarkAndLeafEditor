#include "rockgenerator.h"


RockGenerator::RockGenerator()
{
    _sph = Geometry::create();
    *_sph = Geometry::sphere(8, 1.0);
    _sph->rotX(-3.141592*0.5);
    Geometry::intoCCW(*_sph);
    _sph->noiseBump(8, 0.7, 1.0, 0.5);
    _sph->generateColors(Vec3(1.0));
    _sph->generateNormals( Geometry::NormalGenMode_PerFace );
    _sph->generateTexCoords( Geometry::TexGenMode_Cubic, 1.0);
    _sph->interpolateNormals();
}


Geometry RockGenerator::buildRockGeo(int index,
                         float radius, float height, Vec3 position, Vec3 rotation,
                         const RockDescriptor& desc)
{
    Geometry g = *_sph;
    g.scale(Vec3(radius));

    if(index < desc._nodeCount-1)
    {
        for(int i=0;i<desc._divisionCount;++i)
        {
            Vec3 r( 0.0);
            const float pi2 = 3.141592 * 2.0;
            Vec3 p( radius*std::cos(pi2 * float(i) / desc._divisionCount), 0.0, radius*std::sin(pi2 * float(i) / desc._divisionCount));
            g += buildRockGeo(index+1, radius*desc._radiusFactor, /*height*desc._heightFactor*/0.0, p,r, desc);
        }
    }

    // g.rotX(rotation[0]); g.rotY(rotation[1]); g.rotZ(rotation[2]);
    g += position;

    return g;
}


GeoNode::Ptr RockGenerator::generateRock(const RockDescriptor& desc, Material::Ptr mat, ReflexionModel::Ptr reflexion, bool wireframe)
{
    Geometry::Ptr geo = Geometry::create();
    *geo = buildRockGeo(0,desc._baseRadius, /*desc._baseHeight*/0.0, Vec3(7.0,0.0,0.0), Vec3(0.0), desc);

    GeoNode::Ptr gn = GeoNode::create(geo, wireframe);
    gn->setMaterial(mat);
    gn->setReflexion(reflexion);

    RenderPass::Ptr env = RenderPass::create();
    env->enablePass(RenderPass::Pass_ShadowMapping);
    gn->getState()->setRenderPass(env);

    return gn;
}
