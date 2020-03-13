#include "treegenerator.h"


TreeGenerator::TreeGenerator()
{
    _cyl = Geometry::create();
    *_cyl = Geometry::cone(16, 1.0, 1.0, 0.7);
    _cyl->rotX(-3.141592*0.5);
    Geometry::intoCCW(*_cyl);
    _cyl->generateColors(Vec3(1.0));
    _cyl->generateNormals( Geometry::NormalGenMode_PerFace );
    _cyl->generateTexCoords( Geometry::TexGenMode_Cubic, 1.0);
    _cyl->interpolateNormals();

    _leaf = Geometry::create();
    *_leaf = Geometry::cube();
    _leaf->generateColors(Vec3(1.0));
    _leaf->generateNormals( Geometry::NormalGenMode_PerFace );
    _leaf->generateTexCoords( Geometry::TexGenMode_Cubic, 1.0 );
    _leaf->rotX(3.141592); // inverse texture
    _leaf->scale( Vec3(1.0,1.0,0.0) );
    (*_leaf)+=Vec3(0.0,1.0,0.0);

    _leafBase = Geometry::create();
}



void TreeGenerator::buildInstancedBranch(int index, float radius, float height, Vec3 position, Vec3 rotation, const TreeDescriptor& desc, std::vector<Matrix4>& transforms, const Matrix4& parent)
{
    static const float pi2 = 3.141592 * 2.0;

    Matrix4 curr = Matrix4::rotationXYZ(rotation) * Matrix4::translation(position) * parent;
    Matrix4 trans = Matrix4::scale(radius, height, radius) * curr;
    transforms.push_back(trans);

    if(index < desc._nodeCount-1)
    {

        for(int i=0;i<desc._divisionCount;++i)
        {
            float progr = float(i) / desc._divisionCount;
            Vec3 p( 0.0, height * (1.0-progr*desc._staggering), 0.0 );
            Vec3 r1( desc._theta, pi2 * progr, 0.0 );
            buildInstancedBranch(index+1, radius*desc._radiusFactor, height*desc._heightFactor, p, r1, desc, transforms,curr);
        }
    }
}

Node::Ptr TreeGenerator::buildInstancedTree(const TreeDescriptor& desc, Material::Ptr mat, ReflexionModel::Ptr refl)
{
    InstancedGeometry::Ptr instanced = InstancedGeometry::create();
    instanced->clone(_cyl);

    std::vector<Matrix4> transforms;
    buildInstancedBranch(0,desc._baseRadius, desc._baseHeight, Vec3(0.0), Vec3(0.0), desc, transforms, Matrix4());
    instanced->setTransforms(transforms);
    instanced->setCount(transforms.size());

    GeoNode::Ptr node = GeoNode::create(instanced,false);
    node->setMaterial(mat);
    node->getState()->setReflexion(refl);
    RenderPass::Ptr env = RenderPass::create();
    env->enablePass(RenderPass::Pass_EnvironmentMapping);
    env->enablePass(RenderPass::Pass_ShadowMapping);

    node->getState()->setRenderPass(env);
    return node;
}

void TreeGenerator::buildInstancedLeaf_sub(int index, float radius, float height, Vec3 position, Vec3 rotation, const TreeDescriptor& desc, std::vector<Matrix4>& transforms, const Matrix4& parent)
{
    Matrix4 curr = Matrix4::rotationXYZ(rotation) * Matrix4::translation(position) * parent;

    if(index < desc._nodeCount-1)
    {

        for(int i=0;i<desc._divisionCount;++i)
        {
            const float pi2 = 3.141592 * 2.0;
            float progr = float(i) / desc._divisionCount;
            Vec3 p( 0.0, height * (1.0-progr*desc._staggering), 0.0 );
            Vec3 r1( desc._theta, pi2 * progr, 0.0 );
            buildInstancedLeaf_sub(index+1, radius*desc._radiusFactor, height*desc._heightFactor, p,r1, desc, transforms, curr);
        }

        // leaf
        for(int i=0;i<desc._leafCount;++i)
        {
            const float pi2 = 3.141592 * 2.0;
            float progr = float(i) / desc._leafCount;
            float ang_pos = pi2 * progr;
            float h = height * (1.0-progr*desc._staggering);

            Matrix4 tilt = Matrix4::rotationX(desc._theta*2.0);
            Matrix4 pos = Matrix4::translation(0.0, h, radius);
            Matrix4 rot = Matrix4::rotationY(ang_pos);
            Matrix4 leafMat = tilt * pos * rot;

            Matrix4 trans = leafMat * curr;
            transforms.push_back(trans);
        }
    }
}

Node::Ptr TreeGenerator::buildInstancedLeaf(const TreeDescriptor& desc, Material::Ptr mat, ReflexionModel::Ptr refl)
{
    InstancedGeometry::Ptr instanced = InstancedGeometry::create();
    instanced->clone(_leaf);
    instanced->scale(desc._leafSize);

    std::vector<Matrix4> transforms;
    buildInstancedLeaf_sub(0,desc._baseRadius, desc._baseHeight, Vec3(0.0), Vec3(0.0), desc, transforms, Matrix4());
    instanced->setTransforms(transforms);
    instanced->setCount(transforms.size());

    GeoNode::Ptr node = GeoNode::create(instanced,false);
    node->setMaterial(mat);
    node->getState()->setReflexion(refl);

    RenderPass::Ptr env = RenderPass::create();
    env->enablePass(RenderPass::Pass_EnvironmentMapping);
    env->enablePass(RenderPass::Pass_ShadowMapping);

    node->getState()->setRenderPass(env);
    return node;
}
