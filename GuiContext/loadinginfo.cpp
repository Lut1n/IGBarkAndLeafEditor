#include "loadinginfo.h"

#include <QDebug>

LoadingInfo* LoadingInfo::s_instance = nullptr;

LoadingInfo::LoadingInfo()
    : QObject()
{
    s_instance = this;
    m_progress = 0.0;

    m_taskCount = 0;
    m_taskCompleted = 0;
    m_taskProgress = 0;
    lastRemove = -1.0;

    m_editMode = 0;
    m_validMode = -1;
}

void LoadingInfo::signalProgress(float value, QString msg)
{
    qDebug() << "progress - " << msg;
    setProgress(value);
    setMessage(msg);
}

void LoadingInfo::setMessage(QString msg)
{
    logArray.push_back(msg.toStdString());
    m_msg+="\n"+msg;
    m_history+="\n"+msg;
    emit messageChanged();
    emit historyChanged();
}

void LoadingInfo::updateTaskProgress()
{
    m_taskProgress = m_taskCount==0?0.0:float(m_taskCompleted)/float(m_taskCount);
    emit taskProgressChanged();
}

void LoadingInfo::updateTime(float elapsed)
{
    if( logArray.empty() ) lastRemove = elapsed;

    static const float timer = 1.0;
    static const int max_msg = 6;

    if( (elapsed-lastRemove) >= timer || logArray.size()>=max_msg )
    {
        m_msg = "";
        if(!logArray.empty())logArray.pop_front();
        for(auto s : logArray) m_msg += "\n" + QString(s.c_str());
        emit messageChanged();

        lastRemove = elapsed;
    }
}
