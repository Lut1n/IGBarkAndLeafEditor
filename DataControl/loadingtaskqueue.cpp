#include "loadingtaskqueue.h"

#include <QtConcurrent/QtConcurrent>

#include "GuiContext/loadinginfo.h"

#include "DataControl/rockgenerator.h"
#include "DataControl/groundgenerator.h"

LoadingTaskQueue* LoadingTaskQueue::s_instance = nullptr;

LoadingTaskQueue* LoadingTaskQueue::instance()
{
    return s_instance;
}

LoadingTaskQueue::LoadingTaskQueue()
{
    s_instance = this;

    loaderThread = new LoaderThread();
    connect(loaderThread,&LoaderThread::finished,this,&LoadingTaskQueue::handleFinished, Qt::QueuedConnection);
    connect(this,&LoadingTaskQueue::requestLoading, loaderThread,&LoaderThread::setCurrentTask, Qt::QueuedConnection);
}

LoadingTaskQueue::~LoadingTaskQueue()
{
    loaderThread->wait();
    disconnect(this,&LoadingTaskQueue::requestLoading, loaderThread,&LoaderThread::setCurrentTask);
    disconnect(loaderThread,&LoaderThread::finished,this,&LoadingTaskQueue::handleFinished);
    delete loaderThread;
}

void LoadingTaskQueue::request(imp::Material::Ptr material, MatCompTy dst, const std::string& url)
{
    qDebug() << "[LoadingTaskQueue] request loading (" << url.c_str() << ")";
    LoadingInfo::s_instance->setMessage("texture requested");
    LoadingTaskTextureInfo::Ptr info = LoadingTaskTextureInfo::create();
    info->_dstMat = material;
    info->_dstMatSmpl = dst;
    info->_url = url;

    m_tasks.push_back(info);
    LoadingInfo::s_instance->setTaskCount(1);
    wakeup();
}

void LoadingTaskQueue::request(CubeMapHandler* skybox, const std::vector<std::string> urls)
{
    qDebug() << "[LoadingTaskQueue] request loading cubemap";
    LoadingInfo::s_instance->setMessage("cubemap requested");
    LoadingTaskCubeMapInfo::Ptr info = LoadingTaskCubeMapInfo::create();
    info->_url = urls;
    info->_dstHandler = skybox;

    m_tasks.push_back(info);
    LoadingInfo::s_instance->setTaskCount(1);
    wakeup();
}

void LoadingTaskQueue::request(imp::Graph::Ptr graph, imp::Node::Ptr dstNode, imp::ReflexionModel::Ptr reflexion, imp::Material::Ptr mat, GeometryDescriptors::Ptr desc)
{
    qDebug() << "[LoadingTaskQueue] request loading geometry";
    LoadingInfo::s_instance->setMessage("geometry requested");
    LoadingTaskGeometryInfo::Ptr info = LoadingTaskGeometryInfo::create();
    info->_dstNode = dstNode;
    info->_graph = graph;
    info->_reflexion = reflexion;
    info->_mat = mat;
    info->_desc = desc;
    if(desc->_ty == GeometryDescriptors::Ty_Tree) info->_unique = true;
    if(desc->_ty == GeometryDescriptors::Ty_Leaf) info->_unique = true;

    m_tasks.push_back(info);
    LoadingInfo::s_instance->setTaskCount(1);
    wakeup();
}

Node::Ptr loadFromGeometryDesc(GeometryDescriptors::Ptr desc, ReflexionModel::Ptr refl, Material::Ptr mat)
{
    TreeGenerator treegen;
    RockGenerator rockgen;

    Node::Ptr res;

    if(desc->_ty == GeometryDescriptors::Ty_Tree)
    {
        TreeDescriptor::Ptr model = std::dynamic_pointer_cast<TreeDescriptor>(desc);
        res = treegen.buildInstancedTree(*model,mat,refl);
    }
    else if(desc->_ty == GeometryDescriptors::Ty_Leaf)
    {
        TreeDescriptor::Ptr model = std::dynamic_pointer_cast<TreeDescriptor>(desc);
        res = treegen.buildInstancedLeaf(*model,mat,refl);
    }
    else if(desc->_ty == GeometryDescriptors::Ty_Rock)
    {
        RockDescriptor::Ptr model = std::dynamic_pointer_cast<RockDescriptor>(desc);
        res = rockgen.generateRock(*model, mat, refl, false);
    }
    else if(desc->_ty == GeometryDescriptors::Ty_Ground)
    {
        GroundDescriptor::Ptr model = std::dynamic_pointer_cast<GroundDescriptor>(desc);
        res = GroundGenerator::generate(model, refl, mat);
    }
    return res;
}

void LoaderThread::run()
{
    if(_currTask->_t == LoadingTaskInfoBase::Ty_Texture)
    {
        LoadingTaskTextureInfo* info = dynamic_cast<LoadingTaskTextureInfo*>(_currTask);
        qDebug() << "[LoadingTaskQueue] concurrent load (" << info->_url.c_str() << ")";
        info->result = TextureCache::instance()->load(info->_url);
        qDebug() << "[LoadingTaskQueue] concurrent load finished";
    }
    else if(_currTask->_t == LoadingTaskInfoBase::Ty_CubeMap)
    {
        LoadingTaskCubeMapInfo* info = dynamic_cast<LoadingTaskCubeMapInfo*>(_currTask);
        qDebug() << "[LoadingTaskQueue] concurrent load cubemap";
        info->result.resize(6);
        for(int i=0;i<6;++i)
            info->result[i] = TextureCache::instance()->load(info->_url[i]);
        qDebug() << "[LoadingTaskQueue] concurrent load finished";
    }
    else if(_currTask->_t == LoadingTaskInfoBase::Ty_Geometry)
    {
        LoadingTaskGeometryInfo* info = dynamic_cast<LoadingTaskGeometryInfo*>(_currTask);
        qDebug() << "[LoadingTaskQueue] concurrent load geometry";
        GeometryDescriptors::Ptr desc = info->_desc;

        info->_result = loadFromGeometryDesc(desc, info->_reflexion, info->_mat);

        qDebug() << "[LoadingTaskQueue] concurrent load finished";
    }
}

void LoaderThread::setCurrentTask(LoadingTaskInfoBase* task)
{
    // qDebug() << "set current task";
    _currTask = task;
    start();
}

void LoadingTaskQueue::loadNext()
{
    auto it = m_tasks.begin();
    if(it != m_tasks.end())
    {
        LoadingTaskInfoBase::Ptr task = (*it);
        if(task->_t == LoadingTaskInfoBase::Ty_Texture)
            LoadingInfo::s_instance->setMessage("load texture");
        if(task->_t == LoadingTaskInfoBase::Ty_CubeMap)
            LoadingInfo::s_instance->setMessage("load cubemap");
        if(task->_t == LoadingTaskInfoBase::Ty_Geometry)
            LoadingInfo::s_instance->setMessage("load geometry");

        requestLoading(it->get());
        m_loading = true;
    }
    else
    {
        m_loading = false;
    }
}

void LoadingTaskQueue::wakeup()
{
    if( !m_loading ) loadNext();
}

void LoadingTaskQueue::handleFinished()
{
    auto it = m_tasks.begin();
    LoadingTaskInfoBase::Ptr task = (*it);

    if(task->_t == LoadingTaskInfoBase::Ty_Texture)
    {
        LoadingTaskTextureInfo::Ptr info = std::dynamic_pointer_cast<LoadingTaskTextureInfo>(task);

        qDebug() << "[LoadingTaskQueue] handle finished (" << info->_url.c_str() << ")";
        // emit message("[LoadingTaskQueue] handle finished texture");
        imp::ImageSampler::Ptr sampler = imp::ImageSampler::create();
        sampler->setSource( info->result );
        sampler->setWrapping(imp::ImageSampler::Wrapping_Repeat);
        sampler->setFiltering(imp::ImageSampler::Filtering_Linear);
        sampler->_mipmap = true;
        LoadingInfo::s_instance->setMessage("load texture finished");

        if(info->_dstMatSmpl == MatComp_Color)
            info->_dstMat->_baseColor = sampler;
        else if(info->_dstMatSmpl == MatComp_Normal)
            info->_dstMat->_normalmap = sampler;
        else if(info->_dstMatSmpl == MatComp_Reflective)
            info->_dstMat->_reflectivity = sampler;
    }
    if(task->_t == LoadingTaskInfoBase::Ty_CubeMap)
    {
        LoadingTaskCubeMapInfo::Ptr info = std::dynamic_pointer_cast<LoadingTaskCubeMapInfo>(task);

        qDebug() << "[LoadingTaskQueue] handle finished cubemap";
        // emit message("[LoadingTaskQueue] handle finished cubemap");
        LoadingInfo::s_instance->setMessage("load cubemap finished");
        info->_dstHandler->handleCubemap(info->result);
    }
    if(task->_t == LoadingTaskInfoBase::Ty_Geometry)
    {
        LoadingTaskGeometryInfo::Ptr info = std::dynamic_pointer_cast<LoadingTaskGeometryInfo>(task);

        qDebug() << "[LoadingTaskQueue] handle finished geometry";
        // emit message("[LoadingTaskQueue] handle finished geometry");
        LoadingInfo::s_instance->setMessage("load geometry finished");
        if(info->_unique) info->_dstNode->clearNode();
        info->_dstNode->addNode(info->_result);
        // info->_graph->notifyChanges();
    }

    LoadingInfo::s_instance->setTaskCompleted(1);
    m_tasks.pop_front();
    loadNext();
}
