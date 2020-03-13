#ifndef LOADINGTASKQUEUE_H
#define LOADINGTASKQUEUE_H

#include <QObject>
#include <QThread>

#include "DataControl/loadingtask.h"


class LoaderThread : public QThread
{
    Q_OBJECT
    void run() override;
public slots:
    void setCurrentTask(LoadingTaskInfoBase* task);
protected:
    LoadingTaskInfoBase* _currTask;
};


class LoadingTaskQueue : public QObject
{
    Q_OBJECT

public:
    LoadingTaskQueue();
    virtual ~LoadingTaskQueue();

    void request(imp::Material::Ptr material, MatCompTy dst, const std::string& url);
    void request(CubeMapHandler* skybox, const std::vector<std::string> urls);
    void request(imp::Graph::Ptr graph, imp::Node::Ptr dstNode, imp::ReflexionModel::Ptr reflexion, imp::Material::Ptr mat, GeometryDescriptors::Ptr desc);

    void loadNext();
    void wakeup();

    LoaderThread* loaderThread;
    bool m_loading;

    std::list<LoadingTaskInfoBase::Ptr> m_tasks;

    static LoadingTaskQueue* s_instance;
    static LoadingTaskQueue* instance();

public slots:
    void handleFinished();

signals:
    void message(QString s);
    void requestLoading(LoadingTaskInfoBase*);
};

#endif // LOADINGTASKQUEUE_H
