#ifndef LOADINGINFO_H
#define LOADINGINFO_H

#include <QObject>

class LoadingInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QString history READ history WRITE setHistory NOTIFY historyChanged)
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)

    Q_PROPERTY(qreal taskprogress READ taskProgress WRITE setTaskProgress NOTIFY taskProgressChanged)

    Q_PROPERTY(int editMode READ editMode WRITE setEditMode NOTIFY editModeChanged)
    Q_PROPERTY(int validMode READ validMode WRITE setValidMode NOTIFY validModeChanged)

public:
    LoadingInfo();
    virtual ~LoadingInfo() {}

    QString m_msg;
    QString m_history;
    qreal m_progress;

    int m_taskCount;
    int m_taskCompleted;
    float m_taskProgress;

    std::list<std::string> logArray;
    float lastRemove;

    int m_editMode;
    int m_validMode;

    QString message() {return m_msg;}
    void setMessage(QString msg);

    QString history() {return m_history;}
    void setHistory(QString msg){m_history=msg; emit historyChanged();}

    qreal progress() {return m_progress;}
    void setProgress(qreal progress){m_progress=progress; emit progressChanged();}

    static LoadingInfo* s_instance;

    void signalProgress(float value, QString msg);

    void setTaskCompleted(int c) { m_taskCompleted+=c;updateTaskProgress(); }
    void setTaskCount(int c) { m_taskCount+=c;updateTaskProgress(); }

    qreal taskProgress() {return m_taskProgress;}
    void setTaskProgress(qreal tp) {m_taskProgress=tp; emit taskProgressChanged();}

    int editMode() {return m_editMode;}
    void setEditMode(int editmode) {m_editMode=editmode; emit editModeChanged();}

    int validMode() {return m_validMode;}
    void setValidMode(int mode) {m_validMode=mode; emit validModeChanged();}

    void updateTaskProgress();

    void updateTime(float elapsed);

signals:
    void messageChanged();
    void historyChanged();
    void progressChanged();

    void taskProgressChanged();

    void editModeChanged();
    void validModeChanged();
};

#endif // LOADINGINFO_H
