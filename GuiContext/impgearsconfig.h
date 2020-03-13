#ifndef IMPGEARSCONFIG_H
#define IMPGEARSCONFIG_H

#include <QObject>

class ImpGearsConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool transparency READ transparency WRITE setTransparency NOTIFY transparencyChanged)
    Q_PROPERTY(bool animated READ animated WRITE setAnimated NOTIFY animatedChanged)
    Q_PROPERTY(int shadowRes READ shadowRes WRITE setShadowRes NOTIFY shadowResChanged)
    Q_PROPERTY(int environmentRes READ environmentRes WRITE setEnvironmentRes NOTIFY environmentResChanged)
    Q_PROPERTY(int shadowSamples READ shadowSamples WRITE setShadowSamples NOTIFY shadowSamplesChanged)
    Q_PROPERTY(int ssaoSamples READ ssaoSamples WRITE setSsaoSamples NOTIFY ssaoSamplesChanged)
    Q_PROPERTY(qreal ambient READ ambient WRITE setAmbient NOTIFY ambientChanged)
    Q_PROPERTY(qreal power READ power WRITE setPower NOTIFY powerChanged)

public:

    static ImpGearsConfig* s_instance;

    ImpGearsConfig();
    virtual ~ImpGearsConfig() {}

    bool transparency() {return m_transparencyEnabled;}
    void setTransparency(bool transparency){m_transparencyEnabled=transparency;emit transparencyChanged();}

    bool animated() {return m_animationEnabled;}
    void setAnimated(bool anim){m_animationEnabled=anim;emit animatedChanged();}

    int shadowRes() {return m_shadowRes;}
    void setShadowRes(int res){m_shadowRes=res; emit shadowResChanged();}

    int environmentRes() {return m_environmentRes;}
    void setEnvironmentRes(int res){m_environmentRes=res; emit environmentResChanged();}

    int shadowSamples() {return m_shadowSamples;}
    void setShadowSamples(int samples){m_shadowSamples=samples; emit shadowSamplesChanged();}

    int ssaoSamples() {return m_ssaoSamples;}
    void setSsaoSamples(int samples){m_ssaoSamples=samples; emit ssaoSamplesChanged();}

    qreal ambient() {return m_ambient;}
    void setAmbient(qreal a){m_ambient=a; emit ambientChanged();}

    qreal power() {return m_power;}
    void setPower(qreal p){m_power=p; emit powerChanged();}

signals:
    void transparencyChanged();
    void animatedChanged();
    void shadowResChanged();
    void environmentResChanged();
    void shadowSamplesChanged();
    void ssaoSamplesChanged();
    void ambientChanged();
    void powerChanged();

protected:

    bool m_transparencyEnabled;
    bool m_animationEnabled;

    int m_shadowRes;
    int m_environmentRes;
    int m_shadowSamples;
    int m_ssaoSamples;

    float m_ambient;
    float m_power;
};

#endif // IMPGEARSCONFIG_H
