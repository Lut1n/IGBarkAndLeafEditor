#include <Renderer/CpuRenderer.h>

#include <Renderer/CpuBlinnPhong.h>

#include <cstdlib>

IMPGEARS_BEGIN

//--------------------------------------------------------------
CpuRenderer::CpuRenderer()
    : SceneRenderer()
{
    imp::Vec4 vp(0.0,0.0,512.0,512.0);
    Image::Ptr defaultTarget = Image::create((int)vp[2] , (int)vp[3], 4);
    _geoRenderer.setViewport( vp );
    _geoRenderer.setTarget(0,defaultTarget, Vec4(0.0));
}

//--------------------------------------------------------------
CpuRenderer::~CpuRenderer()
{
}

//---------------------------------------------------------------
void CpuRenderer::render(Graph::Ptr& scene)
{
}


//--------------------------------------------------------------
int prepareReflexion(GeometryRenderer& _geoRenderer, ReflexionModel::Ptr& program)
{
    ReflexionModel::AbstractFrag::Ptr frag = ReflexionModel::AbstractFrag::create();

  //  GeometryRenderer::VertCallback::Ptr vertCb = DefaultVertCb::create();
  //  FragCallback::Ptr fragCb = frag;
  //
  //  ReflexionModel::Lighting li = program->getLighting();
  //  if(li == ReflexionModel::Lighting_None) frag->_lighting = NoLighting::create();
  //  else if(li == ReflexionModel::Lighting_Phong) frag->_lighting = PhongLighting::create();
  //  else frag->_lighting = program->_lightingCb;
  //
  //  ReflexionModel::Texturing te = program->getTexturing();
  //  if(te == ReflexionModel::Texturing_PlainColor) frag->_texturing = PlainColorCb::create();
  //  else if(te == ReflexionModel::Texturing_Samplers_CN) frag->_texturing = SamplerCbCN::create();
  //  else if(te == ReflexionModel::Texturing_Samplers_CNE) frag->_texturing = SamplerCbCNE::create();
  //  else frag->_texturing = program->_texturingCb;
  //
  //  ReflexionModel::MRT mrt = program->getMRT();
  //  if(mrt == ReflexionModel::MRT_1_Col) frag->_mrt = Mrt1ColorCb::create();
  //  else if(mrt == ReflexionModel::MRT_2_Col_Emi) frag->_mrt = Mrt2ColorEmiCb::create();
  //  else frag->_mrt = program->_mrtCb;
  //
  //  if(vertCb == nullptr) _geoRenderer.setDefaultVertCallback();
  //  else _geoRenderer.setVertCallback(vertCb);
  //  if(fragCb == nullptr) _geoRenderer.setDefaultFragCallback();
  //  else _geoRenderer.setFragCallback(fragCb);
    return 0;
}

void updateUniform(GeometryRenderer& geoRenderer, Uniform::Ptr& uniform)
{
    std::string name = uniform->getID();
    Uniform::Type type = uniform->getType();

    if(type == Uniform::Type_Mat4)
    {
        if(name == "u_proj") geoRenderer.setProj( uniform->getMat4() );
        else if(name == "u_view") geoRenderer.setView( uniform->getMat4() );
        else if(name == "u_model") geoRenderer.setModel( uniform->getMat4() );
        // todo
    }
    else
    {
        geoRenderer.setUniform( uniform );
    }
}

//---------------------------------------------------------------
void CpuRenderer::applyState(const State::Ptr& state)
{
    if(state->getFaceCullingMode() == State::FaceCullingMode_None)
        _geoRenderer.setCullMode(GeometryRenderer::Cull_None);
    else if(state->getFaceCullingMode() == State::FaceCullingMode_Back)
        _geoRenderer.setCullMode(GeometryRenderer::Cull_Back);
    else if(state->getFaceCullingMode() == State::FaceCullingMode_Front)
        _geoRenderer.setCullMode(GeometryRenderer::Cull_Front);

    _geoRenderer.setViewport(state->getViewport());

    if(!_direct)
    {
        for(int i=0;i<(int)_targets->count();++i)
            _geoRenderer.setTarget(i,_targets->get(i)->getSource(),Vec4(0.0));
    }

    ReflexionModel::Ptr reflexion = state->getReflexion();
    prepareReflexion(_geoRenderer,reflexion);

    const std::map<std::string,Uniform::Ptr>& uniforms = state->getUniforms();
    for(auto u : uniforms) updateUniform(_geoRenderer, u.second);
}

//---------------------------------------------------------------
void CpuRenderer::applyClear(ClearNode* clearNode)
{
    // int count = _geoRenderer.getTargetCount();
    // for(int i=0;i<count;++i)
    //     _geoRenderer.setClearColor( i, clearNode->getColor()*255.0 );

    _geoRenderer.init();
    _geoRenderer.clearTargets(); // todo : remove color clearing
    _targets->clearTargets();
}

//---------------------------------------------------------------
void CpuRenderer::drawGeometry(GeoNode* geoNode)
{
    _geoRenderer.render( *(geoNode->_geo) );
}

IMPGEARS_END
