#ifndef DISPLAYOPTIONS_H
#define DISPLAYOPTIONS_H

#include <QObject>

class DisplayOptions : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool environment READ environment WRITE setEnvironment NOTIFY environmentChanged)
    Q_PROPERTY(bool phong READ phong WRITE setPhong NOTIFY phongChanged)
    Q_PROPERTY(bool shadow READ shadow WRITE setShadow NOTIFY shadowChanged)
    Q_PROPERTY(bool bloom READ bloom WRITE setBloom NOTIFY bloomChanged)
    Q_PROPERTY(bool ssao READ ssao WRITE setSSAO NOTIFY ssaoChanged)

    Q_PROPERTY(QString gbuffer READ gbuffer WRITE setGBuffer NOTIFY gbufferChanged)
    Q_PROPERTY(int gBufferIndex READ gBufferIndex WRITE setGBufferIndex NOTIFY gBufferIndexChanged)

public:
    DisplayOptions();
    virtual ~DisplayOptions() {}

    static DisplayOptions* s_instance;

    bool m_phong;
    bool m_environment;
    bool m_shadow;
    bool m_bloom;
    bool m_ssao;

    QString m_gBuffer;
    int m_gBufferIndex;

    bool environment() {return m_environment;}
    void setEnvironment(bool environment){m_environment=environment; emit environmentChanged();}

    bool phong() {return m_phong;}
    void setPhong(bool phong){m_phong=phong; emit phongChanged();}

    bool shadow() {return m_shadow;}
    void setShadow(bool shadow){m_shadow=shadow; emit shadowChanged();}

    bool bloom() {return m_bloom;}
    void setBloom(bool bloom){m_bloom=bloom; emit bloomChanged();}

    bool ssao() {return m_ssao;}
    void setSSAO(bool ssao){m_ssao=ssao; emit ssaoChanged();}

    QString gbuffer() {return m_gBuffer;}
    void setGBuffer(QString gbuffer){m_gBuffer=gbuffer; emit gbufferChanged();}

    int gBufferIndex() {return m_gBufferIndex;}
    void setGBufferIndex(int gbufferindex){m_gBufferIndex=gbufferindex; emit gBufferIndexChanged();}

signals:
    void environmentChanged();
    void phongChanged();
    void shadowChanged();
    void ssaoChanged();
    void bloomChanged();
    void gbufferChanged();
    void gBufferIndexChanged();
};

#endif // DISPLAYOPTIONS_H
