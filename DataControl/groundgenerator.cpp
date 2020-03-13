#include "groundgenerator.h"

GeoNode::Ptr GroundGenerator::generate(GroundDescriptor::Ptr d, ReflexionModel::Ptr refl, Material::Ptr mat)
{
    Geometry::Ptr geometry_terrain = Geometry::create();
    *geometry_terrain = Geometry::cube();
    *geometry_terrain = geometry_terrain->subdivise(d->_subdiv);
    Geometry::intoCCW(*geometry_terrain);
    geometry_terrain->generateColors(Vec3(1.0));
    geometry_terrain->generateTexCoords( Geometry::TexGenMode_Cubic, d->_texCoordDensity);
    geometry_terrain->sphericalNormalization(d->_sphericalN);
    geometry_terrain->noiseBump(d->_noiseOctaves, d->_noisePersist, d->_noiseFreq, d->_noiseForce);
    geometry_terrain->generateNormals( Geometry::NormalGenMode_PerFace );
    geometry_terrain->interpolateNormals();

    GeoNode::Ptr terrainNode = GeoNode::create(geometry_terrain, false);
    terrainNode->setReflexion(refl);
    terrainNode->setMaterial(mat);

    RenderPass::Ptr env = RenderPass::create();
    env->enablePass(RenderPass::Pass_EnvironmentMapping);
    env->enablePass(RenderPass::Pass_ShadowMapping);
    terrainNode->getState()->setRenderPass(env);

    if(d->_transparent)
        terrainNode->getState()->setTransparent(true);

    terrainNode->setScale( d->_scale );
    terrainNode->setPosition( d->_pos );

    return terrainNode;
}
